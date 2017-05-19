/*
 *   PGHcaller
 *   Copyright 2015 <javier.navarro@caos.uab.es> GNU license.
 *
 *   Original work from:
 *   GHcaller
 *   Copyright (c) 2013 Bruno Nevado, GNU license.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CCommon.h"
#include "Logger.h"
#include "MasterWorker.h"
#include "COptions.h"
#include "Logger.h"
#include "CManager.h"
#include "Data.h"
#include "CAlgorithm.h"
#include "CFileWriter.h"
#include "CFileWriterMPIIO.h"

// TODO(jnavarro) Remove unused headers
// TODO(jnavarro) Exception control
// TODO(jnavarro) Debug level
// TODO(jnavarro) Debug CONST in config.h
#define DEBUG_OPTIONS 0

int main(int argc, char* argv[]) {
	ngh::Logger::getInstance();

	// Create and initialize task manager
	ngh::MasterWorker<ngh::Window> taskmanager(argc, argv);
	ngh::COptions options(argc, argv, taskmanager.getRole());

	// Check argument number
	if( ! options.check_arguments_number()) {
	    taskmanager.finalize();
	    ngh::Logger::resetInstance();
      exit (EXIT_SUCCESS);
	}
	// Parse all argument options and check
	if(ngh::OPTIONS_STATUS::OPTIONS_ERROR == options.parse_arguments()) {
	    taskmanager.finalize();
	    ngh::Logger::resetInstance();
	    exit (EXIT_SUCCESS);
	}

	// If help option was selected print help and exit.
    if(options.check_help()) {
            taskmanager.finalize();
            ngh::Logger::resetInstance();
            exit (EXIT_SUCCESS);
    }


	// Set options to task manager
	taskmanager.setOptions(&options);

	// TODO(jnavarro) At this moment algorithm uses RoestiCaller Class
	ngh::Algorithm::CAlgorithm algorithm(ngh::Algorithm::AlgorithmType::ROESTI, &options);

	// Create manager
	ngh::CManager<ngh::Window> manager(&options);
	// Set options and task manager
	manager.set_options(&options);
	manager.set_taskmanager(&taskmanager);

	// ... and declare and set input file and output files
	// TODO(jnavarro) FileWriter Factory
	// CFileReader* file_reader
	// file_reader = new ngh::CFileReader(ngh::ReaderType::PileupReader);
	// manager->set_input(&file_reader);
	ngh::FileWriter::CFileWriter *file_writer = new ngh::FileWriter::CFileWriterMPIIO();
	manager.set_output_writer(file_writer);

	// Set algorithm
	manager.set_algorithm(&algorithm);

	// Run Manager: master & Worker
	manager.run();

	// Grateful exit
	taskmanager.finalize();
    ngh::Logger::resetInstance();

    exit (EXIT_SUCCESS);
}

