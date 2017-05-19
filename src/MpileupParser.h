/*
 * MpileupParser.h
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

#ifndef MPILEUPPARSER_H_
#define MPILEUPPARSER_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <stdio.h>

#include "Data.h"
#include "Logger.h"
#include "CFilters.h"


#define DEBUG_PARSER 0
#if DEBUG_PARSER
//#define PILEUP(str) do { std::cout << "[PILEUP : " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#define PARSER_(str) do { std::cout << "[PILEUP : "<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define PARSER_(str) do { } while ( false )
#endif


#define DEBUG_PARSER2 0
#if DEBUG_PARSER2
#define PARSER2_(str) do { std::cout << "[PILEUP : "<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define PARSER2_(str) do { } while ( false )
#endif


namespace ngh {

class MpileupParser {
 public:
    MpileupParser(unsigned long num, FilterOptions* filters);
    virtual ~MpileupParser();
    void parse_buffer_to_reads(std::string* buffer, const Window& window, std::vector <ngh::Site>* sites);

 private:
    uint32_t number_of_individuals;
    FilterOptions* filters;
    // File and buffers
    std::ifstream in;
    ngh::Logger *logger;
};
} /* namespace ngh */
#endif /* MPILEUPPARSER_H_ */
