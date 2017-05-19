/*
 * PileupFile.h
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
#ifndef SRC_PILEUPFILE_H_
#define PILEUPFILE_H_

// #include "common.h"
#include "Data.h"
#include "Logger.h"
// #include "SNPcall.h"
// #include "RoestiCaller.h"
#include "MpileupParser.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <iterator>
#include <string>

namespace ngh {
namespace Reader {

#define DEBUG_PILEUP 0

#if DEBUG_PILEUP
#define PILEUP(str) do { std::cout << "[PILEUP : "<< __LINE__ << "] - " << str << std::endl; } while (false)
#else
#define PILEUP(str) do { } while ( false )
#endif

class PileupFile {
 public:
    explicit PileupFile(std::string);
    virtual ~PileupFile();

    uint64_t get_individuals();
    uint64_t get_init_site();
    uint64_t get_end_site();
    std::string* get_buffer();
    void set_filter_options(FilterOptions* filters);

    void initialize();
    void finalize();

    int32_t gen_chunk_pool(std::vector<Chunk>&, const uint64_t);
    int32_t gen_chunk_pool(std::vector<Window>&, const uint64_t);
    int32_t gen_window_pool(std::vector<Window>&, const uint64_t);
    void read_buffer(const Chunk& chunk);
    void read_buffer(const Chunk&, std::string&);
    void free_buffer();
    // void parse(Window* window, std::vector<ngh::Site>* site_reads);
    void parse(const Window& window, std::vector<ngh::Site>* site_reads);

 private:
        uint64_t size_;
        std::string buffer_;
        uint64_t number_of_individuals_;
        uint64_t init_site_;
        uint64_t end_site_;
        bool is_initialized_;
        std::string filename_;

        // Parser
        MpileupParser *parser;

        // File and buffers
        std::ifstream in;
        Logger *logger;
        uint64_t getInitSiteFromFile();
        uint64_t getEndSiteFromFile();
        uint64_t getNumIndividualsFromFile();
        int32_t open();
        int32_t close();

        inline uint64_t get_locus(char* line) {
            const char tab[] = "\t";
            char *token, *save_ptr;
            //   chr1 2 C 0 * * 0 * * 0 * * 1 ^]. = 0 * *
            // strok is not thread safe
            // token = strtok_r(line, tab, &save_ptr);      // token <- chr1
	    // LOG_("Token CHR " << token);
            strtok_r(line, tab, &save_ptr);      // token <- chr1
            token = strtok_r(NULL, tab, &save_ptr);
	    // LOG_("Token SITE " << token);
            return( atoi(token));
        }
};
} /* namespace Reader */
} /* namespace ngh */

#endif  // SRC_PILEUPFILE_H_
