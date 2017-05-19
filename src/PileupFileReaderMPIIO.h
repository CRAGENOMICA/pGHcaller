/*
 * PileupFileReaderMPIIO.h
 *
 *  Created on: May 19, 2015
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

#ifndef PILEUPFILEREADERMPIIO_H_
#define PILEUPFILEREADERMPIIO_H_

namespace ngh {

class PileupFileReaderMPIIO : public ngh::PileupFile {
public:
    PileupReaderMPIIO();
    virtual ~PileupFileReaderMPIIO();
};

} /* namespace ngh */

#endif /* PILEUPFILEREADERMPIIO_H_ */
