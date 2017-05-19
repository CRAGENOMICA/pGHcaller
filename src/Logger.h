/*
 * Logger.h
 *
 *  Created on: Nov 24, 2014
 *  Author: javier.navarro@caos.uab.es
 *  Copyright (c) 2015 Javier Navarro, GNU license.
 *
 *  This file is part of PGHcaller.
 *
 *  Original work from:
 *  GHcaller
 *  Copyright (c) 2013 Bruno Nevado, GNU license.
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

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_
#include <iostream>
#include <string>

namespace ngh {

// enum class DEBUG WARN INFO ERROR
enum typelog { DEBUG, INFO, WARN, ERROR };



class Logger {
private:
    // priority_;
    static Logger *single;
    bool headers = true;
    bool opened = false;
    typelog level = INFO;
    typelog msglevel;
    inline std::string getLabel(typelog type) {
        std::string label;
        switch(type) {
        case DEBUG: label = "DEBUG"; break;
        case INFO:  label = "INFO"; break;
        case WARN:  label = "WARN"; break;
        case ERROR: label = "ERROR"; break;
        }
        return label;
    }


public:
    Logger() {
    }
    Logger(typelog type) {
        msglevel = type;
        if(headers) {
            operator << ("["+getLabel(type)+"] - <GHcaller> ");
        }
    }

    ~Logger() {
        if(opened) {
            std::cout << std::endl;
        }
        opened = false;
    }

    static Logger* getInstance(void) {
        // http://mail-archives.apache.org/mod_mbox/logging-log4cxx-dev/200901.mbox/%3C4975157F.8070001@purplescarab.com%3E
        if (single == NULL) {
            single = new Logger();
        }
        return single;
    }

    static void resetInstance(void) {
        if(single != NULL) {
            delete single;
            single = NULL;
        }
    }

    void enableDebug(void) {
        msglevel = DEBUG;
    }

    template<class T>
    Logger &operator<<(const T &msg) {
        if(msglevel >= level) {
            std::cout << msg;
            opened = true;
        }
        return *this;
    }
};

#define GET_INSTANCE ngh::Logger::getInstance()
#define CODE_LOCATION __FILE__<< ":" << __PRETTY_FUNCTION__ << ":" << __LINE__<< " - "
#define ERR_(str) do { ngh::Logger(ngh::typelog::ERROR) << str;  } while(false)
#define ERR_MASTER_(str) do {  } while (false)
#define WAR_MASTER_(str) do {  } while (false)
#define LOG_MASTER_(str) do {  } while (false)
#define WAR_(str) do { ngh::Logger(ngh::typelog::WARN) << str; } while (false)
//#define LOG_(str) do { GET_INSTANCE (INFO) << str; } while (false)
#define LOG_(str) do { ngh::Logger(ngh::typelog::INFO) << str; } while (false)

#define DEBUG_A
#ifdef DEBUG_A
#define DEBUG_(str) do { ngh::Logger(ngh::typelog::DEBUG) << str; } while( false )
#else
#define DEBUG_(str) do {} while ( false )
#endif

/*
std::ostream& operator<<(std::ostream& os, const Logger& logger) {
	os <<  log4cpp::eol;
	 //os <<  logger->getPriority() << log4cpp::eol;
     return os;
}
 */


#define DISABLED 0
//#define MASTER 0
#define ENABLED 1
//#define CGENO 1

// #define DEBUG DEBUG
#define LOG 1
// #define DEBUG 1
// #define DEBUG_EXTRA
#define DEBUG_BUILD_NGH 1
// #define DEBUG_MPI_FARM
// #define DEBUG_TASKS

// #define DEBUG_WORKER
// #define DEBUG_MEM
#define DEBUG_TIME
#define DEBUG_TIME2

/*
#define ERR_(str) do { std::cerr << "[ERROR] - <GHcaller> " << str << std::endl; } while( false )
#define ERR_MASTER_(str) do { if (rank == 0) std::cerr << "[ERROR] - <GHcaller> " << str << std::endl; } while( false )
#define WAR_(str) do { std::cout << "[WARNING] - <GHcaller> " << str << std::endl; } while( false )

#ifdef LOG
//#define log_(str) do { std::cerr << "[LOG] - <GHcaller> " << str; } while( false )
#define log_(str) do { std::cerr << "[LOG] - <GHcaller> " << str << std::endl; } while( false )
// PREVIOUS
#define LOG_(str) do { std::cerr << "[LOG] - <GHcaller> " << str << std::endl; } while( false )
//#define LOG_(str) do { logger->info(str); } while( false )
//#define log_master_(str) do { if (rank == 0) std::cerr << "[LOG] - <GHcaller> " << str << std::endl; } while( false )
//#define LOG_MASTER_(str) do { if (rank == 0) std::cerr << "[LOG] - <GHcaller> " << str << std::endl; } while( false )
#else
#define log_(str) do { } while ( false )
#define LOG_(str) do { } while ( false )
//#define log_master_(str) do { } while ( false )
//#define LOG_MASTER_(str) do { } while ( false )
#endif
*/

#if DEBUG_MSG
#define DEBUG_MSG(str) do { std::cout << "[DEBUG_MSG : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#if DEBUG_EXTRA
#define DDEBUG_MSG(str) do { std::cout << "[DDEBUG_MSG : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define DDEBUG_MSG(str) do { } while ( false )
#endif


#if DEBUG_BUILD_NGH
#define BUILD_NGH_DEBUG_MSG(str) do { std::cout << "[BUILD_NGH_DEBUG_MSG : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define BUILD_NGH_DEBUG_MSG(str) do { } while ( false )
#endif

#if CGENO
#define CGENO_MSG(str) do { std::cout << "[CALC_GENOTYPES : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define CGENO_MSG(str) do { } while ( false )
#endif


#if DEBUG_MPI_FARM
#define MPI_FARM_MSG(str) do { std::cout << "[MPI_FARM : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define MPI_FARM_MSG(str) do { } while ( false )
#endif

#if DEBUG_TASKS
#define TASKS_MSG(str) do { std::cout << "[TASKS: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define TASKS_MSG(str) do { } while ( false )
#endif



#ifdef DEBUG_WORKER
#define WORKER_MSG(str) do { std::cout << "[WORKER: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define WORKER_MSG(str) do { } while ( false )
#endif

#ifdef DEBUG_TIME
#define TIME_MSG(str) do { std::cout << "[TIME_MSG] - " << str << std::endl; } while( false )
#else
#define TIME_MSG(str) do { } while ( false )
#endif

#ifdef DEBUG_TIME2
#define TIME_MSG2(str) do { std::cout << "[TIME2_MSG] - " << str << std::endl; } while( false )
#else
#define TIME_MSG2(str) do { } while ( false )
#endif


#ifdef DEBUG_MEM
#define MEM_MSG(str) do { std::cout << "[MEM_MSG] - " << str << std::endl; } while( false )
#else
#define MEM_MSG(str) do { } while ( false )
#endif



} /* namespace ngh */

#endif  // SRC_LOGGER_H_




