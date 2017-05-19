//  common.h
//  NGHcaller
//
//  Copyright (c) 2013 Bruno Nevado, GNU license.

#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <sys/resource.h>
//#include "Logger.h"


#define DISABLED 0
//#define MASTER 0
#define ENABLED 1
//#define CGENO 1

/*
#define NO_MORE_TASKS 0
#define FIRST_TASK 0
#define TASKS_IN_POOL 10
*/

#ifndef __ngh_common__
#define __ngh_common__


//#define DEBUG DEBUG
#define LOG 1
//#define DEBUG 1
//#define DEBUG_EXTRA
#define DEBUG_BUILD_NGH 1
//#define DEBUG_MPI_FARM
//#define DEBUG_TASKS

//#define DEBUG_WORKER
//#define DEBUG_MEM
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




//  DEBUG_MSG("Hello" << ' ' << "World!" << 1 );
//#define VERSION "<pGHcaller> v 0.0.2"

long getMemoryUsage(void);
long getMemoryLimit(void);

#define MSPLITNEW
#ifdef MSPLITNEW
void msplit2( const std::string& tosplit ,  const char& delim,  std::vector<std::string> * output );
#else
void msplit2( const std::string& tosplit , const char& delim,  std::vector<std::string> * output );
#endif

// char  toIUPAC2 ( char in1, char in2 ); // for haplo/genotype calls, returns e.g. c for CN, and C for CC
/*
double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks)/(CLOCKS_PER_SEC/1000);
    return diffms;
}
*/
// Clock for events
double diffclock( clock_t clock1, clock_t clock2 );
double Mtime( clock_t& time );


#endif
