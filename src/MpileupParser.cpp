/*
 * MpileupParser.cpp
 *
 *  Created on: Feb 20, 2015
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


#include "MpileupParser.h"

namespace ngh {

MpileupParser::~MpileupParser() {
}

MpileupParser::MpileupParser(unsigned long num, FilterOptions* filters) {
    logger = ngh::Logger::getInstance();
    number_of_individuals = num;
    this->filters = filters;
}

void MpileupParser::parse_buffer_to_reads(std::string* buffer, const Window& data, std::vector <ngh::Site>* sites) {
    std::locale loc;
    const char tab[] = "\t";
    char *token, *save_ptr, *raw_depth_ptr;
    char ref;

    ngh::Window window(data.sites(), data.site_start, data.site_end, 0, data.file_end - data.file_start);
    uint64_t last_site = window.site_start -1;
    std::string line, last_line;


    std::stringstream str(buffer->substr(window.file_start, window.file_end));
    std::vector<ngh::Site>::iterator s = sites->begin();
    while ( std::getline(str, line) ) {
        //   chr1 2 C 0 * * 0 * * 0 * * 1 ^]. = 0 * *
        // strok is not thread safe
        token = strtok_r(&line[0], tab, &save_ptr);     // token <- chr1
        token = strtok_r(NULL, tab, &save_ptr);         // Site
        (*s).site = atoi(token);

        // Check new site number, must be last_site + 1
        if((*s).site != last_site + 1) {
            // save THIS site number
            uint64_t saved_site =  (*s).site;
            PARSER2_("This site number is " << saved_site << " but previous site number was " << last_site << ".");
            PARSER2_("We will fill the missing data with 'N' lines.  Take this in account in your analyses.");
            for (uint32_t i=(last_site+1); i < saved_site; i++ ) {
                // WAR_("Set site " << i);
                (*s).site = i;
                // WAR_("Reads size " << (*new_s).reads.size());
                for (uint32_t ind = 0; ind < number_of_individuals; ind++) {
                    // (*new_s)[ind].resize(1);
                    (*s)[ind] = "N";
                    // (*new_s).reads[i] = "N";
                    // WAR_("Reads size " << (*s)[ind].size());
                }
                ++s;
            }
            PARSER_( saved_site - last_site - 1 << " line(s) added.");

            // Restore last_site and site number
            last_site = saved_site - 1;
            (*s).site = saved_site;
        }
        last_site =  (*s).site;
	    // uint64_t LOGS = 3176;
        token = strtok_r(NULL, tab, &save_ptr);         // Ref
        ref = tolower(*token);
        PARSER_("Process individuals site " << (*s).site);
        // if((*s).site == LOGS) PARSER_("Process individuals site " << (*s).site);
        // Repeat this process for all individual's column
        for (uint32_t ind = 0; ind < number_of_individuals; ind++) {
            // Depth read from token and save pointer
            raw_depth_ptr = strtok_r(NULL, tab, &save_ptr);
            uint32_t raw_depth = atoi(raw_depth_ptr);
            PARSER_("Site " << (*s).site << ", ind " << ind << ", raw_depth " << raw_depth << ", mindeps " << filters[ind].mindeps);

            // Set minimum quality for this individual
            int32_t min_qual = filters[ind].baseqs + filters[ind].platforms;
            // Parse if number of reads > 0 and > mindeps
            if (raw_depth >= filters[ind].mindeps) {
                // Reserve enough memory to prevent resize
                (*s)[ind].reserve(raw_depth);

                // Get current reads/quals from file. Next element
		        PARSER2_("Tokens reads and quals");
                std::string reads = strtok_r(NULL, tab, &save_ptr);
                std::string quals = strtok_r(NULL, tab, &save_ptr);
                PARSER2_("reads " << reads);
                PARSER2_("quals " << quals);
		        // if((*s).site == LOGS) PARSER_("reads " << reads << " quals " << quals);

                // Final depth after filters
                uint32_t depth = 0;

                // Index to read string
                // uint32_t idx = 0;
                /*
                if((*s).site == 32366 || (*s).site == 32367 ) {
                LOG_("Site " << (*s).site << " ind " << ind << " total reads " <<  reads.size());
                LOG_("      BaseQ << " << filters[ind].baseqs);
                LOG_("      Platform " << filters[ind].platforms);
                LOG_("      MinQual " << min_qual);
                }
                */
                for (uint32_t j=0, idx = 0; j < reads.size(); j++) {
                    // Read starts
                    // PILEUP_("  Read " << reads.at(j));
                    /*
                    if((*s).site == 32366 || (*s).site == 32367 ) {
                    LOG_("Site " << (*s).site << " ind " << ind );
                    LOG_("      Read " << reads.at(j));
                    LOG_("      Qual " << (int) quals[idx] );
                    LOG_("      MinQual " << min_qual);
                    }
                    */
                    switch (reads.at(j)) {
                    case '.':
                        if (quals[idx] >= min_qual) {
                            depth++;
                            (*s)[ind].push_back(toupper(ref));
                        }
                        idx++;
                        break;


                    case ',':
                        if (quals[idx] >= min_qual) {
                            depth++;
                            (*s)[ind].push_back(tolower(ref));
                        }
                        idx++;
                        break;

                    case 'A': case 'a':
                    case 'C': case 'c':
                    case 'G': case 'g':
                    case 'T': case 't':
                    case 'N': case 'n':
                        if (quals[idx] >= min_qual) {
                            depth++;
                             (*s)[ind].push_back(reads.at(j));
                        }
                        // Move index to next
                        idx++;
                        break;


                    case '+':
                    case '-':
                        // get number of insetions/deletions
                        uint32_t t, bases;
                        t = 0;
                        j++;
                        while (isdigit(reads.at(j+t), loc)
                                && (j+t < reads.size())) {
                            t++;
                        }
                        bases = atoi(reads.substr(j, t).c_str());
                        // Bypass all the characters
                        j = j+bases+t-1;
                        break;

                    case '^':
                        // Means beginning of read, next symbol is
                        // mapping quality in ascii -> remove both
                        j++;
                        break;

                    case '*':
                        //  Skip '*'
                        idx++;
                        break;
                    }
                 }
                // Check depth
                if (depth < filters[ind].mindeps || depth > filters[ind].maxdeps) {
                    (*s)[ind] = "N";
                }
            } else if (raw_depth > 0 ) {
                // e.g. 2 .. HH - Bypass these items
                PARSER2_("Removing next two tokens ..");
                strtok_r(NULL, tab, &save_ptr);
                strtok_r(NULL, tab, &save_ptr);
            } else {
                // 0 * * - If the beginning of reads equals zero, bypass (* *) items
                // it=it+2;
                PARSER_("raw_depth is 0");
                // Save this position and check if next token is "*" or anything else.
                // If result is * then the format is "0 <tab>*<tab>* NEXT_READ", otherwise the
                // format is "0 <tab><tab> NEXT_READ" and we have to recover the old pointer
                if(save_ptr[0] == '\t' && save_ptr[1] == '\t') {
                    PARSER2_(" '0\t\t' .. We must continue");
                    continue;
                } else if (save_ptr[0] == '\t' && save_ptr[1] == '*') {
                    PARSER2_(" '0\t*\t*' .. We must read token");
                    strtok_r(NULL, tab, &save_ptr);
                } else {
                    strtok_r(NULL, tab, &save_ptr);
                    strtok_r(NULL, tab, &save_ptr);
                }
                /*
		        else {
			    ERR_("Error parsing input file " << line);
   		        exit(EXIT_FAILURE);
		        }
                */
            }
            /*
	        // Force exit
	        if((*s).site == 3178) {
	            WAR_("EXIT here");
            	exit(EXIT_FAILURE);
	        }
            if((*s).site == 32366 || (*s).site == 32367 ) {
                LOG_("Site " << (*s).site << " ind " << ind << " reads " <<  (*s)[ind].size());
            }
            */
        }
        // Move iterator to next site
        ++s;
        /*
        if (s == sites->end()) {
            ERR_("Something wrong happened when parsing buffer at site " << (*s).site);
            ERR_("Window " << window.site_start << ":" << window.site_end << "-" << window.sites()
                               <<" , " << window.file_start << ":" << window.file_end << "-" << window.size());
            ERR_("Sites vector size is " <<  sites->size() );
            ERR_("Last line " <<  last_line );
            exit(EXIT_FAILURE);
        }
        */
    }
}
} /* namespace ngh */
