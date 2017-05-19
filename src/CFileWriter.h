/*
 * CFileWriter.h
 *
 *  Created on: Oct 30, 2015
 *      Author: jnavarro
 *
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
#ifndef SRC_CFILEWRITER_H_
#define SRC_CFILEWRITER_H_

#include "CFileWriter.h"
#include "CCommon.h"
#include "Logger.h"
#include "Name.h"
#include "Data.h"
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>

namespace ngh {
namespace FileWriter {

#define FASTA_NAME_SIZE 25

#define DEBUG_WRITER 0
#if DEBUG_WRITER
#define WRITER_(str) do { std::cout << "[WRITER: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define WRITER_(str) do { } while ( false )
#endif


class CFileWriter : public ngh::CCommon {
 public:
    CFileWriter() {
        logger = ngh::Logger::getInstance();
        ploidy_ = 2;
        individuals_ = 0;
        init_site_ = 0;
        end_site_ = 0;
        reads_end_site_ = 0;
        fasta_end_site_ = 0;
    }
    CFileWriter(std::string file, uint32_t individuals, ngh::Name* names) {
                logger = ngh::Logger::getInstance();
                individuals_ = individuals;
                ploidy_ = 2;
                this->outputfile_ = file;
                set_names(names);
    }
    virtual ~CFileWriter() {}
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void build(const SeqProperties* properties, const Window& window, const CBasecalls& basecalls) = 0;
    virtual void write_headers() = 0;
    virtual void fill_init_sites_with_n() = 0;
    virtual void fill_sites_with_n(uint64_t, uint64_t) = 0;
    virtual void fill_end_sites_with_n(void) = 0;
    void append_fasta(std::string fasta_filename);
    uint64_t size(void);

    void set_individuals(uint32_t individuals);
    void set_output_filename(std::string filename);
    void set_names(Name* names);
    void set_init_site(uint64_t site);
    void set_end_site(uint64_t site);
    void set_fasta_end_site(uint64_t site);

 protected:
    ngh::Logger *logger;
    std::vector <std::string> names_;
    std::string outputfile_;

    uint32_t ploidy_;
    uint32_t individuals_;
    // Default 1
    uint64_t init_site_;
    // Max. site position in FASTA or Reference/outgroup file
    uint64_t end_site_;
    uint64_t reads_end_site_;
    uint64_t fasta_end_site_;

    int file_handler_;   // file descriptor
    uint64_t filesize_;  // Store filesize of memory mapped file
};


}  // namespace FileWriter
}  // namespace ngh

#endif   // SRC_CFILEWRITER_H_

