/*
 * CAlgorithm.h
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
#ifndef SRC_CALGORITHM_H_
#define SRC_CALGORITHM_H_

#include "CCommon.h"
#include "IAlgorithm.h"
#include "COptions.h"
#include "CAlgorithmRoesti.h"
#include "CAlgorithmDummy.h"


namespace ngh {
namespace Algorithm {


/*
 *
 */
enum class AlgorithmType {
    DUMMY = 0,
    ROESTI,
    PF,
    AlgorithmTypeTypeFirst = DUMMY,
    AlgorithmTypeTypeLast = PF
};


class CAlgorithm : public ngh::CCommon {
 public:
    explicit CAlgorithm(ngh::Algorithm::AlgorithmType type, ngh::COptions* options);
    ~CAlgorithm();
    void set_algorithm(AlgorithmType type, COptions* options);
    IAlgorithm * get_algorithm();

 private:
    IAlgorithm *algorithm_;
}; /* namespace ngh */

}  // namespace Algorithm
}  // namespace ngh
#endif /* SRC_CALGORITHM_H_ */
