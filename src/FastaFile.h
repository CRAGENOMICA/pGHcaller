/*
 * FastaFile.h
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

#ifndef FASTAFILE_H_
#define FASTAFILE_H_

#include "CCommon.h"
#include "Data.h"
#include "Logger.h"
// #include "SNPcall.h"
// #include "RoestiCaller.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <iterator>
#include <string>

namespace ngh {

#define DEBUG_FASTA 0

#if DEBUG_FASTA
//#define PILEUP(str) do { std::cout << "[PILEUP : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#define FASTA(str) do { std::cout << "[FASTA : "<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define FASTA(str) do { } while ( false )
#endif

class FastaFile : public ngh::CCommon {
public:

	FastaFile(std::string);
	virtual ~FastaFile();

	unsigned long getNumBases();
	void initialize();
	void readBuffer(const Chunk& chunk, std::string& data);
	std::string getName();
	std::string getFilename();
	//std::filebuf* getFileBuf();
	std::streambuf* getFileBuf();


private:
	std::string name;
	unsigned long seq_pos_start;
	unsigned long seq_pos_end;
	long int num_bases;
	bool isInitialized;
	std::string filename;

	// File and buffers
	std::ifstream in;
	ngh::Logger *logger;

	int open();
	int close();
	void AnalyzeFile();
};




} /* namespace ngh */

#endif /* FASTAFILE_H_ */
