/*
 * FastaFile.cpp
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

#include "FastaFile.h"

namespace ngh {



FastaFile::FastaFile(std::string filename) {
	logger = ngh::Logger::getInstance();

	this->filename  = filename;
	isInitialized = false;
	open();
}

FastaFile::~FastaFile() {
	close();
}


void FastaFile::initialize() {
	if( ! isInitialized )
	{
		AnalyzeFile();
	}
	isInitialized = true;
}

std::string FastaFile::getFilename(){
	if( ! isInitialized )
	{
		initialize();
	}
	return ( filename );
}

std::string FastaFile::getName(){
	if( ! isInitialized )
	{
		initialize();
	}
	return ( name );
}

unsigned long FastaFile::getNumBases(){
	if( ! isInitialized )
	{
		initialize();
	}
	return ( num_bases );
}
/*
std::filebuf* FastaFile::getFileBuf() {
	if( ! isInitialized )
	{
		initialize();
	}
	return ( in.rdbuf() );
}
*/
std::streambuf* FastaFile::getFileBuf() {
	if( ! isInitialized )
	{
		initialize();
	}
	in.seekg (0, in.beg);
	return ( in.rdbuf() );
}

void FastaFile::AnalyzeFile(){
    //in.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
	in.exceptions ( std::ifstream::badbit );
	num_bases=0;
	char ch;
	// TODO: many other cases to work here. sequence splitted in two o more lines, etc.

	try {
		in.seekg (0, in.beg);

		bool keepLooping = true;
		while( keepLooping ) {
			in.get(ch);
			switch(ch) {
			case '>': case '\t': case ' ': case ';':
				num_bases=0;
				while( in.get(ch) && ch != '\n')
				{
					name.push_back(ch);
				}
				FASTA("Name " << name);
			break;
			default:
				//if(ch == '\n') 	keepLooping = false;
				if ( in.eof() ) 	keepLooping = false; // Stop ath the end
				else if(ch == '\n') continue;			 // Ignore \n
				else 				num_bases++;		 // Count these
			}
		}
		FASTA("Num bases " << num_bases);
		FASTA("Fasta filename " << filename << " Head " << name << " Num bases " << num_bases);
	}
	catch (std::ifstream::failure e) {
		ERR_("Reading FASTA outgroup/reference - Exception opening/reading input file ");
		exit(EXIT_FAILURE);
	}
}

int FastaFile::open(){
	in.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
	try {
		in.open(filename, std::ios::binary);
		in.seekg (0, in.beg);
		return EXIT_SUCCESS;
	}
	catch (std::ifstream::failure e) {
		//ERR_("Exception opening/reading input file ");
		return EXIT_FAILURE;
	}
}

int FastaFile::close(){
    in.close();
    isInitialized = false;
    return EXIT_FAILURE;
}


void
FastaFile::readBuffer(const Chunk& chunk, std::string& data){
	// Find file position
	in.seekg(chunk.start);
	// read data as a block:
	unsigned long int size = chunk.end - chunk.start;
	data.resize(size, '\0');
	in.read (&data[0], size);
}



} /* namespace ngh */
