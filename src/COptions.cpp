/*
 * COptions.cpp
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

#include "COptions.h"

namespace ngh {

COptions::COptions(int argc, char* argv[], ngh::ROLE r) {
    this->argv_ = argv;
    this->argc_ = argc;
    // parse_arguments();
    role = r;
}

COptions::~COptions() {}

// void COptions::parse_arguments(int argc, char* argv[]) {
ngh::OPTIONS_STATUS  COptions::parse_arguments() {
    // Parse arguments
    cmd_options_.parse(this->argc_, this->argv_);

    // Check options status
    ngh::OPTIONS_STATUS status = check_options();
    if(ngh::ROLE::MASTER == role) {
        switch(status) {
        case ngh::OPTIONS_STATUS::SUCCESS:
            break;
        case ngh::OPTIONS_STATUS::OPTIONS_WARNING:
            WAR_("Some options cannot be parsed but we can continue.");
            break;
        case ngh::OPTIONS_STATUS::OPTIONS_ERROR:
            help();
            ERR_("Error detected parsing options.");
            ERR_("Exiting!");
        }
    }
    return(status);
}


void COptions::set_individuals(unsigned long individuals) {
    this->individuals_ = individuals;

    /*
     *  Get Filter options and output names from command line or options file. Filters
     *  are created with defaults values.
     */
    this->filters_ = new ngh::Filters(individuals);
    std::string names = cmd_options_.getNames();
    if (!names.empty()) {
        // if(ngh::ROLE::MASTER == role )
        //   DEBUG_("Names from command line  " << names );
        filters_->setNames(names);
    }
    std::string platforms = cmd_options_.getPlatforms();
    if (!platforms.empty()) {
        // if(ngh::ROLE::MASTER == role )
        //   DEBUG_("Platforms from command line  " << platforms );
        filters_->setPlatforms(platforms);
    }
    std::string baseqs = cmd_options_.getBaseqs();
    if (!baseqs.empty()) {
        // if(ngh::ROLE::MASTER == role )
        //  DEBUG_("Base Quality from command line  " << baseqs);
        filters_->setBaseqs(baseqs);
    }
    std::string mindeps = cmd_options_.getMindeps();
    if (!mindeps.empty()) {
        //if(ngh::ROLE::MASTER == role )
        //    DEBUG_("Min. depth from command line  " << mindeps );
        filters_->setMindeps(mindeps);
    }
    std::string maxdeps = cmd_options_.getMaxdeps();
    if( ! maxdeps.empty() )
    {
        //if(ngh::ROLE::MASTER == role )
        //    DEBUG_("Max. depth from command line  " << maxdeps );
        filters_->setMaxdeps( maxdeps );
    }

    this->fnames_ = filters_->getNames();
    ngh::FilterOptions* f;
    f = filters_->getFilterOptions();
    if(ngh::ROLE::MASTER == role ) {
        for( unsigned int i=0; i<individuals; i++) {
                LOG_("Name " << i << ": " << fnames_[i].name << " "
                << "Filter: " << f[i].mindeps
                << ", " << f[i].maxdeps
                << ", " << f[i].baseqs
                << ", " << f[i].platforms
                << ", " << f[i].minreads );
        }
    }
}
ngh::CmdArguments* COptions::get_cmd_arguments(void) {
    return &cmd_options_;
}

ngh::Name* COptions::get_names(void) {
    return fnames_;
}

ngh::Filters* COptions::get_filters(void) {
    return filters_;
}

int COptions::get_argc(void) {
    return argc_;
}

char** COptions::get_argv(void) {
    return argv_;
}

bool COptions::doAppendFasta(void) {
    return cmd_options_.doConcatenateFasta();
}

bool COptions::check_help(void) {
     bool b = cmd_options_.printHelp();
     if ( b && role == ROLE::MASTER)  {
          help();
     }
     return(b);
}

bool COptions::check_arguments_number(void) {
    if (role == ROLE::MASTER)  {
        LOG_("Starting GH caller version " << GHCALLER_VERSION);
    }
    if(argc_ < 2 ) {
         if (role == ROLE::MASTER) {
             ERR_("No options provided. Use -h to get more information.");
             ERR_("Exiting!");
         }
         return(false);
     }
    return(true);
}


void COptions::help(void) {
        cmd_options_.help();
}

OPTIONS_STATUS COptions::check_options(void) {
    OPTIONS_STATUS status = OPTIONS_STATUS::SUCCESS;

    // Check cmd options first
    switch(cmd_options_.check()) {
    case ARGUMENTS_STATUS::SUCCESS:
            status = OPTIONS_STATUS::SUCCESS;
            break;
    case ARGUMENTS_STATUS::ARGUMENT_ERROR:
        status = OPTIONS_STATUS::OPTIONS_ERROR;
        break;
    case ARGUMENTS_STATUS::ARGUMENT_WARNING:
        status = OPTIONS_STATUS::OPTIONS_WARNING;
            break;
    }
     //TODO)(jnavarro) check other internal options here
     // this->check();
    return status;
}


} /* namespace ngh */
