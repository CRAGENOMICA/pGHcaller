/*
 * MasterWorker.cpp
 *
 *  Created on: Nov 28, 2014
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

#include "MasterWorker.h"

namespace ngh {
template <class T>
MasterWorker<T>::MasterWorker() {
    tasks_by_worker_  = 1;
    initialize();
}

template <class T>
MasterWorker<T>::MasterWorker(int argc, char* argv[]) {
    tasks_by_worker_  = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    workers = size -1;
    // Number of chunks to send
    number_of_chunks = 1;
    // Init Datatype defined
    setDataType();
}


template <class T>
MasterWorker<T>::~MasterWorker() {
    // Finalize Task manager
finalize();
}

template <class T>
void
MasterWorker<T>::setDataType() {
    MPI_Type_contiguous(sizeof(data), MPI_BYTE, &MPI_DATA);
    MPI_Type_commit(&MPI_DATA);
}

template <class T>
void
MasterWorker<T>::setOptions(ngh::COptions* options) {
    options_ = options;
}

template <class T>
int
MasterWorker<T>::QueryMPIThread() {
    int claimed;
    MPI_Query_thread(&claimed);
    if (claimed != provided) {
        WAR_("Query thread gave thread level "<< claimed
                << " but Init_thread gave "<< provided);
    } else {
        switch (provided) {
        case MPI_THREAD_FUNNELED:
            LOG_("MPI thread level provided is  MPI_THREAD_SINGLE");
            break;
        case MPI_THREAD_SINGLE:
            LOG_("MPI thread level provided is MPI_THREAD_FUNNELED");
            break;
        default:
            WAR_("MPI thread level provided code is " << provided);
        }
    }
    return provided;
}


template <class T>
void
MasterWorker<T>::init_mpi(int argc, char* argv[], int thread_level) {
    // Start MPI
    // TODO(jnavarro): Check MPI_ERROR values
    provided = MPI_THREAD_FUNNELED;
    // provided = MPI_THREAD_MULTIPLE;
    MPI_Init_thread(&argc, &argv, thread_level, &provided);

    // TODO(jnavarro) check if required != provided
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Now, number of workers is size -1
    workers = size -1;
    // Number of chunks to send
    number_of_chunks = 1;
    // Init Datatype defined
    setDataType();
}

/*
 *  --enable-event-thread-support --enable-opal-multi-threads
 *  --enable-orte-progress-threads --enable-mpi-thread-multiple
 *
 *  http://manpages.ubuntu.com/manpages/precise/man3/MPI_Init_thread.3.html
 *     MPI_THREAD_MULTIPLE Support
       MPI_THREAD_MULTIPLE support is included if Open MPI was configured with
       the --enable-thread-multiple  configure  switch.   You  can  check  the
       output  of  ompi_info(1)  to  see  if  Open MPI has MPI_THREAD_MULTIPLE
       support:

       shell$ ompi_info | grep -i thread
                 Thread support: posix (mpi: yes, progress: no)
       shell$

       The "mpi: yes" portion of the above output indicates that Open MPI  was
       compiled with MPI_THREAD_MULTIPLE support.

       Note  that  MPI_THREAD_MULTIPLE  support  is  only  lightly tested.  It
       likely does not work for thread-intensive applications.  Also note that
       only  the  MPI  point-to-point  communication  functions  for the BTL's
       listed below are  considered  thread  safe.   Other  support  functions
       (e.g.,   MPI   attributes)   have  not  been  certified  as  safe  when
       simultaneously used by multiple threads.

           tcp
           sm
           mx
           elan
           self

       Note that Open MPI's thread support is in a  fairly  early  stage;  the
       above  devices  are  likely  to  work,  but the latency is likely to be
       fairly  high.   Specifically,  efforts  so  far  have  concentrated  on
       correctness, not performance (yet).
 */

template <class T>
void MasterWorker<T>::initialize() {
 /*
#if defined(MPI_VERSION) && (MPI_VERSION >= 2)
    // LOG_("FUNNELED");
    init_mpi(options_->get_argc(), options_->get_argv(), MPI_THREAD_FUNNELED);

#else
    ERR_("MPI THREAD SINGLE detected!");
    init_mpi(options_->get_argc(), options_->get_argv(), MPI_THREAD_SINGLE);
#endif
*/

}


template <class T>
void MasterWorker<T>::finalize() {
    // Wait all and end taskmanager
    barrier();
    MPI_Finalize();
}

template <class T>
void
MasterWorker<T>::barrier() {
    MPI_Barrier(MPI_COMM_WORLD);
}

template <class T>
ngh::ROLE
MasterWorker<T>::getRole() {
    if (0 == rank) {
        return ngh::ROLE::MASTER;
    } else {
        return ngh::ROLE::WORKER;
    }
}

template <class T>
int
MasterWorker<T>::getRank() {
    return rank;
}

template <class T>
void
MasterWorker<T>::registerWorkers() {
    int request, tag, who, msg;
    // TODO(jnavarro) check MPI errors
    for (int i=0; i < workers; i++) {
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE,
                tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD, &status);
        who = status.MPI_SOURCE;
        MPI_Send(&msg, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::REGISTERED), MPI_COMM_WORLD);
    }
}


template <class T>
void
MasterWorker<T>::endWorkers() {
    int tag, who, msg;

    // TODO(jnavarro) check MPI errors
    for (int i=0; i < workers; i++) {
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE,
                tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD, &status);
        who = status.MPI_SOURCE;
        MPI_Send(&msg, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::NO_MORE_TASKS), MPI_COMM_WORLD);
    }
    LOG_("All tasks assigned to workers");
}

/*
template <class T>
void
MasterWorker<T>::assignTasks(std::vector <T>* Pool) {
    int tag, who, msg;
    //T data;
    //setDataType(data);
    // Progress info
    unsigned long int size = Pool.size();
    unsigned int number = 1; // default number of elements to send here
    unsigned long int progress = 0;
    unsigned long int point = static_cast<unsigned long int>(10*100/size);


    // Send data until Pool is empty
    while (!Pool.empty()) {
        // Get next chunk and assign data to deal with MPI send without creating new DATA types
        data = Pool.front();
        Pool.pop_back();

        // Manage request and send GET_DATA orders to workers
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE,
                tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD, &status);
        who = status.MPI_SOURCE;

        // Log progress status only from worker 1
        if ((who == 1) && (progress % point == 0)) {
            LOG_("Tasks assigned " << (static_cast<int>(progress*100/size)) << "%");
        }
        MPI_Send(&msg, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::GET_DATA), MPI_COMM_WORLD);

        // Send task to worker
        MPI_Send(&number, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::DATA_SIZE), MPI_COMM_WORLD);
        MPI_Send(&data, 1, MPI_DATA, who,
                tag = static_cast<int>(MSG::DATA), MPI_COMM_WORLD);

        // Refresh progress status
        progress++;
    }
}
*/

template <class T>
ngh::STATUS
MasterWorker<T>::getData(T& data) {
    int tag;
    unsigned int number = 0;

    // MPI_Send(&task, 1, MPI_INT, 0, tag=static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD);
    MPI_Recv(&number, 1, MPI_INT, 0,
            tag = static_cast<int>(MSG::DATA_SIZE), MPI_COMM_WORLD, &status);
    MPI_Recv(&data, number, MPI_DATA, 0,
            tag = static_cast<int>(MSG::DATA), MPI_COMM_WORLD, &status);

    // TODO Send Status
    return  STATUS::EMPTY;
}

template <class T>
ngh::STATUS
MasterWorker<T>::getData(std::vector <T>& data ) {
    int tag;
    unsigned int number = 0;

    //MPI_Send(&task, 1, MPI_INT, 0, tag=static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD);
    MPI_Recv(&number, 1, MPI_INT, 0, tag=static_cast<int>(MSG::DATA_SIZE), MPI_COMM_WORLD, &status);
    TASKS("Number DATA_SIZE received " << number );
    TASKS("Expected vector size " << number );
    data.resize(number);
    // MPI_Recv(&data[0], number, MPI_DATA, 0, tag=static_cast<int>(MSG::DATA), MPI_COMM_WORLD, &status);
    MPI_Recv(&data[0], number, MPI_DATA, 0, tag=static_cast<int>(MSG::DATA), MPI_COMM_WORLD, &status);
    TASKS("Vector size is " << data.size());
    TASKS("Get Window [0] - "
            <<  data[0].site_start << ":" << data[0].site_end << "-" << get_sites(data[0])
            <<" , " << data[0].file_start << ":" << data[0].file_end << "-" << get_size(data[0]));

    // TODO Send Status
    return  STATUS::EMPTY;
}

/*
template <class T>
void
MasterWorker<T>::assignTasks(std::vector <T>* Pool, unsigned int number) {
    int tag, who, msg;
    // T data;
    // setDataType(data);
    // Progress info
    unsigned long int size = Pool.size();
    unsigned long int index = 0;
    unsigned long int progress = 0;
    unsigned long int point = static_cast<unsigned long int>(10*100/size);

    // Send data until Pool is empty
    TASKS(" Pool size " << Pool.size() );
    while (size) {
        // check if there are enough number of data elements t
        if ( number > size ) {
            number = size;
        }
        // Get next chunk and assign data to deal with MPI send without creating new DATA types
        // data = Pool.front();

        // Manage request and send GET_DATA orders to workers
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE,
                tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD, &status);
        who = status.MPI_SOURCE;

        // Log progress status only from worker 1
        if ((who == 1) && (progress % point == 0)) {
            LOG_("Tasks assigned " << (static_cast<int>(progress*100/size)) << "%");
        }
        MPI_Send(&msg, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::GET_DATA), MPI_COMM_WORLD);

        // Send number tasks to worker
        TASKS(" DATA_SIZE to send " << number);
        MPI_Send(&number, 1, MPI_INT, who,
                tag = static_cast<int>(MSG::DATA_SIZE), MPI_COMM_WORLD);
        // MPI_Send(&data, number, MPI_DATA, who, tag=static_cast<int>(MSG::DATA), MPI_COMM_WORLD);
        MPI_Send(&Pool[index], number, MPI_DATA, who,
                tag = static_cast<int>(MSG::DATA), MPI_COMM_WORLD);

        // Refresh index, size
        TASKS(" Pool size " << size << " index " << index);
        size = size-number;
        index = index+number;

        // Refresh progress status
        progress++;
    }
}
*/

template <class T>
void
MasterWorker<T>::assignTasks(const std::vector <T>& pool) {
    int tag, who, msg;

    uint64_t size = pool.size();
    uint64_t index = 0;


    // Send data until Pool is empty
    TASKS(" Pool size " << pool.size() );
    while ( size) {
        // check if there are enough number of data elements t
        if ( tasks_by_worker_ > size ) {
            tasks_by_worker_ = size;
        }

        // Manage request and send GET_DATA orders to workers
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE,
                tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD, &status);
        who = status.MPI_SOURCE;

        // Log progress status only from worker
        if(pool.size() > 10) {
            if( (index%((int)pool.size()/10) ) == 0) {
                LOG_("Tasks assigned " << index+1 << "/" << pool.size());
            }
        } else {
            LOG_("Tasks assigned " << index+1 << "/" << pool.size());
        }
        MPI_Send(&msg, 1, MPI_INT, who, tag = static_cast<int>(MSG::GET_DATA), MPI_COMM_WORLD);

        // Send number of tasks to worker
        TASKS(" DATA_SIZE to send " << tasks_by_worker_);
        MPI_Send(&tasks_by_worker_, 1, MPI_INT, who, tag = static_cast<int>(MSG::DATA_SIZE), MPI_COMM_WORLD);
        // MPI_Send(&data, number, MPI_DATA, who, tag=static_cast<int>(MSG::DATA), MPI_COMM_WORLD);
        // MPI_Send(&pool[index], tasks_by_worker_, MPI_DATA, who, tag = static_cast<int>(MSG::DATA), MPI_COMM_WORLD);
        MPI_Send(&pool[index], tasks_by_worker_, MPI_DATA, who, tag = static_cast<int>(MSG::DATA), MPI_COMM_WORLD);
        // LOG_("Send Window [" << index << "] - " <<  pool[index].site_start << ":" << pool[index].site_end );
        TASKS("Send Window [" << index << "] - " <<  pool[index].site_start << ":" << pool[index].site_end << "-" << get_sites(pool[index])
                <<" , " << pool[index].file_start << ":" << pool[index].file_end << "-" << get_size(pool[index]));

        // Refresh index, size
        TASKS(" Pool size " << size << " index " << index);
        // LOG_(" Pool size " << size << " index " << index);
        size = size - tasks_by_worker_;
        index = index + tasks_by_worker_;
    }
}

template <class T>
ngh::MSG
MasterWorker<T>::sendRequest() {
    int tag, request, msg;
    MPI_Send(&request, 1, MPI_INT, 0,
            tag = static_cast<int>(MSG::REQUEST), MPI_COMM_WORLD);
    MPI_Recv(&request, 1, MPI_INT, 0 ,
            MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    msg = status.MPI_TAG;
    // WARNING!! This is not defined but must work
    return  (ngh::MSG) msg;
}

template class MasterWorker<Window>;
// template class MasterWorker<int>;
// template class MasterWorker<Chunk>;
}  /* namespace ngh */
