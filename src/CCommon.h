/*
 *  CCommon.h
 *
 *  Created on: Jun 11, 2015
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


#ifndef SRC_CCOMMON_H_
#define SRC_CCOMMON_H_
#include <errno.h>
#include <stdint.h>
#include <sys/resource.h>
#include <cassert>
// #include "./common.h"
#include "../config.h"
#include "Logger.h"

namespace ngh {

#define GHCALLER_VERSION "0.2.3"

enum class ROLE   { MASTER, WORKER, UNDEF };

class CCommon {
 public:
    CCommon() {
        // Get logger instance
        // logger = ngh::Logger::getInstance();
        // role = ngh::ROLE::UNDEF;
    }
    ~CCommon() {
    }

    uint64_t getMemoryUsage(void) {
      struct rusage r_usage;
      getrusage(RUSAGE_SELF, &r_usage);
      // Return in MB
      return(r_usage.ru_maxrss/1024);
    }
 protected:
    // ngh::Logger *logger;
    // ngh::ROLE role;
};
}  /* namespace ngh */

#endif  // SRC_CCOMMON_H_
