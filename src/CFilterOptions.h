/*
 * FilterOptions.h
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

#ifndef FILTEROPTIONS_H_
#define FILTEROPTIONS_H_

namespace ngh {
class FilterOptions {

public:
	const static int DEFAULT_MINDEP=3;
	const static int DEFAULT_MAXDEP=100;
	const static int DEFAULT_PLATFORM=33;
	const static int DEFAULT_BASEQ=20;
	const static int DEFAULT_MINREAD=6;

	unsigned int mindeps;
	unsigned int maxdeps;
	unsigned int platforms;
	unsigned int baseqs;
	unsigned int minreads;

	FilterOptions(unsigned int min, unsigned int max, unsigned int p, unsigned int b,
			unsigned int mr)
	: mindeps(min), maxdeps(max), platforms(p), baseqs(b), minreads(mr) {};
	FilterOptions()
	:mindeps(DEFAULT_MINDEP), maxdeps(DEFAULT_MAXDEP), platforms(DEFAULT_PLATFORM), baseqs(DEFAULT_BASEQ), minreads(DEFAULT_MINREAD) {};
};

}




#endif /* FILTEROPTIONS_H_ */
