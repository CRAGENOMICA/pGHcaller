/*
 * Filters.h
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

#ifndef FILTERS_H_
#define FILTERS_H_

#include "CCommon.h"
#include "CFilterOptions.h"
#include "Name.h"
#include <errno.h>
#include <sstream>


/*
 * TODO:
 * remove msplit2 function
 * generalize set options
 */

namespace ngh {
class Filters : public ngh::CCommon {
public:
    Filters(unsigned int individuals) {
        size = individuals;
        names = new ngh::Name[size];
        filterOptions = new FilterOptions[size];
        /*
         * Create default names and filter options for each individual.
         * By default, name will be set using suffix INDIVIDUAL_#number_#variant
         */
        // Check for cmd parameters if we have to change defaults propeties in  names[]
        // TODO: Implement a option to parse names, store filters, ...
        // Name* ListNames = new Name[individuals];
        // Set suffix
        // add individual tag
        for( long long int i=0; i<individuals; i++)	{
            // Append individual number to default name
            names[i].name.append( std::to_string(i) );
            //DEBUG_MSG("NAMES " << names[i].name);
        }


    };
    ~Filters() {
        delete(names);
        delete(filterOptions);
    };

    void setNames(const std::string& str) {
        // Split Names comma-separated names
        std::vector <std::string> t;
        msplit2(str, ',', &t);
        for(unsigned int i = 0; i < t.size(); i++)
            names[i].name = t.at(i).c_str();
        t.clear();
    }

    ngh::Name* getNames() {
        return names;
    }

    FilterOptions* getFilterOptions() {
        return filterOptions;
    }

    void setPlatforms( const std::string& str ) {
        std::vector <std::string> t;
        msplit2(str, ',', & t);

        // If only one param is provided, generalize to all
        if( t.size() == 1 ) {
            for(unsigned int i = 0; i < size; i++) 	{
                int value = std::atoi(t.at(0).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].platforms = value;
                    DEBUG_MSG("Set platforms " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        } else {
            for(unsigned int i = 0; i < t.size(); i++) {
                int value = std::atoi(t.at(i).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].platforms = value;
                    DEBUG_MSG("Set platforms " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        }
        t.clear();
    }

    void setBaseqs( const std::string& str ) {
        std::vector <std::string> t;
        msplit2(str, ',', & t);

        // If only one param is provided, generalize to all
        if( t.size() == 1 ) {
            for(unsigned int i = 0; i < size; i++) {
                int value = std::atoi(t.at(0).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].baseqs = value;
                    DEBUG_MSG("Set baseqs " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        } else {
            for(unsigned int i = 0; i < t.size(); i++)
            {
                int value = std::atoi(t.at(i).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].baseqs = value;
                    DEBUG_MSG("Set baseqs " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        }
        t.clear();
    }
    void setMaxdeps( const std::string& str ) {
        std::vector <std::string> t;
        msplit2(str, ',', & t);

        // If only one param is provided, generalize to all
        if( t.size() == 1 ) {
            for(unsigned int i = 0; i < size; i++) {
                // Check format
                int value = std::atoi(t.at(0).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].maxdeps = value;
                    DEBUG_MSG("Set Maxdeps " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        } else {
            for(unsigned int i = 0; i < t.size(); i++)
            {
                int value = std::atoi(t.at(i).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].maxdeps = value;
                    DEBUG_MSG("Set Maxdeps " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        }
        t.clear();
    }
    void setMindeps( const std::string& str ) {
        std::vector <std::string> t;
        msplit2(str, ',', & t);

        // If only one param is provided, generalize to all
        if( t.size() == 1 ) {
            for(unsigned int i = 0; i < size; i++) {
                int value = std::atoi(t.at(0).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].mindeps = value;
                    DEBUG_MSG("Set Mindeps " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        } else {
            for(unsigned int i = 0; i < t.size(); i++) {
                int value = std::atoi(t.at(i).c_str());
                if(value != ERANGE || value != EINVAL) {
                    filterOptions[i].mindeps = value;
                    DEBUG_MSG("Set Mindeps " << value );
                } else {
                    WAR_("This value will be discarted");
                }
            }
        }
        t.clear();
    }

    std::string print_option_debug(void) {
        std::ostringstream oss;
        oss << "Filter options: " << std::endl;
        for( uint32_t i=0; i<size; i++){
            oss << "\tName " << i;
            oss <<  ": " << names[i].name;
            oss << " Filter: Mindeps " << filterOptions[i].mindeps
                    << ", Maxdeps " << filterOptions[i].maxdeps
                    << ", BaseQs " << filterOptions[i].baseqs
                    << ", Platform " << filterOptions[i].platforms
                    << ", MinReads " << filterOptions[i].minreads;
            if(i<(size-1)) {
                oss << std::endl;
            }
        }

        // write obj to stream
        return oss.str();
    }


private:
    uint32_t size;
    ngh::Name* names;
    FilterOptions* filterOptions;


    void msplit2( const std::string& s , const char& delim,  std::vector<std::string> * output){
        unsigned long start = 0U;
        unsigned long end = s.find(delim);
        while (end != std::string::npos) {
            output->push_back(s.substr(start, end - start));
            start = end + 1;
            end = s.find(delim, start);
        }
        output->push_back(s.substr(start, end - start));
    }
};
}
#endif /* FILTERS_H_ */
