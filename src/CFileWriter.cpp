/*
 * CFileWriter.cpp
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
#include "CFileWriter.h"

namespace ngh {
namespace FileWriter {

void CFileWriter::set_names(Name* names) {
    for (uint32_t i = 0; i < individuals_; i++) {
            std::string aname = ">" + names[i].name;
            for (uint32_t j=0; j < ploidy_; j++) {
                names_.push_back(aname + "_" + std::to_string(j));
            }
    }
}

void CFileWriter::set_init_site(uint64_t site) {
            init_site_ = site;
}

void CFileWriter::set_end_site(uint64_t site) {
            reads_end_site_ = site;
            if(site > end_site_) end_site_ = site;
}

void CFileWriter::set_fasta_end_site(uint64_t site) {
            fasta_end_site_ = site;
            if(site > end_site_) end_site_ = site;
}

void CFileWriter::set_individuals(uint32_t individuals) {
            individuals_ = individuals;
}

void CFileWriter::set_output_filename(std::string file) {
            this->outputfile_ = file;
}

uint64_t CFileWriter::size(void) {
    assert( end_site_ >= init_site_);
    return (end_site_);
}

/**
 * Append fasta file pointed by filename to mfasta file
 */
void CFileWriter::append_fasta(std::string fasta_filename) {
    std::ofstream of_a;
    std::ifstream if_b;
    of_a.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    if_b.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // DEBUG_("Append file " << fasta_filename << " to fasta file " << outputfile_);
        WAR_("This is a simple APPEND operation. We are doing this operations AS IS without checking init/end sites, or name");
        //, ... in " << fastafilename );
        of_a.open(outputfile_, std::ios_base::binary | std::ios_base::app);
        if_b.open(fasta_filename, std::ios_base::binary);

        // TODO: Seek initSite position before append reference
        of_a.seekp(0, std::ios_base::end);
        of_a << if_b.rdbuf();
        of_a.close();
        if_b.close();
    }
    catch (std::ifstream::failure &e) {
        ERR_("Appending outgroup/reference: Exception opening/reading or writing input/output file");
        exit(EXIT_FAILURE);
    }
}

/**
 * Append fasta file but checking number of bases
 */


/*
 * Generate empty file using old style IO
 * - fopen the file for write.
 * - fseek to the desired file size - 1.
 * - fwrite a single byte
 * - fclose the file
 */
/*
void CFileWriter::generate_empty_file(void) {
         LOG_("Initializing " << individuals_*ploidy_ << " fasta sequences with size " << size());

         _IO_FILE * fd =NULL;
         fd = fopen(outputfile_.c_str(), "w+");
         if (NULL == fd) {
               ERR_("Cannot open file " << outputfile_);
         }
         /*
          * SEEK_SET, SEEK_CUR, or SEEK_END
          * These three values (in order) depict the start of the file, the current position and the end of the file.
          *

         // Set fd to the start of the file
         fseek(fd, 0, SEEK_SET);

         for (unsigned int i = 0; i < individuals_*ploidy_; i++) {
            // Name
            std::vector<char> empty(names_[i].begin(), names_[i].end());
            // fill with spaces
            // empty.resize(FASTA_NAME_SIZE-_names[i].size(), '*');
            empty.resize(FASTA_NAME_SIZE, ' ');
            empty.push_back('\n');
            LOG_("Writing Fasta header [" << i << "] - " << names_[i]); //'" << empty << "'");
            if ( sizeof(char)*empty.size() != fwrite(&empty[0], sizeof(char), empty.size(),fd)) {
                ERR_("problem writing to file");
                exit(EXIT_FAILURE);
            }

            // Sequence: Move to the end of this sequence size and write \n
            fseek(fd, size(), SEEK_CUR);
            LOG_("Writing fasta end [" << i << "] - at position " << SEEK_CUR );
            // if ( sizeof(char) != fwrite("\n", sizeof(char), 1, fd)) {
            if ( sizeof(char) != fwrite("X", sizeof(char), 1, fd)) {
                ERR_("problem writing to file");
                exit(EXIT_FAILURE);
            }

            // if (!outfile.write( &n_vector[0], n_vector.size())) {
            //     ERR_("problem writing to file");
            //     exit(EXIT_FAILURE);
            // }

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
            *
        }
        fclose(fd);
}
*/

/*
void CFileWriter::generate_empty_file(void) {

    // TOCpas   O: Check error opening/closing file
    std::ofstream outfile(outputfile_, std::ios::out | std::ofstream::binary);

    try {
        outfile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        std::vector<char> n_vector(end_site_, 'N');
        LOG_("Initializing " << individuals_*ploidy_ << " fasta sequences with size " << n_vector.size());
        n_vector.push_back('\n');

        // TODO PROGRESS BAR HERE??
        for (unsigned int i = 0; i < individuals_*ploidy_; i++) {
            // Name
            std::vector<char> empty(names_[i].begin(), names_[i].end());
            // fill with spaces
            // empty.resize(FASTA_NAME_SIZE-_names[i].size(), '*');
            empty.resize(FASTA_NAME_SIZE, ' ');
            empty.push_back('\n');

            LOG_("Fasta Name vector[" << i << "] - " << names_[i]); //'" << empty << "'");
            if (!outfile.write( &empty[0], empty.size())) {
                ERR_("problem writing to file");
                exit(EXIT_FAILURE);
            }

            // Sequence
            // Move to the end of this sequence size and write \n
            outfile.se

            // if (!outfile.write( &n_vector[0], n_vector.size())) {
            //     ERR_("problem writing to file");
            //     exit(EXIT_FAILURE);
            // }

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
            *
        }
        outfile.close();
    }
    catch (std::ofstream::failure& e) {
        ERR_("Exception open/write generating empty file " << outfile);
    }
}
*/
}  // namespace FileWriter
}  // namespace ngh
