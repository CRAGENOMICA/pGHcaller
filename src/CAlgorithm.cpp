/*
 * CAlgorithm.cpp
 *
 *  Created on: Oct 2, 2015
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
#include "CAlgorithm.h"


namespace ngh {
namespace Algorithm{

CAlgorithm::CAlgorithm(ngh::Algorithm::AlgorithmType type, ngh::COptions* options) {
        set_algorithm(type, options);
}

CAlgorithm::~CAlgorithm() {
}

void CAlgorithm::set_algorithm(ngh::Algorithm::AlgorithmType type, ngh::COptions* options) {
    switch (type) {
    case ngh::Algorithm::AlgorithmType::ROESTI: {
        // m_algorithm = new CAlgorithmRoesti();
        // algorithm_ = new CAlgorithmRoesti();
        // algorithm_ = new RoestiCaller();

        uint32_t min_reads = options->get_cmd_arguments()->getMinReads();
        float error = options->get_cmd_arguments()->getPErr();
        float chi = options->get_cmd_arguments()->getChi();

        algorithm_ = new ngh::CAlgorithmRoesti(min_reads, error, chi);
        break;
    }
    case ngh::Algorithm::AlgorithmType::DUMMY: {
        // Get options to Dummy Algorithm and
        // create object
        algorithm_ = new ngh::CAlgorithmDummy();

        break;
    }
    default:
        assert("Algorithm Class unknown");
    }
}
ngh::IAlgorithm * CAlgorithm::get_algorithm() {
    return algorithm_;
}
}  // namespace Algorithm
} /* namespace ngh */
