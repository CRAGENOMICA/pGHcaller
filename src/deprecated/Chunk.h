/*
 *  Chunk.h
 *
 *  Created on: Mar 3, 2015
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


#ifndef SRC_CHUNK_H_
#define SRC_CHUNK_H_

#include <stdint.h>

namespace ngh {

struct Chunk {
    // uint64_t id;
    uint64_t start;
    uint64_t end;
    Chunk() {}
    Chunk(uint64_t start, uint64_t end) : start(start), end(end) {}
};

struct Window {
    uint64_t site_start;
    uint64_t site_end;
    uint64_t file_start;
    uint64_t file_end;
    Window() {}
    Window(uint64_t s, uint64_t e, uint64_t fs, uint64_t fe)
        : site_start(s), site_end(e), file_start(fs), file_end(fe) {}
};
}


#endif  // SRC_CHUNK_H_
