/*
 * test_master-worker.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: jnavarro
 */
#include <queue>
#include <pthread.h>
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


//void *master(const ngh::MasterWorker<int>& TaskManager) {
void *master(void*) {
	 cout << "task1 says\n";
}

int
main(int argc, char* argv[])
{
	ngh::Logger *logger;
	logger = ngh::Logger::getInstance();
	//logger->warn("Warning message");
	pthread_t server_thread;

	cout << "Create taskmanager\n";
	ngh::MasterWorker<int> TaskManager;
	cout << "Init taskmanager\n";
	TaskManager.init_thread(argc, argv);

	// Start master thread in mpi MASTER process
	ngh::ROLE role = TaskManager.getRole();
	if ( ngh::ROLE::MASTER == role ) {
		cout << "CREATE MASTER Thread\n";
		pthread_create(&server_thread, NULL, &master, NULL);
/*
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
		*/
	}

	/* Code for workers*/
	int data;
	ngh::MSG request;
	ngh::STATUS status;
/*
	bool done = false;
	while( !done )
	{
		request = TaskManager.sendRequest();

		cout << "SWITCH " << std::to_string( static_cast<int>(request) ) << "\n";
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

	TaskManager.barrier();
 	 */

	// Wait until master thread finish
	void* result;
	pthread_join(server_thread, &result);

	// End taskmanager
	TaskManager.finalize();

    return (0);
}

