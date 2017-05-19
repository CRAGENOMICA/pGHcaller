/*
 * COptions.h
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

#ifndef COPTIONS_H_
#define COPTIONS_H_

#include <string>
#include <vector>
#include <iostream>

#include "CCommon.h"
#include "Name.h"
#include "CFilters.h"
#include "CFilterOptions.h"
#include "CmdArguments.h"
#include "Logger.h"


namespace ngh {

enum class OPTIONS_STATUS {
    OPTIONS_ERROR=0, SUCCESS=1, OPTIONS_WARNING
};

class COptions : public ngh::CCommon {
 public:
    COptions(int, char*[], ngh::ROLE r);
    virtual ~COptions();

    ngh::OPTIONS_STATUS parse_arguments();
    void set_individuals(unsigned long);
    ngh::CmdArguments* get_cmd_arguments(void);
    ngh::Name* get_names(void);
    ngh::Filters* get_filters(void);
    int get_argc(void);
    char** get_argv(void);
    bool doAppendFasta(void);
    bool check_help(void);
    bool check_arguments_number(void);

    void help(void);
    OPTIONS_STATUS check_options(void);

    friend std::ostream& operator << (std::ostream &os,const COptions &o) {
        os << "Provided command line  arguments" << std::endl;
        os << o.cmd_options_;
           return os;
    }

 private:
    int argc_;
    char** argv_;
    ngh::ROLE role;
    ngh::CmdArguments cmd_options_;
    ngh::Filters* filters_;
    ngh::Name* fnames_;
    unsigned long individuals_;
};

}  // namespace ngh

#endif  // COPTIONS_H_
