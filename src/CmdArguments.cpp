/*
 * CmdArguments.cpp
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


#include "CmdArguments.h"

namespace ngh {

CmdArguments::CmdArguments() {
	// logger = ngh::Logger::getInstance();
	setDefaults();
}


void CmdArguments::setDefaults(void) {


	// SNP CALLING ALGORITHM OPTIONS
	min_reads   = DEFAULT_MINREADS;     //!< genotype / haplotype call threshold
	p_err      = DEFAULT_P_ERR;  		//!< raw sequencing error
	chivalue   = DEFAULT_CHIVALUE; 		//!< 1 d.f., 0.05 (for LRT)
	wlen = DEFAULT_WINDOW_LEN;  //!< window len
	/*
	outgroupfile = '';     // will append sequence to outfile (and fill ends of seqs with Ns)
	outputfile = '';
	inputfile = '',
	bedfile = '';
	allnames = '';
	*/
	verbose = false;
	concatenate_fastas = false;      //!< when using bed file only
   	chunksize = DEFAULT_CHUNK_SIZE;
   	enable_help_ = false;
   	haplocalls = true;
}

// Create new class to store all these params & Names
/*
void CmdArguments::setDefaultsMPILEUP(void){
	mindep   = "3";
	maxdep   = "100";
	platform = "33";
	baseq    = "20";
}
*/

ARGUMENTS_STATUS CmdArguments::check(void )
{
	// TODO: Include all argument checks here!
	ARGUMENTS_STATUS status = ARGUMENTS_STATUS::SUCCESS;
	// std::cout << "Role " << (int) role << std::endl;

	// CHECK ARGS
	if( outputfile.empty() )
	{
		ERR_MASTER_("Output file unspecified (-output)");
        return(ARGUMENTS_STATUS::ARGUMENT_ERROR);
	}

	if( inputfile.empty() )
	{
	    ERR_MASTER_("Input file unspecified (-input)");
		return(ARGUMENTS_STATUS::ARGUMENT_ERROR);
	}

	/*
	if( numind == 0 ){
		err_("Set num of individuals (-numind)");
	}
	*/

	if( wlen != 0 &&  !bedfile.empty() )
	{
	    ERR_MASTER_("(fasta output): -winlen and -bed options are mutually exclusive");
        return(ARGUMENTS_STATUS::ARGUMENT_ERROR);
	}
	if( concatenate_fastas == true && bedfile.empty()  )
	{
	    ERR_MASTER_("(fasta output): -concatenate option only valid with bed file");
		return(ARGUMENTS_STATUS::ARGUMENT_ERROR);
	}

	if( chunksize <= 0 )
	{
	    ERR_MASTER_("Cannot set chunksize to " << chunksize);
		return(ARGUMENTS_STATUS::ARGUMENT_ERROR);
	} else
		if( chunksize < 1024*1024 )
		{
			WAR_MASTER_("Chunsize of " << chunksize << " is too low. Consider increase the size to prevent performance issues.");
			status = ARGUMENTS_STATUS::ARGUMENT_WARNING;
		}
	return(status);
}

void CmdArguments::parse(int argc, char* argv[])
{
	  int index;
	  int iarg=0;

	  const struct option long_options[] =
	  	{
	  		{"names",              required_argument, 0, 'N'},
	  		{"mindepth",          required_argument, 0, 'm'},
	  	    {"maxdepth",          required_argument, 0, 'M'},
	  		{"platform",           required_argument, 0, 'p'},
	  		{"baseq",              required_argument, 0, 'b'},
	  		{"filterfile",         required_argument, 0, 'f'},

	  		{"output",             required_argument, 0, 'o'},
	  		{"input",              required_argument, 0, 'i'},
	  		{"outgroup",           required_argument, 0, 'O'},
	  		//{"reference",          required_argument, 0, 'R'},
	  		{"individuals",        required_argument, 0, 'n'},

	  		{"version",            no_argument,  0, 'v'},
	  	    {"help",               no_argument,  0, 'h'},
	  	    //{"iupac",              no_argument,  0, 'I'},
	  		{"verbose",            no_argument,  0, 'V'},
	  		{"concatenate",        no_argument,  0, 'a'},
	  		{"only_genotypes",     no_argument,  0, '6'},
	  		{"chunksize",          required_argument, 0, 's'},

	  		// Algorithm options string to be parsed and checked by
	  		// algorithm class
	        {"algorithm",          required_argument, 0, '0'},
	        {"minreads",           required_argument, 0, '1'},
	        {"error",              required_argument, 0, '2'},
	        {"chivalue",           required_argument, 0, '3'},
	        // Custom read and write options strings must be parsed by
	        // read and write classes
	        {"read_options",       required_argument, 0, '4'},
	        {"write_options",      required_argument, 0, '5'},
	        {0, 0, 0, 0}
	  	};

	  // extern char *optarg;
	  // extern int optind, opterr, optopt;
	  //turn off getopt error message
	  opterr=1;

	  // Parse all arguments
	  // TODO Add more cmd options.
	  // TODO check errors
	  // To store temporal parameters
	  int i;
	  float f;
	  unsigned long ul;
	  std::string str;

	  while(iarg != -1)
	  {
	    iarg = getopt_long(argc, argv, "vhIVCHm:M:O:o:i:n:r:e:c:s:", long_options, &index);
	    switch (iarg)
	    {
	      case 'h':
            // DEBUG_("Calling help function ");
	    	// help();
	        DEBUG_("Enable help function ");
	        enable_help_ = true;
	    	// exit(EXIT_SUCCESS);
	        break;
	      case 'v':
	    	DEBUG_("verbose enabled ");
	    	verbose = true;
	        break;
	      case 's':
  	  	  	ul = std::atol(optarg);
  	  	  	if(ul == ERANGE || ul == EINVAL)
  	  	  	{
  	  	  		WAR_("Error parsing parameter 'chunsize'. Using default " << DEFAULT_CHUNK_SIZE);
  	  	  	} else {
  	  	  		chunksize = ul*1024*1024;
  	  	  		DEBUG_("Set chunksize " << chunksize );
  	  	  	}
  	  	  	break;
	      case 'i':
	      	inputfile = optarg;
	      	DEBUG_("Input filename: " << inputfile );
	      	break;
	      case 'o':
	    	outputfile = optarg;
	    	DEBUG_("Output filename: " << outputfile );
	      	break;
	      case 'O':
	      case 'R':
	     	outgroupfile = optarg;
	    	DEBUG_("Outgroup/reference filename: " << outgroupfile );
	      	break;
	      	/*
	      case 'I':
	    	  iupac = true;
	    	  DDEBUG_MSG("IUPAC enabled");
	    	  break;
	       */
	      case 'a':
	    	  concatenate_fastas = true;
	    	  DEBUG_("Append FASTA enabled" );
	    	  break;
	      case 'N':
	    	  names = optarg;
	    	  DEBUG_("Set names " << names );
	    	  break;
	      case 'm':
	    	  mindep = optarg;
	    	  DEBUG_("Set min_depth " << mindep );
	    	  break;
	      case 'M':
	    	  maxdep = optarg;
	    	  DEBUG_("Set max_depth " << maxdep );
	    	  break;
	      case 'p':
	    	  platform = optarg;
	    	  DEBUG_("Set platform " << platform );
	    	  break;
	      case 'b':
	    	  baseq = optarg;
	    	  DEBUG_("Set baseq " << baseq );
	    	  break;
	      case 'f':
	    	  filterfile = optarg;
	    	  DEBUG_("Set filter file name  " << filterfile );
	    	  break;
	    /*
	      case 'm':
	    	  i = std::atoi(optarg);
	    	  if(i == ERANGE || i == EINVAL)
	    	  {
	    		  WAR_("Error parsing 'min_depth' parameter. Discarted.");
	    	  } else {
	    		  mindep = i;
	    		  DEBUG_MSG("Set min_depth " << mindep );
	    	  }
	    	  break;
	      case 'M':
	    	  i = std::atoi(optarg);
	    	  if(i == ERANGE || i == EINVAL)
	    	  {
	    		  WAR_("Error parsing 'max_depth' parameter. Discarted.");
	    	  } else {
	    		  maxdep = i;
	    		  DEBUG_MSG("Set max_depth " << maxdep );
	    	  }
	    	  break;
	      case 'p':
	    	  i = std::atoi(optarg);
	    	  if(i == ERANGE || i == EINVAL)
	    	  {
	    		  WAR_("Error parsing 'platform' parameter. Discarted.");
	    	  } else {
	    		  platform = i;
	    		  DEBUG_MSG("Set platform " << platform );
	    	  }
	    	  break;
	      case 'b':
	    	  i = std::atoi(optarg);
	    	  if(i == ERANGE || i == EINVAL)
	    	  {
	    		  WAR_("Error parsing 'baseq' parameter. Discarted.");
	    	  } else {
	    		  baseq = i;
	    		  DEBUG_MSG("Set max_depth " << baseq );
	    	  }
	    	  break;
	    	  */
	    	  // Algorithm
	      case '0':
	          algorithm_options = optarg;
	          DEBUG_("Set algorithm options  " << algorithm_options );
	          break;
	      case '1':
	          i = std::atoi(optarg);
	          if(i == ERANGE || i == EINVAL) {
	              WAR_("Error parsing 'min_reads' parameter. Using default " << DEFAULT_MINREADS);
	          } else {
	              min_reads = i;
	              DEBUG_("Set min_reads " << min_reads );
	          }
	          break;
	      case '2':
	          f = std::atof(optarg);
	          if(f == ERANGE || f == EINVAL) {
	              WAR_("Error parsing 'p_err' parameter. Using default " << DEFAULT_P_ERR);
	          } else {
	              p_err = f;
	              DEBUG_("Set p_err " << p_err );
	          }
	          break;
	      case '3':
	          f = std::atof(optarg);
	          if(f == ERANGE || f == EINVAL) {
	              WAR_("Error parsing 'chivalue' parameter. Using default " << DEFAULT_CHIVALUE);
	          } else {
	              chivalue = f;
	              DEBUG_("Set chivalue " << chivalue );
	          }
	          break;
	          // Read
	      case '4':
	          read_options = optarg;
	          DEBUG_("Set read options  " << read_options );
	          break;
	          // Write
	      case '5':
	          write_options = optarg;
	          DEBUG_("Set write options  " << write_options );
	          break;
	      case '6':
	          DEBUG_("Only genotypes enabled ");
	          haplocalls = false;
	          break;
	      default:
	          //ERR_("Cannot parse parameter "<< iarg << ". Exiting!!");
	          //exit(EXIT_FAILURE);
	          if(iarg != -1 ) {
	              WAR_("Error parsing options. One parameter discarted.");
	          }
	          break;
	    }
	  }
}

void CmdArguments::help(){
	// TODO: Change help function
	std::cout << std::endl << "* TODO: Change help text *"<< std::endl;
	std::cout << "Usage: "<< PACKAGE_NAME <<"  -i data.pileup  -o outfile.fasta [ filtering options ] [ SNP calling options ] [ Output options ]" << std::endl;
	std::cout << "Genotype-Haplotype SNP calling from MPILEUP data."<< std::endl;
	std::cout << "Naive Genotype/Haplotype SNP caller version " << PACKAGE_VERSION << std::endl;
	std::cout << "Authors: J. Navarro & S. Ramos-Onsins"<< std::endl;
	std::cout << "Contact: javier.navarro@uab.es"<< std::endl;
	std::cout << std::endl;
	std::cout << "Original version: bioinformatics.cragenomica.es/numgenomics"<< std::endl;
	std::cout << "Authors of original version: B. Nevado & S. Ramos-Onsins"<< std::endl;
    std::cout << "Contact of original version: bruno.nevado@cragenomica.es"<< std::endl;
    std::cout << std::endl;
    std::cout << "[ Filtering options ]" << std::endl;
    std::cout << "  -b, --baseq      minimum base quality of reads. Reads below threshold are discarded  (default is " << FilterOptions::DEFAULT_BASEQ << ")" << std::endl;
    std::cout << "  -m, --mindepth     minimum number of reads. Sites below threshold are coded missing (default is "<< FilterOptions::DEFAULT_MINDEP <<")"   << std::endl;
    std::cout << "  -M, --maxdepth     maximum number of reads. Sites above threshold are coded missing (default is "<< FilterOptions::DEFAULT_MAXDEP <<")"<< std::endl;
    std::cout << "  -p, --platform   offset to convert base qualities (default is "<< FilterOptions::DEFAULT_PLATFORM <<")" << std::endl;
    std::cout << "  -f, --filter     a filter file that contains all above information plus names (to implement)" << std::endl;
    std::cout << std::endl;
    std::cout << "Filtering options accept comma-separated list of values per individual, or single value for all individuals." << std::endl;
    std::cout << "platform values e.g. 33 is for illumina 1.8+ and Sanger, 64 for Illumina 1.3, ..." << std::endl;

    std::cout << std::endl;
    std::cout << "[ SNP calling options ]" << std::endl;
    std::cout << "  --minreads   minimum number of reads to attempt a genotype call (default is "<< DEFAULT_MINREADS <<")" << std::endl;
    // std::cout << "  --haplocalls to perform only genotype calls (default is enabled)" << std::endl;
    std::cout << "  --error            RAW sequencing error (default is "<< DEFAULT_P_ERR <<")" << std::endl;
    std::cout << "  --chivalue         chi-square threshold for LRT test of genotypes (default is "<< DEFAULT_CHIVALUE <<")" << std::endl;
    std::cout << "  ---only_genotypes  set to perform only genotype calls (default is "<< DEFAULT_HAPLOCALLS <<")" << std::endl;
        std::cout << std::endl;
    std::cout << "haplocalls enabled to perform both haplotype and genotype calls." << std::endl;
    std::cout << "chivalue value set to 3.841459, i.e. 0.05 with 1 d.f." << std::endl;

    std::cout << std::endl;
    std::cout << "[ Output options ]" << std::endl;
    std::cout << "  -v, --verbose   set verbose " << std::endl ;
    std::cout << "  -h, --help      this help text" << std::endl ;
    std::cout << "  -i, --input     mpileup data to process (required)" << std::endl;
    std::cout << "  -o, --output    file  set to zero (0) to perform only genotype callsto write to        (required)" << std::endl;
 // -f option to read input file with all filter options
    std::cout << "  -N, --names     comma-separated list of individuals' names, in same order as input." << std::endl;
    std::cout << "  -O, --outgroup  sequence file in fasta format to add to output alignment (def: none)." << std::endl;
    std::cout << "  -a, --append    append outgroup/reference sequence provided at the end of the output FASTA file" << std::endl;
    //    std::cout << "    -iupac: if 1, will output 1 sequence per individual using IUPAC codes, and lower-case bases denoting haplotype calls (def: 0, output 2 lines per individual)" << std::endl;
//    std::cout << "    -bed: output regions in bed file only (def: none)" << std::endl;
//    std::cout << "    -concatenate: set to 1 to concatenate all regions in input bed into a single output file (def: 0, output 1 file per region in bed file)" << std::endl;
//    std::cout << "    -wlen: output fasta sequences in windows of specified length, 1 window per file (def: none)" << std::endl;
    std::cout << std::endl;
    std::cout << "input mpileup data for a single chromosome, e.g. \"samtools mpileup -r chr1 infile1.bam infile2.bam > data.chr1.mpileup\"" << std::endl;
    std::cout << "output is a multi-fasta file" << std::endl;
    std::cout << "names default names are 'INDIVIDUAL_#NUMBER_#HAPLO_NUMBER'" << std::endl;
    std::cout << "append operation is disabled by default" << std::endl;
    std::cout << std::endl;
    std::cout << "[ Other options ]" << std::endl;
    std::cout << "  --algorithm      set  algorithm  options (not implemented) " << std::endl ;
    std::cout << "  --read_options   set  read  options (not implemented)" << std::endl ;
    std::cout << "  --write_options  set  write options (not implemented)" << std::endl ;
    std::cout << std::endl;

}

CmdArguments::~CmdArguments() {
	// TODO Auto-generated destructor stub
}

} /* namespace ngh */
