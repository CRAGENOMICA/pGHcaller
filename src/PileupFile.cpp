/*
 * PileupFile.cpp
 *
 *  Created on: Nov 7, 2014
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

#include "PileupFile.h"

namespace ngh {
namespace Reader {

PileupFile::PileupFile(std::string filename) {
    logger = ngh::Logger::getInstance();
    this->filename_  = filename;
    number_of_individuals_ = 0;
    init_site_ = 0;
    end_site_ = 0;
    is_initialized_ = false;
}

PileupFile::~PileupFile() {
}

std::string* PileupFile::get_buffer() {
	return &buffer_;
}

void PileupFile::parse(const Window& window, std::vector<ngh::Site>* sites) {
    parser->parse_buffer_to_reads(&buffer_, window, sites);
    free_buffer();
}

void PileupFile::initialize() {
    if (!is_initialized_) {
        if (EXIT_FAILURE == open()) {
            ERR_("Exception opening/reading input file " << filename_);
            // TODO(jnavarro) Throw exception here!!!
            // trowsMPI_ERR_END(EXIT_FAILURE);
        }
        number_of_individuals_ = getNumIndividualsFromFile();
        init_site_ = getInitSiteFromFile();
        end_site_ = getEndSiteFromFile();
    }
    is_initialized_ = true;
}

void PileupFile::set_filter_options(FilterOptions* filters) {
	assert(is_initialized_);
    // Parser
    parser = new ngh::MpileupParser(number_of_individuals_, filters);
}

void PileupFile::finalize() {
    close();
}

uint64_t PileupFile::get_individuals() {
    assert(is_initialized_);
    return ( number_of_individuals_ );
}

uint64_t PileupFile::get_init_site() {
	assert(is_initialized_);
	return ( init_site_ );
}

uint64_t PileupFile::get_end_site() {
	assert(is_initialized_);
	return (end_site_);
}

uint64_t PileupFile::getNumIndividualsFromFile() {
    // unsigned int getNumIndividuals(std::string filename){
    std::string line;
    in.seekg(0, in.beg);
    getline(in, line);
    in.seekg(0, in.beg);
    uint64_t fields = 0;
    char *ptr = &line[0];
    while ((ptr = strchr(ptr, '\t')) != NULL) {
        ++fields;
        ptr++;
    }
    // Remove two extra fields from site and reference from mpileup
    // Each individual has 3 fields, so indiviuals should be (number_of_fields) / 3
    return ((fields-2)/3);
}

uint64_t PileupFile::getInitSiteFromFile() {
    std::string line;
    in.seekg(0, in.beg);
    getline(in, line);
    char *ptr = &line[0];
    if ((ptr = strchr(ptr, '\t')) != NULL) {
        ptr++;
        return( std::atol(ptr) );
    }
    return 0L;
}


uint64_t PileupFile::getEndSiteFromFile() {
    // TODO Check blanck lines, and comments in file. Check last line null
    // in.seekg (-2, in.end);
    in.seekg(-2, std::ios_base::end);
    bool keepLooping = true;
    while (keepLooping) {
        char ch;
        in.get(ch);                  // Get current byte's data
        if ((long)in.tellg() <= 1) { // If the data was at or before the 0th byte
            in.seekg(0);             // The first line is the last line
            keepLooping = false;     // So stop there
        } else if (ch == '\n') {     // If the data was a newline
            keepLooping = false;     // Stop at the current position.
        } else {                     // If the data was neither a newline nor at the 0 byte
            // Move to the front of that data, then to the front of the data before it
            in.seekg(-2, std::ios_base::cur);
        }
    }

    // Get line
    std::string lastLine;
    getline(in, lastLine);

    // Parse site from mpileup file
    char *ptr = &lastLine[0];
    if ((ptr = strchr(ptr, '\t')) != NULL) {
        ptr++;
        return( std::atol(ptr) );
    }
    return 0L;
}

int32_t
PileupFile::open() {
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        in.open(filename_, std::ios::binary);
        in.seekg(0, in.beg);
        return EXIT_SUCCESS;
    } catch (std::ifstream::failure e) {
        // ERR_("Exception opening/reading input file ");
        return EXIT_FAILURE;
    }
}

int32_t
PileupFile::close() {
    in.close();
    is_initialized_ = false;
    return EXIT_FAILURE;
}

int32_t
PileupFile::gen_chunk_pool(std::vector<Chunk>& ChunkPool, const uint64_t chunksize) {
    try {
        std::string line;
        uint64_t get_counter = chunksize, last_counter = 0;
        // We start at taskId=1
        uint64_t id = 1;
        uint64_t filesize;

        // get length of file:
        in.seekg(0, in.end);
        filesize = in.tellg();
        in.seekg(0, in.beg);
        LOG_("Working with input file ( " << filesize/(1024*1024)
                << " MB ) and chunk size of " << chunksize/(1024*1024) << " MB");
        ChunkPool.reserve((unsigned int) filesize/chunksize);
        ngh::Chunk chunk;
        while (get_counter < filesize) {
            in.seekg(get_counter);
            getline(in, line);
            // variable1=atof(line.c_str());
            // Save end line position
            get_counter += line.length();
            PILEUP("length " <<  line.size() << " char ##" << line[line.length()] << "##" );
            PILEUP("length " <<  line.length() << " line ##" << line << "##" );

            // Save Chunk position
            // chunk.id = id;
            chunk.start = last_counter; chunk.end = get_counter;
            ChunkPool.push_back(chunk);
            // ChunkPool.push_back(Chunk(id, last_counter, get_counter));
            id++;
            PILEUP("Chunk #" << ChunkPool.size() << " [" << last_counter << "-" <<get_counter << "]");
            // Next to eol. Move to begin of new line
            last_counter = get_counter+1;
            // Jump to next position = eol + CHUNK and find next endline
            get_counter += chunksize;
        }

        // Record last chunk only if it not the end
        if ((last_counter < filesize)) {
            // chunk.id = id;
            chunk.start = last_counter; chunk.end = filesize;
            ChunkPool.push_back(chunk);
            // ChunkPool.push_back(Chunk(id, last_counter, filesize));
            PILEUP("FINAL Chunk #" << ChunkPool.size() << " [" << last_counter << "-"<< filesize << "]");
            /*
			in.seekg (last_counter);
			while(getline(in, line))
			{
				PILEUP("FINAL #" << line);
			}
             */
        }
        // Go to begin of the file
        in.seekg(0, in.beg);
        return EXIT_SUCCESS;
    } catch(std::ifstream::failure e) {
        ERR_("Exception opening/reading individuals from file " << filename_);
        return EXIT_FAILURE;
    }
}

int32_t
PileupFile::gen_chunk_pool(std::vector<Window>& pool, const uint64_t chunksize) {
    try {
        std::string line;
        uint64_t get_counter = chunksize, last_counter = 0;
        uint64_t filesize;

        // get length of file:
        in.seekg(0, in.end);
        filesize = in.tellg();
        in.seekg(0, in.beg);
        if (filesize/(1024*1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024*1024) << " MB) and chunk size of " << chunksize/(1024*1024) << " MB");
        } else if (filesize/(1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024) << " KB) and chunk size of " << chunksize/(1024*1024) << " MB");
        } else {
            LOG_("Working with input file (" << filesize << " B) and chunk size of " << chunksize/(1024*1024) << " MB");
        }

        // Reserve pool. Check if we should reserve more than 1 chunk
        if(filesize > chunksize) {
            pool.reserve((unsigned int) filesize/chunksize);

        ngh::Window window;

        // Get where window starts
        getline(in, line);
        window.site_start = get_locus(&line[0]);

        while (get_counter + chunksize < filesize) {
            // Jump to next position and find EOL
            in.seekg(get_counter + chunksize);
            getline(in, line);
            get_counter = in.tellg();

	    // if(window.site_start > 27171096) LOG_("DBG: " << window.site_start << ":" << window.site_end );
            
	    // Get where window ends
            if (get_counter < filesize) {
                getline(in, line);
                // Save end line position
	        // if(window.site_start > 27171096) LOG_("DBG: -->" << window.site_start << ":" << window.site_end );
		// if(window.site_start > 27171096) LOG_( line );
                window.site_end = get_locus(&line[0]);
	        // if(window.site_start > 27171096) LOG_("DBG: <--" << window.site_start << ":" << window.site_end );
                get_counter = in.tellg();
                // window.nsites = window.site_start - window.site_end + 1;
                // PILEUP("length " <<  line.size() << " char ##" << line[line.length()] << "##" );
                // PILEUP("length " <<  line.length() << " line ##" << line << "##" );
                window.file_start = last_counter; window.file_end = get_counter-1;
                window.nsites = window.site_end - window.site_start + 1;
	        if(false == window.check()) {
                     ERR_("Error creating window from file with size " << filesize << " window:"  
                        << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
 		     exit(EXIT_FAILURE);
                }
                PILEUP("Window [" << pool.size() << "] " 
                        << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
                pool.push_back(window);

                PILEUP("Window [" << pool.size() << "] "
                        << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
                last_counter = get_counter;
            }

            // Get where window starts
            if (get_counter < filesize) {
                getline(in, line);
                window.site_start = get_locus(&line[0]);
            }
	    // if(window.site_start > 27171096) LOG_("DBG: " << window.site_start << ":" << window.site_end );
        }

        // Record last chunk only if it not the end
        if ((last_counter < filesize)) {
            // chunk.id = id;
            window.file_start = last_counter; window.file_end = filesize;

            // Get last site
            last_counter = filesize - 1;
            char c;
            do {
                in.seekg(--last_counter);
                in.get(c);
                // PILEUP("Char |" << c << "| at " << last_counter );
            } while( c != '\n' );
            in.seekg(last_counter + 1);
            // Get where window end
            getline(in, line);
            window.site_end = get_locus(&line[0]);
            window.nsites = window.site_end - window.site_start + 1;

	    // TODO(jnavarro) Exceptions? Check window
	    if(false == window.check()) {
                ERR_("Error creating window from file with size " << filesize << " window:"
                     << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                     << window.file_start << ":" << window.file_end << "-" << window.size());
 		exit(EXIT_FAILURE);
            }
            PILEUP("Window [" << pool.size() << "] " 
                        << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
            pool.push_back(window);
            PILEUP("Last Window [" << pool.size() << "] "
                                << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                                << window.file_start << ":" << window.file_end << "-" << window.size());
        }
        } else {
            // file is too little to use more than one chunk
            pool.reserve(1);
            ngh::Window window;
            // Get where window starts
            in.seekg(0, in.beg);
            getline(in, line);
            window.site_start = get_locus(&line[0]);

            // Get last site
            last_counter = filesize - 1;
            char c;
            do {
                in.seekg(--last_counter);
                in.get(c);
            } while( c != '\n' );
            in.seekg(last_counter + 1);
            // Get where window end
            getline(in, line);
            window.site_end = get_locus(&line[0]);
            window.nsites = window.site_end - window.site_start + 1;
            window.file_start = 0;
            window.file_end = filesize;
            pool.push_back(window);
            PILEUP("Last Window [" << pool.size() << "] "
                                 << window.site_start << ":" << window.site_end << "-" << window.sites() <<" , "
                                 << window.file_start << ":" << window.file_end << "-" << window.size());
        }

        // Go to begin of the file
        in.seekg(0, in.beg);
        return EXIT_SUCCESS;
    } catch(std::ifstream::failure e) {
        ERR_("Exception opening/reading individuals from file " << filename_);
        return EXIT_FAILURE;
    }
}

int32_t
PileupFile::gen_window_pool(std::vector<Window>& pool, const uint64_t window_size) {
    try {
        std::string line;
        uint64_t filesize, fpos = 0, scounter = 1;

        // get length of file:
        in.seekg(0, in.end);
        filesize = in.tellg();
        in.seekg(0, in.beg);
        if (filesize/(1024*1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024*1024) << " MB) and window size of " << window_size << " sites");
        } else if (filesize/(1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024) << " KB) and window size of " << window_size << " sites");
        } else {
            LOG_("Working with input file (" << filesize << " B) and window size of " << window_size << " sites");
        }

        // Set file start/end and site start/end to 0
        // ngh::Window window(0, 0, 0, 0, 0);
        ngh::Window window;
        // File start = 0
        window.file_start = 0;
        window.file_end = 0;

        // Start
        getline(in, line);
        window.site_start = get_locus(&line[0]);
        window.file_start = 0;
        window.file_end = in.tellg();
        window.nsites  = scounter;
        fpos = in.tellg();

        while ((fpos < filesize) &&  getline(in, line)) {
            ++scounter;
            fpos = in.tellg();
            // Read lines until reach window_size
            if (scounter == window_size) {
                window.file_end = fpos;
                window.site_end = get_locus(&line[0]);
                window.nsites = scounter;

                PILEUP("Window [" << pool.size() << "] "
                        << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
                pool.push_back(window);
                // Update next window. Next to EOL
                window.site_start = window.site_end + 1;
                window.file_start = window.file_end + 1;
                scounter = 0;
            }
        }
        if (fpos <= filesize) {
            window.file_end = fpos;
            window.site_end = get_locus(&line[0]);
            window.nsites = scounter;
            pool.push_back(window);
            PILEUP("Last Window [" << pool.size() << "] "
                    << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
                    << window.file_start << ":" << window.file_end << "-" << window.size());
        }

        // Go to begin of the file
        in.seekg(0, in.beg);
        return EXIT_SUCCESS;
    } catch(std::ifstream::failure e) {
        ERR_("Exception opening/reading individuals from file " << filename_);
        return EXIT_FAILURE;
    }
}
/* Acaba en site 31 cuando debería asignar 32, empieza en site 32 cuando debería empezar en 33
int32_t
PileupFile::gen_window_pool(std::vector<Window>& pool, const uint64_t window_size) {
    try {
        std::string line;
        uint64_t filesize, fpos = 0, scounter = 1, locus = 0;

        // get length of file:
        in.seekg(0, in.end);
        filesize = in.tellg();
        in.seekg(0, in.beg);
        if (filesize/(1024*1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024*1024) << " MB) and window size of " << window_size << " sites");
        } else if (filesize/(1024) > 0 ) {
            LOG_("Working with input file (" << filesize/(1024) << " KB) and window size of " << window_size << " sites");
        } else {
            LOG_("Working with input file (" << filesize << " B) and window size of " << window_size << " sites");
        }

        // Set file start/end and site start/end to 0
        // ngh::Window window(0, 0, 0, 0, 0);
        ngh::Window window;
        // File start = 0
        window.file_start = 0;
        window.file_end = 0;
        bool parse = true, in_start = false;

        // Start
        getline(in, line);
        window.site_start = get_locus(&line[0]);
        window.file_start = 0;
        window.file_end = in.tellg();
        window.nsites  = scounter;
        fpos = in.tellg();

        while ((fpos < filesize) &&  getline(in, line)) {
            ++scounter;
            fpos = in.tellg();
            // Read lines until reach window_size
            if (scounter < window_size) {
                locus = get_locus(&line[0]);
                // getline(in, line);

            } else {
                window.file_end = fpos;
                window.site_end = locus;
                window.nsites = scounter;

                PILEUP("Window [" << pool.size() << "] "
                        << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
                        << window.file_start << ":" << window.file_end << "-" << window.size());
                pool.push_back(window);
                //            PILEUP("Pool size " << pool.size());

                // Update next window. Next to EOL
                window.site_start = get_locus(&line[0]);
                window.file_start = window.file_end + 1;
                scounter = 0;
            }
        }
        if (fpos <= filesize) {
            window.file_end = fpos;
            window.site_end = locus;
            window.nsites = scounter;
            pool.push_back(window);
            PILEUP("Last Window [" << pool.size() << "] "
                    << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
                    << window.file_start << ":" << window.file_end << "-" << window.size());
        }
        PILEUP("Pool size " << pool.size());

        // Go to begin of the file
        in.seekg(0, in.beg);
        return EXIT_SUCCESS;
    } catch(std::ifstream::failure e) {
        ERR_("Exception opening/reading individuals from file " << filename_);
        return EXIT_FAILURE;
    }
}
*/
//int32_t
//PileupFile::gen_window_pool(std::vector<Window>& pool, const uint64_t window_size) {
//    try {
//        std::string line;
//        uint64_t filesize, fpos = 0, scounter = 1, locus = 0;
//
//        // get length of file:
//        in.seekg(0, in.end);
//        filesize = in.tellg();
//
//        in.seekg(0, in.beg);
//        LOG_("Working with input file ( " << filesize/(1024*1024) << " MB ) and window size of " << window_size << " sites");
//        // Set file start/end and site start/end to 0
//        // ngh::Window window(0, 0, 0, 0, 0);
//        ngh::Window window;
//        // File start = 0
//        window.file_start = 0;
//        window.file_end = 0;
//        bool parse = true;
//
//        // Start
//        getline(in, line);
//        window.site_start = get_locus(&line[0]);
//        window.file_start = 0;
//        window.file_end = in.tellg();
//        window.nsites  = scounter;
//
//        LOG_("Start Window [" << pool.size() << "] "
//                            << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
//                            << window.file_start << ":" << window.file_end << "-" << window.size());
//
//        while (parse) {
//            fpos = in.tellg();
//            locus = get_locus(&line[0]);
//            if (fpos < filesize) {
//                if (scounter < window_size) {
//                    PILEUP("COUNTER" << line);
//
//                    getline(in, line);
//                    PILEUP("COUNTER" << line);
//                    ++scounter;
//                    locus = get_locus(&line[0]);
//                    fpos = in.tellg();
//                } else {
//                    PILEUP("END" << line);
//                    window.nsites  = scounter;
//                    window.site_end = locus;
//                    window.file_end = fpos;
//                    window.nsites  = scounter;
//
//                    LOG_("Window [" << pool.size() << "] "
//                                        << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
//                                        << window.file_start << ":" << window.file_end << "-" << window.size());
//                    // pool.push_back(window);
//
//                    window.file_start = ++fpos;
//                    getline(in, line);
//                    window.site_start = get_locus(&line[0]);
//                    fpos = in.tellg();
//                }
//            } else {
//                window.nsites  = scounter;
//                window.site_end = locus;
//                window.file_end = filesize;
//                parse = false;
//            }
//        }
//        PILEUP("Pool size " << pool.size());
//
//        // Go to begin of the file
//        in.seekg(0, in.beg);
//        return EXIT_SUCCESS;
//    } catch(std::ifstream::failure e) {
//        ERR_("Exception opening/reading individuals from file " << filename_);
//        return EXIT_FAILURE;
//    }
//}
//
//int32_t
//PileupFile::gen_window_pool(std::vector<Window>& pool, const uint64_t window_size) {
//    try {
//        std::string line;
//        uint64_t filesize, scounter = 1, locus = 0;
//
//        // get length of file:
//        in.seekg(0, in.end);
//        filesize = in.tellg();
//        in.seekg(0, in.beg);
//        DEBUG_("Working with input file ( " << filesize/(1024*1024) << " MB ) and window size of " << window_size << " sites");
//        // Set file start/end and site start/end to 0
//        // ngh::Window window(0, 0, 0, 0, 0);
//        ngh::Window window;
//        // File start = 0
//        window.file_start = 0;
//        window.file_end = 0;
//
//
//        // Read input file
//        while (window.file_end < filesize && getline(in, line)) {
//        	scounter = 1;
//        	// Get window from line
//            // locus = get_locus(&line[0]);
//            window.site_start = get_locus(&line[0]);
//            while (window.file_end < filesize && getline(in, line)) {
//
//                if (scounter < window_size) {
//                    ++scounter;
//                    locus = get_locus(&line[0]);
//                } else {
//                    window.nsites  = scounter;
//                    window.site_end = locus;
//                    break;
//                }
//                window.file_end = in.tellg();
//            }
//            // window.file_end = in.tellg();
//            LOG_("Window [" << pool.size() << "] "
//                    << window.site_start << ":" << window.site_end << "-" << window.sites() << ":" << scounter <<" , "
//                    << window.file_start << ":" << window.file_end << "-" << window.size());
//
//            pool.push_back(window);
//            PILEUP("Pool size " << pool.size());
//            // Update next window. Next to EOL
//            window.file_start = window.file_end + 1;
//        }
//
//        // Go to begin of the file
//        in.seekg(0, in.beg);
//        return EXIT_SUCCESS;
//    } catch(std::ifstream::failure e) {
//        ERR_("Exception opening/reading individuals from file " << filename_);
//        return EXIT_FAILURE;
//    }
//}

// TODO(jnavarro): Check errors
void
PileupFile::read_buffer(const Chunk& chunk) {
    // Find file position
    in.seekg(chunk.start);

    // read data as a block:
    // buffer.resize(chunk.size());
    uint64_t size = chunk.end - chunk.start;

    // Add null terminator
    buffer_.resize(size, '\0');
    in.read(&buffer_[0], size);
}

void
PileupFile::read_buffer(const Chunk& chunk, std::string& data) {
    // Find file position
    in.seekg(chunk.start);
    // read data as a block:
    uint64_t size = chunk.end - chunk.start;
    data.resize(size, '\0');
    in.read(&data[0], size);
}

void
PileupFile::free_buffer() {
    buffer_.clear();
}


} /* namespace Reader */
} /* namespace ngh */
