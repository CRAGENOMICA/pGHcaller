/*
 * CManager.h
 *
 *  Created on: Jun 11, 2015
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

#ifndef SRC_CMANAGER_H_
#define SRC_CMANAGER_H_

#include "CCommon.h"
#include "COptions.h"
#include "MasterWorker.h"
#include "FastaFile.h"
#include "CFileWriter.h"
#include "CFileWriterMPIIO.h"
#include "MpileupParser.h"
#include "Event.h"
#include "CAlgorithm.h"
#include "IAlgorithm.h"
#include "RoestiCaller.h"
#include <mpi.h>
#ifdef  _OPENMP
#include <omp.h>
#endif
#include "PileupFile.h"
#if  MPE
#include "MPEEventLog.h"
#else
#include "DummyEventLog.h"
#endif
#include <string>
#include <vector>

namespace ngh {

// TODO(jnavarro): Add exception control throws errors!!!
template <class T>
class CManager : public CCommon {
 public:
    explicit CManager(COptions* options);
    virtual ~CManager();
    void run();

    void set_options(ngh::COptions* options);
    void set_taskmanager(ngh::MasterWorker<T>* task_manager);
    void set_algorithm(ngh::Algorithm::CAlgorithm* algorithm);
    void set_output_writer(ngh::FileWriter::CFileWriter* writer);

 private:
    // void run_worker_omp();
    void run_worker();
    void run_master();

    void initialize();
    void finalize();



    ngh::IAlgorithm* algorithm_;
    ngh::MasterWorker<T>* taskmanager_;
    ngh::COptions*   options_;
    ngh::EventLog* events_;

    // Read/Writer
    ngh::Reader::PileupFile* inputfile_;
    // ngh::MFastaFile* outputfile_;
    ngh::FastaFile*  fasta_;
    ngh::FileWriter::CFileWriter* writer_;

    ngh::ROLE role;
    std::string input_filename_;
    std::string output_filename_;
    std::string outgroup_filename_;
    bool outgroup_append_;
};

} /* namespace ngh */

#endif  // SRC_CMANAGER_H_
