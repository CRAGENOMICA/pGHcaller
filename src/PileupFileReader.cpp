/*
 * BufferReader.cpp
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

#include "PileupFileReader.h"

namespace ngh {

PileupFileReader::PileupFileReader() {
    // TODO Auto-generated constructor stub

}

PileupFileReader::~PileupFileReader() {
    // TODO Auto-generated destructor stub
}

void PileupFileReader::readBuffer(const Chunk& chunk){
    // Find file position
    in.seekg(chunk.start);
    // read data as a block:
    int size = chunk.end - chunk.start;
    buffer.resize(size, '\0');
    in.read (&buffer[0], size);
}

} /* namespace ngh */
