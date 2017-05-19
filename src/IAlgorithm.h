/*
 * IAlgorithm.h
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
#ifndef SRC_IALGORITHM_H_
#define SRC_IALGORITHM_H_

namespace ngh {

/*
 *
 */
class IAlgorithm {
 public:
    IAlgorithm() {}
    virtual ~IAlgorithm() {}
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual void run() = 0;
    // virtual void set_cmd_options(ngh::COptions* options) = 0;
};

} /* namespace ngh */

#endif /* SRC_IALGORITHM_H_ */
