/*
 * MasterWorker.h
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

#ifndef SRC_MASTERWORKER_H_
#define SRC_MASTERWORKER_H_

#include <mpi.h>
#include <queue>
#include "Logger.h"
#include "Data.h"
#include "CCommon.h"
#include "COptions.h"


#define DEBUG_TASKS 0
#if DEBUG_TASKS
#define TASKS(str) do { std::cout << "[TASKS: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while ( false )
#else
#define TASKS(str) do { } while ( false )
#endif

// TODO(jnavarro) Remove this! Handle mpi errors in MasterWorker class
// and implement a exit function
#define MPI_ERR_END(err) do { MPI_Finalize(); exit(err); } while (false)

namespace ngh {

class WorkerStats {
 public:
    enum class METRIC {
        READ = 0,
                PROCESS,
                WRITE,
                WAIT,
                MIN_READ,
                MIN_PROCESS,
                MIN_WRITE,
                MIN_WAIT,
                MAX_READ,
                MAX_PROCESS,
                MAX_WRITE,
                MAX_WAIT };

    enum class TIME {
        INIT = 0,
        END,
        ALL };

    unsigned long sites,tasks;
    double metric[8];
    clock_t time[2];
    WorkerStats() : sites(0), tasks(0) {}
};


enum class STATUS {
    HAS_ELEMENTS,
    EMPTY };
enum class MSG : int  {
    TASKID = 100,
            SNPCALLS,
            FILENAME,
            REQUEST,
            REGISTERED,
            GET_DATA,
            SEND_STATS,
            DATA,
            DATA_SIZE,
            ROLES,
            NO_MORE_TASKS };


template <class T>
class MasterWorker : public ngh::CCommon {
 public:

    /*
	typedef enum { TASKID=100, SNPCALLS, FILENAME, REQUEST, CHUNK, ROLES } tag_t;
	typedef enum { HAS_ELEMENTS, EOP } POOL_Status;
	typedef enum { MASTER=0, WRITER=1, WORKER=2 } role_t;
	typedef enum { NO_MORE_TASKS=0, TASKS_IN_POOL } task_t;
     */

    MasterWorker();
    MasterWorker(int, char**);
    virtual ~MasterWorker();

    void setOptions(ngh::COptions* options);
    void registerWorkers();
    void assignTasks(const std::vector <T>& TaskPool);
    // void assignTasks(std::vector <T>* TaskPool, unsigned int number);
    void endWorkers();
    STATUS getData(T& data);
    STATUS getData(std::vector <T>& data);
    void setDataType();
    MSG sendRequest();
    //void init(int argc, char**, int thread_level=MPI_THREAD_FUNNELED);
    //void init_thread(int argc, char* argv[]);
    int QueryMPIThread();
    void initialize();
    void finalize();
    void barrier();
    ngh::ROLE getRole();
    unsigned int  getWorkers() {
        return workers;
    }
    int getRank();
    // int e_init, s_init;
    // int e_regw, s_regw;

 private:
    void init_mpi(int, char**, int);

    ngh::COptions* options_;
    MPI_Datatype MPI_DATA;
    int rank, size, workers, provided, required;
    unsigned int number_of_chunks;
    int MPI_ERROR;
    MPI_Status status;
    // ROLE role;
    T data;
    WorkerStats workerStats;
    uint32_t tasks_by_worker_;
};


} /* namespace ngh */

#endif  // SRC_MASTERWORKER_H_
