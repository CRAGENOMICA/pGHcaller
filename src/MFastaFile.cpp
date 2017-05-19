/*
 * MFastaFile.cpp
 *
 *  Created on: May 24, 2014
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

#include "MFastaFile.h"

namespace ngh {

MFastaFile::MFastaFile(
		std::string oFile,
	    unsigned long individuals,
	    ngh::Name* names/*,
		eNomenclature nomenclature*/
		) {
			logger = ngh::Logger::getInstance();

			setDefaults();
			_individuals = individuals;
			_fastaSequences = _individuals * number_haplotypes;
			_oFilename = oFile;

			//setNomenclature( nomenclature );
			setNames(names);
}

MFastaFile::~MFastaFile() {
	// TODO Auto-generated destructor stub
}

void MFastaFile::setNumBases( unsigned long num_bases ) {
	    _initSite = 1; // Always 1
		// Check endSite size
	    if(num_bases == 0 ) {
	    	ERR_("Size of output FASTA file cannot be 0");
	    	exit(EXIT_FAILURE);
	    } else
	    	if( num_bases <= _initSite ) {
	    		ERR_("End site position cannot be greater than initial position");
	    		exit(EXIT_FAILURE);
	    	} else
	    	{
	    		_endSite = num_bases;
	    	}
}

void MFastaFile::setNames(Name* names){
	for (unsigned int i = 0; i < _individuals; i++)
    {
			std::string aname = ">" + names[i].name;
            for (long long int j=0; j<number_haplotypes; j++)
            {
            	_names.push_back( aname + "_" + std::to_string(j) );
            }
    }
}

void MFastaFile::setDefaults( void ) {
	//_rooted = false;
	_wLen = 0;
	_endSite = 0;
	_individuals = 0;
	//_nomenclature = HAPLOTYPE;
	number_haplotypes = NUMBER_HAPLOTYPES;
	_fastaSequences = _individuals * number_haplotypes;
	//setDefaultNames();
}

void MFastaFile::initialize( void ) {
    initMemoryMappedOutfile();
}

/* Size of NAME_SEQ is fixed and define (FASTA_NAME_SIZE). We need to calculate offsets when we go to write
 * to the file.
 *
 * Format:
 *
 * 	NAME SEQ \n SEQUENCE \n
 * 	NAME SEQ \n SEQUENCE \n
 * 	...
 * 	NAME SEQ \n SEQUENCE \n
 */
void MFastaFile::generateEmptyFile(void) {

    // TOCpas	O: Check error opening/closing file
    std::ofstream outfile(_oFilename, std::ios::out | std::ofstream::binary);

    try {
    	outfile.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
    	std::vector<char> n_vector(_endSite, 'N');
    	LOG_("Initializing " << _fastaSequences << " fasta sequences with size " << n_vector.size());
    	n_vector.push_back('\n');

    	// TODO PROGRESS BAR HERE??
    	for(unsigned int i = 0; i < _fastaSequences; i++)
    	{
    		// Name
    		//std::vector<char> empty(FASTA_NAME_SIZE, ' ');
    		std::vector<char> empty(_names[i].begin(), _names[i].end());
    		// fill with spaces
    		//empty.resize(FASTA_NAME_SIZE-_names[i].size(), '*');
    		empty.resize(FASTA_NAME_SIZE, ' ');
    		empty.push_back('\n');
	 		DEBUG_("Fasta Name vector[" << i << "] - " << _names[i] ); //'" << empty << "'");
    		if (!outfile.write( &empty[0], empty.size() ))
    		{
    			ERR_("problem writing to file");
    			exit(EXIT_FAILURE);
    		}
	 		// Sequence
    		if (!outfile.write( &n_vector[0], n_vector.size() ))
    		{
    			ERR_("problem writing to file");
    			exit(EXIT_FAILURE);
    		}

	 		/*
    		// Sequence
    		empty.insert(empty.end(), n_vector.begin(), n_vector.end());;
    		empty.push_back('\n');
			#if DEBUG
    		LOG_("Fasta vector[" << i << "] size " << empty.size() );
			#endif

    		if (!outfile.write( &empty[0], empty.size() ))
    		{
    			ERR_("problem writing to file");
    			exit(EXIT_FAILURE);
    		}
    		*/
    	}
    	outfile.close();
	}
	catch (std::ofstream::failure& e) {
		ERR_("Exception open/write generating empty file " << outfile);
	}
}

void MFastaFile::initMemoryMappedOutfile( void ) {
	// Calc filesize: number of sequences * (FASTA_NAME_SIZE + \n + SEQUENCE SIZE (endSite) + \n)
	//_mFilesize = _fastaSequences * ( FASTA_NAME_SIZE + sizeof('\n') + _endSite + sizeof('\n') );
	_mFilesize = _fastaSequences * ( FASTA_NAME_SIZE + _endSite + sizeof('\n') );


	DEBUG_("Init file Size " << _mFilesize << " End " << _endSite << " SEQ " << _fastaSequences );

	// Create the memory mapping.
	_fd = open (_oFilename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if( ! _fd ) {
		ERR_("Error openning file " << _oFilename);
        exit(EXIT_FAILURE);
    }
	_mOutputFile = (char*)mmap (0, _mFilesize, PROT_WRITE, MAP_SHARED, _fd, 0);
    if (_mOutputFile == MAP_FAILED) {
        close(_fd);
        ERR_("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
}

void MFastaFile::finalizeMemoryMappedOutfile( void ) {
    // Release the memory (unnecessary because the program exits)
    if(  -1 == munmap (_mOutputFile, _mFilesize) ) {
    	ERR_("Error releasing memory mapped file! ");
    }
    close (_fd);
}


void MFastaFile::finalize( void ) {
	finalizeMemoryMappedOutfile();
}

/*
 * This is critical. We are writing to mmap using non consecutive position of memory. This is
 * inefficient and could generate some blocking issues at the file system. We should try to
 * write using large blocks and check if we are reducing contention.
 *
*
void MFastaFile::build_original(const std::vector<ngh::SNPcall>& snpcalls) {

	// Using indices
	//BUILD_NGH_DEBUG_MSG("Build SNP calls with size #" << snpcalls.size());
	for( std::vector<ngh::SNPcall>::const_iterator it = snpcalls.begin(); it != snpcalls.end(); ++it) {
		//BUILD_NGH_DEBUG_MSG("Build SNP call  with site #" << (*it).getSite());
		//BUILD_NGH_DEBUG_MSG("Called Genotypes #" << (*it).getCalledGenotypes().size() << ":" << (*it).getCalledGenotypes());
		write_snpcall( *it );
	}
}
*/

void MFastaFile::build(const ngh::SNPcall& snp) {
	char c;
	unsigned long pos;
	// Get position to write this individual in their correct sequence number
	unsigned long site = snp.getSite();
	for( unsigned int individual=0; individual < _individuals; individual++ )
	{
		/*
		pos = getPosition(2*individual+ONE, site);
		c=snp.getCalledGenotype(individual, ONE);
		_mOutputFile[pos] = std::toupper(c);
		pos = getPosition(2*individual+TWO, site);
		c=snp.getCalledGenotype(individual, TWO);
		_mOutputFile[pos] = std::toupper(c);
		*/
		pos = getPosition(2*individual+ONE, site);
		if ( 'n' != ( c=snp.getCalledGenotype(individual, ONE) ) )
		{
			_mOutputFile[pos] = std::toupper(c);
		}
		pos = getPosition(2*individual+TWO, site);
		if ( 'n' != ( c=snp.getCalledGenotype(individual, TWO) ) )
		{
			_mOutputFile[pos] = std::toupper(c);
		}
	}
}


void MFastaFile::build(const std::vector<ngh::SNPcall> & snpcalls) {
	char c;
	unsigned long pos;

	for( unsigned int individual=0; individual < _individuals; individual++ )
	{
		WRITER_MSG("Individual " << individual << " calls size " <<  snpcalls.size());
		for(unsigned int i=0;  i < snpcalls.size(); i++)
		{
			// Get position to write this individual in their correct sequence number
			WRITER_MSG("Build site " << snpcalls[i].getSite() << " " << snpcalls[i].getCalledGenotypes() );

			unsigned long site = snpcalls[i].getSite();
			//for(unsigned int j=0; j < number_haplotypes; j++)
			//{
/*
			pos = getPosition(2*individual+ONE, site);
			c=snpcalls[i].getCalledGenotype(individual, ONE);
     		_mOutputFile[pos] = std::toupper(c);
	     	pos = getPosition(2*individual+TWO, site);
		    c=snpcalls[i].getCalledGenotype(individual, TWO);
		    _mOutputFile[pos] = std::toupper(c);
*/

				pos = getPosition(2*individual+ONE, site);
				if ( 'n' != ( c=snpcalls[i].getCalledGenotype(individual, ONE) ) )
				{
								_mOutputFile[pos] = std::toupper(c);
				}
				pos = getPosition(2*individual+TWO, site);
				if ( 'n' != ( c=snpcalls[i].getCalledGenotype(individual, TWO) ) )
				{
								_mOutputFile[pos] = std::toupper(c);
				}

		//}
		}
	}
}

/*
void MFastaFile::build(const std::vector<ngh::SNPcall> & snpcalls) {
	for( unsigned int individual=0; individual < _individuals; individual++ )
	{
		for(unsigned int i=0;  i < snpcalls.size(); i++)
		{
			char c;

			// Get position to write this individual in their correct sequence number
			unsigned long site = snpcalls[i].getSite();
			for(unsigned int j=0; j < number_haplotypes; j++)
			{
				unsigned long pos = getPosition(2*individual+j, site);
				if ( 'n' != ( c=snpcalls[i].getCalledGenotype(individual, j) ) )
				{
								_mOutputFile[pos] = std::toupper(c);
				}
			}
		}
	}
}
*/
/*
void MFastaFile::build(const std::vector<ngh::SNPcall> & snpcalls) {
	char c;
	unsigned int pos1, pos2;
	// Get position to write this individual in their correct sequence number
	for(unsigned int i=0;  i < snpcalls.size(); i++)
	{
		unsigned long site = snpcalls[i].getSite();

		for( unsigned int individual=0; individual < _individuals; individual++ )
		{

			// Get position to write this individual in their correct sequence number
			pos1 = getPosition(2*individual, site);
			pos2 = getPosition(2*individual+1, site);
			//DEBUG_("Site" << site << " POS1 " << pos1);
			if ( 'n' != ( c=snpcalls[i].getCalledGenotype(individual, 0) ) ) {
				_mOutputFile[pos1] = std::toupper(c);
			}
			if ( 'n' != ( c=snpcalls[i].getCalledGenotype(individual, 1) ) ) {
				_mOutputFile[pos2] = std::toupper(c);
			}
		}
	}
}
*/

/*
 * Maybe we should create a index and access to position directly. Anyway, this code is not
 * the most important in order to speed up this app
 */
inline unsigned long MFastaFile::getPosition(const unsigned int sequenceNumber, const unsigned long sitePosition) {
	// Position = BASE + Offset
	unsigned long base = (FASTA_NAME_SIZE + sizeof('\n') + _endSite + sizeof('\n')) * sequenceNumber;
	unsigned long offset = FASTA_NAME_SIZE + sitePosition;
	return (base + offset);
}

/**
 * Append fasta file pointed by filename to mfasta file
 */
void MFastaFile::appendFasta( std::string fastafilename ) {
	std::ofstream of_a;
	std::ifstream if_b;
	of_a.exceptions ( std::ofstream::failbit | std::ofstream::badbit );
	if_b.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	try {
		LOG_("Appending file " << fastafilename << " to fasta file " <<  _oFilename);
		WAR_("This is a simple APPEND operation. We are doing this operations AS IS without checking init/end sites, or name");
		//, ... in " << fastafilename );
		of_a.open(_oFilename, std::ios_base::binary | std::ios_base::app);
		if_b.open(fastafilename, std::ios_base::binary);
		// TODO: Seek initSite position before append reference
		of_a.seekp(0, std::ios_base::end);
		of_a << if_b.rdbuf();
		of_a.close();
		if_b.close();
	}
	catch (std::ifstream::failure e) {
		ERR_("Appending outgroup/reference: Exception opening/reading or writing input/output file");
		exit(EXIT_FAILURE);
	}
}


void MFastaFile::appendFasta( ngh::FastaFile *fasta ) {
	std::ofstream of_a;
	//std::filebuf* inbuf = fasta->getFileBuf();
	std::streambuf* inbuf = fasta->getFileBuf();
	of_a.exceptions ( std::ofstream::failbit | std::ofstream::badbit );

	try {
		LOG_("Appending file " << fasta->getName() << " to fasta file " <<  _oFilename);
		of_a.open(_oFilename, std::ios_base::binary | std::ios_base::app);
		of_a.seekp(0, std::ios_base::end);
		char c;// = inbuf->sbumpc();
	    //LOG_(c);
	    LOG_(inbuf);
		while (c != EOF)
		{
			switch(c) {
			case '>': case '\t': case ' ': case ';':
				do {
					c=inbuf->sbumpc();
				} while(c != '\n');
			break;
			default:
				if(c == '\n') continue;	 // Ignore \n
				else 		   of_a.put(c);
			}
		    c = inbuf->sbumpc();
		    LOG_(c);
		}
		of_a.close();
	}
	catch (std::ifstream::failure e) {
		ERR_("Appending outgroup/reference: Exception opening/reading or writing input/output file");
		exit(EXIT_FAILURE);
	}
}

} /* namespace ngh */
