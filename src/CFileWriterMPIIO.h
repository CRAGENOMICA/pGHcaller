/*
 * CFileWriterMPIIO.h
 *
 *  Created on: Nov 4, 2015
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
#ifndef SRC_CFILEWRITERMPIIO_H_
#define SRC_CFILEWRITERMPIIO_H_

#include "CFileWriter.h"
#include "CCommon.h"
#include "Logger.h"
#include "Name.h"
#include "Data.h"
#include <mpi.h>

namespace ngh {
namespace FileWriter {

/*
 *
 */
class CFileWriterMPIIO: public FileWriter::CFileWriter {
 public:
    CFileWriterMPIIO();
    virtual ~CFileWriterMPIIO();

    void initialize() {
        // Open file
        // fh_ = MPI::File::Open(MPI::COMM_WORLD, outputfile_.c_str(), MPI::MODE_CREATE | MPI::MODE_WRONLY, MPI::INFO_NULL, fh_);
        MPI_File_delete(outputfile_.c_str(), MPI_INFO_NULL);
        // WRITER_("Open/Create file " << outputfile_);
        MPI_File_open(MPI_COMM_WORLD, outputfile_.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh_);
        // Write headers (ONLY MASTER)
    }
    void finalize() {
        // close file
        MPI_File_close(&fh_);
    }
    void build(const SeqProperties* properties, const Window& window, const CBasecalls& basecalls) {
        for (uint32_t i=0; i < properties->individuals; i++) {
            for (uint32_t p=0; p < properties->ploidy; p++) {
                WRITER_("Basecalls [" << i << ", " << p << "]:" << basecalls.called_genotypes[i + p]);
                WRITER_("CALLED IND[" << i << "] PLOIDY[" << p << "]");
                // build_called_genotype(basecalls.region->ploidy*i+p, window, basecalls);
                build_called_genotype(properties->ploidy*i+p, window, basecalls);
            }
        }
    }

 private:
    MPI_File fh_;
    MPI_Offset offset_;
    // ROMIO_CONST void *buf_;
    MPI_Datatype datatype_;
    MPIO_Request request_;

    void write_headers() {
        LOG_("Initializing " << individuals_*ploidy_ << " fasta sequences with size " << size());
        DEBUG_("Writing Fasta headers #" << names_.size());

        // Set fd to the start of the file
        MPI_File_seek(fh_, 0, MPI_SEEK_SET);

        for (unsigned int i = 0; i < individuals_*ploidy_; i++) {
            // Create Name with a fixed size of FASTA_NAME_SIZE
            std::vector<char> empty(names_[i].begin(), names_[i].end());
            empty.resize(FASTA_NAME_SIZE-1, ' ');
            empty.push_back('\n');

            DEBUG_("Writing Fasta header [" << i << "] - " << names_[i]); //'" << empty << "'");
            uint32_t rc = MPI_File_write(fh_, &empty[0], empty.size(), MPI_CHAR, MPI_STATUS_IGNORE);
            if (rc != MPI_SUCCESS) {
                          ERR_("error writing results file\n");
                                MPI_Abort(MPI_COMM_WORLD, 5);
                 }

            // Sequence: Move to the end of this sequence size and write \n
            MPI_File_seek(fh_, size() , MPI_SEEK_CUR);
            // DEBUG_("Writing fasta end [" << i << "] - at position " << MPI_SEEK_CUR );
            // for (uint32_t j=0; j < size(); j++)
            //     rc = MPI_File_write(fh_, "X", 1, MPI_CHAR, MPI_STATUS_IGNORE);
            rc = MPI_File_write(fh_, "\n", 1, MPI_CHAR, MPI_STATUS_IGNORE);
            if (rc != MPI_SUCCESS) {
                ERR_("error writing \endline to file");
                MPI_Abort(MPI_COMM_WORLD, 5);
            }
        }

        // Set fd to the start of the file
        MPI_File_seek(fh_, 0, MPI_SEEK_SET);
    }

    void fill_init_sites_with_n() {
        // Window start cannot be 0
        assert(init_site_ > 0);
        uint64_t offset = 0;

        std::string vector;
        vector.resize(init_site_ - 1, 'N');
	if (init_site_ > 1) {
            DEBUG_("First site is " << init_site_ << ". Fill all sequences from site 1 to " << init_site_ << " with 'N', size " << vector.size());
        }
        for (uint32_t i=0; i < individuals_*ploidy_; i++) {
        /// uint64_t base = ((FASTA_NAME_SIZE  + size() + sizeof('\n') ) * i) + FASTA_NAME_SIZE;
            uint64_t base = ((FASTA_NAME_SIZE  + size() + sizeof('\n') ) * i) + FASTA_NAME_SIZE;
        MPI_File_seek(fh_, offset + base, MPI_SEEK_SET);
        WRITER_("Seek Base:" << base << " offset: " << offset);
        uint32_t rc = MPI_File_write(fh_, &vector[0], vector.size()*sizeof(char), MPI_CHAR, MPI_STATUS_IGNORE);
        if (rc != MPI_SUCCESS) {
            ERR_("error writing results file\n");
            MPI_Abort(MPI_COMM_WORLD, 5);
        }
        }
    }
    
    void fill_end_sites_with_n() {
        if(fasta_end_site_ > reads_end_site_) {
    	     fill_sites_with_n(reads_end_site_, fasta_end_site_);
        }
    }

    void fill_sites_with_n(uint64_t init, uint64_t end) {
        assert(end_site_ > 0);
        assert(init > 0); assert(end > 0);
	assert(init < end);
        uint64_t offset = init;

        std::string vector;
        vector.resize(end - init, 'N');
        DEBUG_("Filling all sequences from site " << init 
	       << " to " << end << " with 'N', size " << vector.size());
        for (uint32_t i=0; i < individuals_*ploidy_; i++) {
            uint64_t base = ((FASTA_NAME_SIZE  + size() + sizeof('\n') ) * i) + FASTA_NAME_SIZE;
            MPI_File_seek(fh_, offset + base, MPI_SEEK_SET);
            WRITER_("Seek Base:" << base << " offset: " << offset);
            uint32_t rc = MPI_File_write(fh_, &vector[0], vector.size()*sizeof(char), MPI_CHAR, MPI_STATUS_IGNORE);
            if (rc != MPI_SUCCESS) {
                 ERR_("error writing results file\n");
                 MPI_Abort(MPI_COMM_WORLD, 5);
            }
        }
    }

    inline void build_called_genotype(uint32_t seq, const Window& window, const CBasecalls& basecalls) {
        // WRITER_("CALLED SEQ[" << seq << "]: " << basecalls.called_genotypes[seq]);

        //  MPI::MPI_File_write_at(fh_, basecalls.called_genotypes[individual + ploidy], basecalls.called_genotypes[individual + ploidy].size()*sizeof(char), MPI_CHAR, MPI_STATUS_IGNORE);
        /*
        disp = my_rank * LOCAL_SIZE * sizeof(int);
        MPI_File_set_view(out_fh, disp, contig, contig, "native", MPI_INFO_NULL);
          if (rc != MPI_SUCCESS) {
            fprintf(stderr, "error setting view on results file\n");
            MPI_Abort(MPI_COMM_WORLD, 4);
          }

          // MPI Collective Write
          t0 = MPI_Wtime();
          rc = MPI_File_write_all(out_fh, buf, 1, contig, MPI_STATUS_IGNORE);
          if (rc != MPI_SUCCESS) {
            fprintf(stderr, "error writing results file\n");
            MPI_Abort(MPI_COMM_WORLD, 5);
          }
          */
        // Throw exception here
        // Window start cannot be 0
        // assert(window.site_start > 0);
        uint64_t offset = window.site_start - 1;
        // FASTA + newline + SEQ:AAAAAAAAAAAAAAAAAAAAAAAAAAA + newline
        // --> uint64_t base = ((FASTA_NAME_SIZE + sizeof('\n') + size() + sizeof('\n') ) * seq) + FASTA_NAME_SIZE + sizeof('\n');
        // --> // --> uint64_t base = ((FASTA_NAME_SIZE + sizeof('\n') + size() + sizeof('\n') ) * seq) + FASTA_NAME_SIZE;
        ////uint64_t base = ((FASTA_NAME_SIZE + sizeof('\n') + size() + sizeof('\n') ) * seq) + FASTA_NAME_SIZE + sizeof('\n');
        uint64_t base = ((FASTA_NAME_SIZE + size() + 1 ) * seq) + FASTA_NAME_SIZE;

        // unsigned long base = (FASTA_NAME_SIZE + sizeof('\n') + _endSite + sizeof('\n')) * sequenceNumber;
        // unsigned long offset = FASTA_NAME_SIZE + sitePosition;

        MPI_File_seek(fh_, offset + base, MPI_SEEK_SET);
        WRITER_("Called seq[" << seq << "]: Seek Base:" << base << " offset: " << offset);
        WRITER_("Starts with: " <<  basecalls.called_genotypes[seq].substr(1,50).c_str() << " .. ");
        WRITER_("Called Sequence[" << seq << "]: Seek Base:" << base << " offset: " << offset);
        WRITER_("Window sites #" << window.sites() << " start:" << window.site_start << " end:" << window.site_end);

        uint32_t rc = MPI_File_write(fh_,
                &basecalls.called_genotypes[seq][0] ,
                basecalls.called_genotypes[seq].size()*sizeof(char),
                MPI_CHAR, MPI_STATUS_IGNORE);
        /*
        char a = 'a';
        // Set the individual pointer to 0
        // MPI_File_seek(fh_, 0, MPI_SEEK_SET);
        uint32_t rc = MPI_File_write(fh_, &a,sizeof(MPI_CHAR), MPI_CHAR, MPI_STATUS_IGNORE);
        */
        if (rc != MPI_SUCCESS) {
                    ERR_("error writing results file\n");
                    MPI_Abort(MPI_COMM_WORLD, 5);
        }
    }
};

}  // namespace FileWriter
}  // namespace ngh
#endif /* SRC_CFILEWRITERMPIIO_H_ */
