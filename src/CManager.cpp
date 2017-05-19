/*
 *  CManager.cpp
 *
 *  Created on: Jun 11, 2015
 *  Author: javier.navarro@caos.uab.es
 *  Copyright (c) 2015 Javier Navarro, GNU license.
 *
 *  Original work from:
 *  GHcaller
 *  Copyright (c) 2013 Bruno Nevado, GNU license.
 *
 *  This file is part of PGHcaller.
 *
 *  PGHcaller is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PGHcaller is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PGHcaller.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CManager.h"


namespace ngh {

template <class T>
CManager<T>::CManager(ngh::COptions* options) {
    set_options(options);
    // Create event log object and start events
#if MPE
    events_ = new ngh::MPEEventLog();
#else
    events_ = new ngh::DummyEventLog();
#endif
    events_->start();
}

template <class T>
CManager<T>::~CManager() {
}

template <class T>
void CManager<T>::set_options(ngh::COptions* options) {
    options_ = options;

    // Set verbose (INFO) or TODO debug (DEBUG)
    input_filename_ = options_->get_cmd_arguments()->getInputFile();
    output_filename_ = options_->get_cmd_arguments()->getOutputFile();
    outgroup_filename_ = options_->get_cmd_arguments()->getOutgroupFile();
}

template <class T>
void CManager<T>::set_taskmanager(ngh::MasterWorker<T>* taskmanager) {
    taskmanager_ = taskmanager;
}

template <class T>
void CManager<T>::set_algorithm(ngh::Algorithm::CAlgorithm* algorithm) {
    algorithm_ = algorithm->get_algorithm();
}

template <class T>
void CManager<T>::set_output_writer(ngh::FileWriter::CFileWriter* writer) {
    this->writer_ = writer;
}

template <class T>
void CManager<T>::run() {
    initialize();

    switch ( role ) {
    case ngh::ROLE::MASTER:
        run_master();
        break;

    case ngh::ROLE::WORKER:
        run_worker();
        break;

    default:
        DEBUG_("Cannot deal with a UNDEF role ");

    }
    finalize();
}

template <class T>
void CManager<T>::run_master() {
    LOG_("Indexing file " << options_->get_cmd_arguments()->getInputFile());
    events_->event(ngh::EventType::INDEX, ngh::EventAction::START);
    /*
     *  Split file and generate table of indexes
     */
    std::vector <ngh::Window> task_pool;

    /*
     * we have to the check number of workers & threads
     * in order to balance number of tasks between all of them
     */
    double t1, t2;
    t1 = MPI_Wtime();
    // TODO(jnavarro): if no size were provided, calculate chunk size using ploidy,
    // individuals, and total number of sites.
    inputfile_->gen_chunk_pool(task_pool, options_->get_cmd_arguments()->getChunksize());
    t2 = MPI_Wtime();
    LOG_("Time indexing file  " << t2 - t1 << " seconds");

    LOG_("New chunks from file " <<  task_pool.size() << ", chunk size " << options_->get_cmd_arguments()->getChunksize());
    events_->event(ngh::EventType::INDEX, ngh::EventAction::STOP);

    // Wait genChunPool before continue
    taskmanager_->barrier();

    // Register workers
    // DEBUG_("Registering workers " << taskmanager_->getWorkers());
    taskmanager_->registerWorkers();
    // events.event(EventType::REGISTER);

    // Start server. Assign job to all workers
    LOG_("Assign tasks [" << task_pool.size() << "] to all workers [" << taskmanager_->getWorkers() << "]");
    events_->event(ngh::EventType::ASSIGNTASKS, ngh::EventAction::START);

    // taskmanager_->assignTasks(task_pool, tasks_by_worker);
    taskmanager_->assignTasks(task_pool);
    events_->event(ngh::EventType::ASSIGNTASKS, ngh::EventAction::STOP);

    // Work is done! End workers
    // LOG_("All tasks sent. Waiting to end workers");
    taskmanager_->endWorkers();
    DEBUG_("All ending messages sent to workers. Finishing ..");
    // Get time elapsed
    // LOG_("#PROCESS " <<  events.elapsed() << " " << events.elapsedSeconds());
    // events.event(EventType::ASSIGNTASKS);
}

template <class T>
void CManager<T>::run_worker() {
    ngh::SeqProperties *properties = new ngh::SeqProperties(inputfile_->get_individuals(), 2);
    ngh::RoestiCaller *algorithm = new ngh::RoestiCaller();
    algorithm->set_mread( options_->get_cmd_arguments()->getMinReads() );
    algorithm->set_error( options_->get_cmd_arguments()->getPErr() );
    algorithm->set_chi( options_->get_cmd_arguments()->getChi() );
    algorithm->set_haplocalls( options_->get_cmd_arguments()->getHaplocalls() );

    MEM_MSG("#_"<< taskmanager_->getRank() << "_Memory_Usage " << getMemoryUsage() << " (Start)");
    // This state changes in "NO_MORE_TASK" event
    bool done = false;
    while ( !done ) {
        events_->event(ngh::EventType::WORKER_WAIT, ngh::EventAction::START);
        ngh::MSG request = taskmanager_->sendRequest();
        events_->event(ngh::EventType::WORKER_WAIT, ngh::EventAction::STOP);

        switch ( request ) {
        case ngh::MSG::REGISTERED: {
             DEBUG_("Worker " << taskmanager_->getRank() << " registered from master");

            break;
        }
        case ngh::MSG::GET_DATA: {
	    try {
            // ngh::STATUS status = TaskManager.getData(chunk);
	        ngh::Window data;
            taskmanager_->getData(data);
            // READ
            events_->event(ngh::EventType::WORKER_READ, ngh::EventAction::START);
            // Window
            DEBUG_("[" << taskmanager_->getRank() << "] Reading buffer from " << data.file_start << " to " << data.file_end);
            ngh::Chunk bufferchunk(data.file_start, data.file_end);
            inputfile_->read_buffer(bufferchunk);
            events_->event(ngh::EventType::WORKER_READ, ngh::EventAction::STOP);
            MEM_MSG("#_"<< taskmanager_->getRank() << "_Memory_Usage READ " << getMemoryUsage() << "");
	    
            // PARSE
            // Check time spend here
            events_->event(ngh::EventType::WORKER_PARSE, ngh::EventAction::START);
            DEBUG_("[" << taskmanager_->getRank() << "] Processing window "
                        << data.site_start << ":" << data.site_end << "-" << data.sites()
                        <<" , " << data.file_start << ":" << data.file_end << "-" << data.size());
            // Create vector of sites with N individuals
            std::vector<ngh::Site> sites(data.sites(), ngh::Site(properties->individuals));
            DEBUG_("[" << taskmanager_->getRank() << "] Parsing buffer with sites #" << sites.size());
            inputfile_->parse(data, &sites);
            MEM_MSG("#_"<< taskmanager_->getRank() << "_Memory_Usage PARSER " << getMemoryUsage() << "");
            events_->event(ngh::EventType::WORKER_PARSE, ngh::EventAction::STOP);
	
            // PROCESS    
            // Create basecalls object to store results. We use data[j].nsites, ploidy,
            // and number of sequences to dim internal size
            events_->event(ngh::EventType::WORKER_PROCESS, ngh::EventAction::START);
            ngh::CBasecalls basecalls(properties, &data);
            DEBUG_("[" << taskmanager_->getRank() << "] Processing sites #" << sites.size());
            algorithm->process(properties, &sites, &basecalls);
            // sites.clear();
            MEM_MSG("#_"<< taskmanager_->getRank() << "_Memory_Usage PROCESS " << getMemoryUsage() << "");
            events_->event(ngh::EventType::WORKER_PROCESS, ngh::EventAction::STOP);
	    	
            // WRITE RESULTS
            events_->event(ngh::EventType::WORKER_WRITE, ngh::EventAction::START);
            DEBUG_("[" << taskmanager_->getRank() << "] Building fasta with basecalls #" << basecalls.size());
            writer_->build(properties, data, basecalls);
            // basecalls.clear();
            MEM_MSG("#_"<< taskmanager_->getRank() << "_Memory_Usage WRITER " << getMemoryUsage() << "");
            events_->event(ngh::EventType::WORKER_WRITE, ngh::EventAction::STOP);
             
    	    } catch (const std::bad_alloc& e) {
                ERR_("Allocation failed: " << e.what());
                exit(EXIT_FAILURE);
            }
            break;
        }
        case ngh::MSG::SEND_STATS: {
            DEBUG_("Master requests stats");
            // Send stats to master
            break;
        }

        case ngh::MSG::NO_MORE_TASKS: {
            // release/flush fasta
            // outputfile_->finalize();
            done = true;
            break;
        }
        default:
        {
            ERR_("Error! This message is not defined");
            break;
        }
        }
    }
    free(properties);
    free(algorithm);
}


template <class T>
void CManager<T>::initialize() {
    // get roel and save to use later
    role = taskmanager_->getRole();
    
    // Events start here
    events_->event(ngh::EventType::INIT, ngh::EventAction::START);
    /*
     *  Open PILEUP file and get/check number of individuals.
     */
    // INCLUDE ALL THIS STUFF IN CLASS
    inputfile_ = new ngh::Reader::PileupFile(input_filename_);

    // Set init site, end site, and check number of individuals in file
    // TODO(jnavarro): Check file format!
    inputfile_->initialize();

    // Store individuals to use later by other classes
    options_->set_individuals(inputfile_->get_individuals());

    // Set filter options in parser
    inputfile_->set_filter_options(options_->get_filters()->getFilterOptions());

    /* Create output file to store all results in multiFASTA format
     * NEXT: We should deal with other output file formats
     *
     * We start always at position 1. To change this, we have to change getPosition()
     * and build() functions
     * Default end site comes from mpileup file. We should check if there is a
     * reference/outgroup file.
     */
    writer_->set_individuals(inputfile_->get_individuals());
    writer_->set_names(options_->get_names());
    writer_->set_output_filename(output_filename_);
    writer_->set_init_site(inputfile_->get_init_site());
    writer_->set_end_site(inputfile_->get_end_site());

    // TODO(jnavarro) remove unneded stuff here
    // If outgroup file is defined, create new fasta file and check end site
    if (!outgroup_filename_.empty()) {
        fasta_ = new ngh::FastaFile(outgroup_filename_);
        if (inputfile_->get_end_site() > fasta_->getNumBases()) {
	    if(role == ngh::ROLE::MASTER) {
                DEBUG_("Outgroup/reference file from command line  " <<  outgroup_filename_);
                WAR_("Outgroup/reference disabled: mpileup has " << inputfile_->get_end_site() << " bases " 
                    << "but reference has " <<   fasta_->getNumBases() << " bases.");
            }
            writer_->set_end_site(inputfile_->get_end_site());
            // We cannot append outgroup file
            outgroup_append_ = false;
        } else {
	    if(role == ngh::ROLE::MASTER) {
                LOG_("Outgroup/reference file "<< fasta_->getFilename()
                    <<" name " << fasta_->getName() << " and "
                    << fasta_->getNumBases() << " bases.");
            }
	    // Remember we should fill from end site to fasta end site with Ns
            writer_->set_fasta_end_site(fasta_->getNumBases());
            // if if init and end sites are correct, and append option is
            // enabled, then set append flag
            if(options_->doAppendFasta()) {
                outgroup_append_ = true;
            }

        }
    } else {
        if(role == ngh::ROLE::MASTER) {
            DEBUG_("Using values from MPILEUP file  site 1, " << inputfile_->get_end_site());
        }
        writer_->set_end_site(inputfile_->get_end_site());
        // We cannot append outgroup file
        outgroup_append_ = false;
    }

    switch ( role ) {
    case ngh::ROLE::MASTER: {
        if (ngh::ARGUMENTS_STATUS::ARGUMENT_ERROR == options_->get_cmd_arguments()->check()) {
            MPI_ERR_END(EXIT_FAILURE);
        }
        /* Check number of workers. We need at least a master, one worker,
         * and one writer
         */
        if (taskmanager_->getWorkers() < 1) {
            ERR_("More than 1 process is needed to do the job!! Set number_of_processes > 1 ");
            // TODO(jnavarro): Error hanling here!
            // MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            MPI_ERR_END(EXIT_FAILURE);
            // taskmanager_.finalize();
            // exit(EXIT_FAILURE);
        }

        // taskmanager_->QueryMPIThread();
        LOG_("Input file is " << options_->get_cmd_arguments()->getInputFile());
        LOG_("Number of individuals " << inputfile_->get_individuals());
        /*
         * We could get from MPILEUP file (last site/position) or
         * get from outgroup/reference file (length).
         *
         * NEXT: To implement check reference length
         */
        LOG_("Detected pileup start/end site [" << inputfile_->get_init_site() << "-" << inputfile_->get_end_site() << "]");

        // Show filter options
        // DEBUG_(options_->get_filters()->print_option());
        DEBUG_(options_->get_filters()->print_option_debug());
        DEBUG_("Starting MASTER");
        /*
         * Initialize a empty FASTA file
         *
         * We need to known the number of bases in output fasta.
         * We could get from MPILEUP file (last site/position) or
         * get from outgroup/reference file (length).
         *
         * NEXT: To implement check reference length
         */
        // Get handler to write in FASTA output file
        writer_->initialize();

        // Move write_headers and fill_with_n
        writer_->write_headers();

        // Prepare output file writing headers, filling init sites and end sites with Ns
        writer_->fill_init_sites_with_n();

        //end writer_->fill_with_ns(inputfile_->get_end_site() > fasta_->getNumBases());
        writer_->fill_end_sites_with_n();

        break;
    }
    case ngh::ROLE::WORKER: {
        // TODO(jnavarro)
        // algorithm_->set_cmd_options(options);
        // algorithm_->initialize();

        // Get handler to write in FASTA output file
        writer_->initialize();

        // Wait master genPool Task and Writer generate FASTA mFilaHanler
        taskmanager_->barrier();
        break;
    }
    default: {
            ERR_("Cannot deal with a UNDEF role ");
    }

    // Events stop
    events_->event(ngh::EventType::INIT, ngh::EventAction::STOP);
    }
}

template <class T>
void CManager<T>::finalize() {
    // Wait all processes before continue
    taskmanager_->barrier();

    // Close file
    // DEBUG_("Finalizing manager");
    inputfile_->finalize();
    // DEBUG_(" Read handler closed " << taskmanager_->getRank());
    switch ( role ) {
    case ngh::ROLE::MASTER:
        /*
         * Append outgroup sequence if outgroup file was defined
         * or not disabled in previous checks
         */
         if ( outgroup_append_ ) {
             LOG_("Append outgroup sequence " << fasta_->getFilename()
                     << ", number of bases " << fasta_->getNumBases() );
             events_->event(ngh::EventType::APPENDFASTA, ngh::EventAction::START);
             // outputMFasta.appendFasta( fasta );

             // LOG_("Append outgroup sequence " << outgroupfile );
             // outputFasta.appendOutgroupFilename();
             // outputfile_->appendFasta(outgroup_filename_);
        /*  TODO:
         *  Now we don't care about size of the reference but use initSite and endSite values
         *  to append reference to output file.
         *  We implement a new class to open FASTA and we will pass this
         *  object to a method whose append new file contents to mFASTA.
         *
         *  FastaFile outgroupFasta(outgroup_filename);
         *  outputMFasta.appendFasta(outgroupFasta);
         */
             writer_->append_fasta(outgroup_filename_);
             events_->event(ngh::EventType::APPENDFASTA, ngh::EventAction::STOP);
         }
        break;

    case ngh::ROLE::WORKER:
        algorithm_->finalize();
        // LOG_("Write " << taskmanager_->getRank());
        // writer_->finalize();
        break;
    default:
        ERR_("Cannot deal with a UNDEF role ");
    }
    writer_->finalize();
    // DEBUG_(" Write handler closed " << taskmanager_->getRank());

    // Wait all and end taskmanager
    taskmanager_->barrier();

    // Stop event logger
    events_->stop();
}

template class CManager<Window>;

} /* namespace ngh */
