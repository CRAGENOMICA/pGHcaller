/*
 * CmdArguments.h
 *
 *  Created on: Mar 21, 2014
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


#ifndef CMDARGUMENTS_H_
#define CMDARGUMENTS_H_

#include <iostream>
#include <string>
#include <cstring>
#include <getopt.h>
#include <errno.h>
// #include "config.h"
//#include "common.h"
#include "Logger.h"
#include "CFilterOptions.h"
#include "CCommon.h"





/**
 * Default values for parameters
 */

#define DEFAULT_MINREADS 	6
#define DEFAULT_P_ERR 		0.01f
#define DEFAULT_CHIVALUE 	3.841459f
#define DEFAULT_WINDOW_LEN 	0
#define DEFAULT_HAPLOCALLS  true

// aka size to read by workers (in MBytes)
#define DEFAULT_CHUNK_SIZE 	50*1024*1024

namespace ngh {

enum class ARGUMENTS_STATUS {
	ARGUMENT_ERROR=0, SUCCESS=1, ARGUMENT_WARNING
};

/*!
 * Parse all parameters from command line and provide 'check methods
 *
 * @author Javier Navarro 		javier.navarro@caos.uab.es
 *
 */
class CmdArguments : public ngh::CCommon{
 private:
	/**
	 * Default values for parameters
	 * but ISO C++ forbids initialization of member

	static const int DEFAULT_MINREADS = 6;
	static const float DEFAULT_P_ERR = 0.01f;
	static const float DEFAULT_CHIVALUE = 3.841459f;
	static const int DEFAULT_WINDOW_LEN = 0;
	static const unsigned long DEFAULT_CHUNK_SIZE = 10*1024*1024;
*/
    bool enable_help_;          // Enables HELP flag
	int iupac;                  // if 1 uses iupac + upper/lower case bases
	int wlen;                   // window len
	std::string outgroupfile;   // will append sequence to outfile (and fill ends of seqs with Ns)
	std::string outputfile;
	std::string inputfile;
	std::string bedfile;		// Not implemented
	std::string filterfile;
	bool verbose;
	bool concatenate_fastas;    // when using bed file only
	unsigned int numind;		// Number of individuals
	unsigned long chunksize;    // aka size to read by workers (Default 100 MB)

	// SNP CALLING ALGORITHM OPTIONS
	std::string algorithm_options;
	unsigned int min_reads;
	float p_err;
	float chivalue;
	bool haplocalls;

	// Temporal. Needed to create
	std::string names;
	std::string mindep;
	std::string maxdep;
	std::string platform;
	std::string baseq;
	// ngh::Logger *logger;

	// Read/write options string
	std::string read_options;
	std::string write_options;

	void setDefaults(void);
	//void setDefaultsMPILEUP(void);


public:
	virtual ~CmdArguments();
	CmdArguments();

	void help ();
	void parse(int argc, char* argv[]);
	//int parse(int argc, char* argv[]);
	ARGUMENTS_STATUS check(void);

	const int& getIupac() const {
		return iupac;
	}
	const int& getWlen() const {
		return wlen;
	}
	const std::string& getOutputFile() const {
		return outputfile;
	}
	const std::string& getOutgroupFile() const {
		return outgroupfile;
	}
	const std::string& getInputFile() const {
		return inputfile;
	}
	const std::string& getBedFile() const {
		return bedfile;
	}
	const bool& doVerbose() const {
		return verbose;
	}
	/*
	const bool& doHaplocalls() const {
		return haplocalls;
	}
	*/
    bool printHelp(){
        return enable_help_;
    }

	bool doConcatenateFasta(){
		return concatenate_fastas;
	}
	const std::string& getNames() const {
		return names;
	}
	const std::string& getMindeps() const {
		return mindep;
	}
	const std::string& getMaxdeps() const {
		return maxdep;
	}
	const std::string& getPlatforms() const {
		return platform;
	}
	const std::string& getBaseqs() const {
		return baseq;
	}
	/*
	const std::vector<std::string> getVnames() const {
		std::vector <std::string> vnames;
		msplit2(allnames, ',', & vnames);
		return vnames;
	}
	*/
	const unsigned int& getNumind() const {
		return numind;
	}
	/*
	void setNumind(const unsigned int& n) {
		numind = n;
	}
	*/
	const unsigned int& getMinReads() const {
		return min_reads;
	}
	const float& getPErr() const {
		return p_err;
	}
	const float& getChi() const {
		return chivalue;
	}
	const bool& getHaplocalls() const {
	    return haplocalls;
	}
	const unsigned long& getChunksize() const {
		return chunksize;
	}
	const std::string& get_algorithm_options() const {
	    return algorithm_options;
	}
	const std::string& get_read_options() const {
	    return read_options;
	}
	const std::string& get_write_options() const {
	    return write_options;
	}

    friend std::ostream& operator << (std::ostream &os, const CmdArguments &o) {
	        os << "\t" << "outgroup "    << "\t" << o.outgroupfile << std::endl;
	        os << "\t" << "output file " << "\t" << o.outputfile << std::endl;
	        os << "\t" << "input file "  << "\t" << o.inputfile << std::endl;
	        os << "\t" << "BED file "    << "\t" << o.bedfile << std::endl;
	        os << "\t" << "filter file " << "\t" << o.filterfile << std::endl;
	        os << "\t" << "Conca FASTA " << "\t" <<  o.concatenate_fastas << std::endl;
	        os << "\t" << "Verbose "     << "\t" << o.verbose << std::endl;
	        // os << "\t" << "#individuals " << "\t" << o.numind << std::endl;
	        os << "\t" << "chunk size "   << "\t" << o.chunksize << std::endl;
	        // SNP CALLING ALGORITHM OPTIONS
	        os << "\t" << "algorithm options " << o.algorithm_options << std::endl;
	        os << "\t" << "Min Reads " << "\t" << o.min_reads << std::endl;
	        os << "\t" << "Prob error "<< "\t" << o.p_err << std::endl;
	        os << "\t" << "CHI value " << "\t" << o.chivalue << std::endl;
	        os << "\t" << "Haplocalls " << "\t" << o.haplocalls << std::endl;
	        // Temporal. Needed to create
	        os << "\t" << "Names "     << "\t" << o.names << std::endl;
	        os << "\t" << "Min depth " << "\t" << o.mindep << std::endl;
	        os << "\t" << "Max depth " << "\t" << o.maxdep << std::endl;
	        os << "\t" << "Platform "  << "\t" << o.platform << std::endl;
	        os << "\t" << "BaseQ "     << "\t" << o.baseq << std::endl;
	        // Read/write options string
	        os << "\t" << "read options "  << "\t" << o.write_options << std::endl;
	        os << "\t" << "write options " << "\t" << o.write_options << std::endl;

	        return os;
	    }

};

} /* namespace ngh */

#endif /* CMDARGUMENTS_H_ */
