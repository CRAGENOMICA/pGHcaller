/*
 * Name.h
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

#ifndef NAME_H_
#define NAME_H_


namespace ngh {

class Name {

public:

	//static const char* DEFAULT_NAME_SUFFIX = "INDIVIDUAL_";
	static const char* DEFAULT_NAME_SUFFIX;
	std::string name;

	Name(std::string n) : name(n) {};
	//Name() : name(DEFAULT_NAME_SUFFIX) {};
	Name() : name("INDIVIDUAL_") {};
};
}


#endif /* NAME_H_ */
