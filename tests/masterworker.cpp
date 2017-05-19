/*
 * test_master-worker.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jnavarro
 */
#include <queue>
#include <mpi.h>
//#include <utility>      // std::pair
// How to include this in configure.ac?
#define OPENMP 1
#if OPENMP
#include <omp.h>
#endif
#include "Logger.h"
#include "MasterWorker.h"
#include "common.h"
#include "CmdArguments.h"
#include "PileupFile.h"
#include "MFastaFile.h"
#include "Site.h"
#include "SNPcall.h"
#include "common.h"
#include "RoestiCaller.h"
#include "Logger.h"


using namespace std;




int
main(int argc, char* argv[])
{
	ngh::Logger *logger;
	logger = ngh::Logger::getInstance();
	//logger->warn("Warning message");

	ngh::MasterWorker<int> TaskManager;
	TaskManager.init(argc, argv);

	ngh::ROLE role = TaskManager.getRole();

	switch( role ) {
	case ngh::ROLE::MASTER:
	{
		logger->info("Starting MASTER");
		logger->info("Create pool of INTs");
		std::queue <int> DataPool;
		for( unsigned int i=0; i < 1; i++) DataPool.push(i);

		// Register workers
		logger->info("Registering workers ");
		TaskManager.registerWorkers();

		// Start server
		logger->info("Sending tasks to workers ");
		TaskManager.assignTasks(DataPool);

		// End workers
		logger->info("Sending END message ");
		TaskManager.endWorkers();
	}
	break;

	case ngh::ROLE::WORKER:
	{
		int data;
		ngh::MSG request;
		ngh::STATUS status;

		bool done = false;
		while( !done )
		{
			request = TaskManager.sendRequest();

			cout << "SWITCH " << std::to_string( static_cast<long long int>(request) ) << "\n";
			switch( request ){
			case ngh::MSG::REGISTERED:
			{
				logger->info("Registered from master. It sounds good for me!");
				// Do something like init stats, etc
				break;
			}
			case ngh::MSG::GET_DATA:
			{
				//ngh::MasterWorker<int>::STATUS::HAS_ELEMENTS == TaskManager.getData(data)
				logger->info("Master is going to send data");
				status = TaskManager.getData(data);
				logger->info("Data received");
				break;
			}
			case ngh::MSG::SEND_STATS:
			{
				logger->info("Master requests stats");
				// Send stats to master
				break;
			}

			case ngh::MSG::NO_MORE_TASKS:
			{
				logger->info("No more tasks to do. Exiting!");
				done = true;
				break;
			}

			default:
			{
				logger->error("Error! This msg is not defined");
				break;
			}
			}
		}
	}
	break;
	}

	TaskManager.barrier();
	TaskManager.finalize();

    return (0);
}

