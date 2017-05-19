/*
 * MFastaFile.h
 *
 *  Created on: May 24, 2014
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

#ifndef MFASTAFILE_H_
#define MFASTAFILE_H_
/*

#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
*/
//#include <cstring>
//#include <cstdio> // SEEK_END
/*
#include <string>
#include <locale>


*/
//#include <iostream>
//#include <fstream>
//#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

// Shared memory
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

//#include "common.h"
#include "SNPcall.h"
#include "Logger.h"
#include "Name.h"
#include "FastaFile.h"


//#define DEFAULT_OUTPUT_FILE "default_output_file.fas"
//#define FASTA_NAME_SIZE 255
// TODO Change FASTA NAME SIZE to 255
#define FASTA_NAME_SIZE 25
#define NUMBER_HAPLOTYPES 2


#define DEBUG_WRITER 0
#if DEBUG_WRITER
#define WRITER_MSG(str) do { std::cout << "[WRITER: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define WRITER_MSG(str) do { } while ( false )
#endif


namespace ngh {

//typedef enum {IUPAC, HAPLOTYPE} eNomenclature;

class MFastaFile {
	std::vector <std::string> _names;
	//Name* names;
    std::string _oFilename;
    //fasta_bed regions;
    //concatenate_fastas;
    //TODO: Set number of haplotypes here. we use 2 by default
    unsigned int number_haplotypes;
    unsigned int _individuals, _wLen;
    unsigned int _fastaSequences;
    unsigned long _initSite;
    unsigned long _endSite;	// Max. site position in FASTA or Reference/outgroup file
    int _fd;                // file descriptor
    //void* _mOutputFile; 	// Memory mapped file handler. Generic pointer type
    char* _mOutputFile; 	// Memory mapped file handler. Generic pointer type
    unsigned long _mFilesize; // Store filesize of memory mapped file
    //bool _rooted; // Is there a outgroup filename to append?


private:

    enum ploidy {ONE=0, TWO=1 };
	ngh::Logger *logger;

    void setDefaults( void );
    void initMemoryMappedOutfile( void );
    void finalizeMemoryMappedOutfile( void );
    inline void write_snpcall(const SNPcall& snpcall);
    inline unsigned long getPosition(const unsigned int sequenceNumber, const unsigned long sitePosition);
    //inline void build_haplotype(const std::vector<SNPcall>& snpcalls);
    void setNames( Name* names );

public:
	MFastaFile(
			std::string oFile,
			unsigned long individuals,
			ngh::Name* names
			// unsigned long init,
		    // unsigned long end/*,
			// eNomenclature nomenclature = HAPLOTYPE*/
			);
	virtual ~MFastaFile();
    void generateEmptyFile( void );
   	void initialize( void );	// Open filename and generate empty FASTA
	void finalize( void );		// Append outgroup/reference sequence and close opened files
	void build(const std::vector<SNPcall> & snpcalls);
	void build(const SNPcall& snp);
	void build_original(const std::vector<SNPcall>& snpcalls);
	void appendFasta( std::string fastafilename );
	void appendFasta( ngh::FastaFile* fasta );
	void setNumBases(unsigned long end);
};

} /* namespace ngh */

#endif /* MFASTAFILE_H_ */
