/*
 * Data.h
 *
 *  Created on: Oct 28, 2015
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
#ifndef SRC_DATA_H_
#define SRC_DATA_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <cstring>

namespace ngh {

class Chunk {
 public:
    // uint64_t id;
    uint64_t start;
    uint64_t end;
    Chunk() {}
    Chunk(uint64_t start, uint64_t end) : start(start), end(end) {}
};

class Window {
 public:
    uint64_t nsites;
    uint64_t site_start;
    uint64_t site_end;
    uint64_t file_start;
    uint64_t file_end;

    Window() :
        nsites(0), site_start(0), site_end(0), file_start(0), file_end(0) {}
    Window(uint64_t sites, uint64_t s, uint64_t e, uint64_t fs, uint64_t fe)
        : nsites(sites),
          site_start(s), site_end(e),
          file_start(fs), file_end(fe) {}

    uint64_t sites() const {
	if(site_end <= site_start) {
		return 0L;
	} 
        return (site_end - site_start + 1);
    //    return(nsites);
    }
    uint64_t size() const {
            return (file_end - file_start);
    } 
    inline bool check() const {
	// Start >> End
	if (site_start >= site_end)
		return false;
	if (file_start >= file_end)
		return false;
	// Sites number cannot be greater than size
	if (sites() > size()) 
		return false;
	return true;
    }
};
/*
std::ostream& operator<<(std::ostream& os, ngh::Window& w) {
    // os << " Window " << w.site_start << ":" << w.site_end << "-" << (w.site_end - w.site_start + 1) <<  " , " << w.file_start << ":" <<  w.file_end << "-" << w.file_end - w.file_start;
    os << "1";
    return os;
}
*/

class Site {
 public:
    Site() {}
    explicit Site(uint64_t size) : reads(size) {}
    ~Site() {}
    uint64_t site;
    std::vector <std::string> reads;

    uint32_t size() const {
    	return reads.size();
    }
    std::string& operator[](uint32_t i) {
    	return reads[i];
    }
    const std::string& operator[](uint32_t i) const {
    	return reads[i];
    }
};


class SeqProperties{
 public:
    uint32_t individuals;
    uint32_t ploidy;
    SeqProperties() {}
    ~SeqProperties() {}
    SeqProperties(uint32_t individuals, uint32_t ploidy)
           : individuals(individuals), ploidy(ploidy) {}
    uint32_t size() {
        return individuals*ploidy;
    }
    SeqProperties& operator=(SeqProperties rhs){
      std::swap(*this, rhs);
      return *this;
    }
};


class CBasecalls{
 public:
    // ngh::SeqProperties* region;
    std::vector<std::string> called_genotypes;

    explicit CBasecalls(ngh::SeqProperties* region, const ngh::Window* window) {
        // this->region = region;
        // Create vector with basecalls
        called_genotypes.resize((region->individuals) * (region->ploidy));
        // Fill with 'n'.
        for (auto bc = called_genotypes.begin();
                bc != called_genotypes.end(); bc++) {
            (*bc).resize(window->sites(), 'N');
            // (*bc).resize(get_sites(window), 'N');
        }
    }

    ~CBasecalls() {
	called_genotypes.clear();
    }
    void clear() {
	 called_genotypes.clear();
    }
    uint32_t size() {
            return called_genotypes.size();
    }
    std::string& operator[](uint32_t i) {
    	return called_genotypes[i];
    }
    const std::string& operator[](uint32_t i) const {
    	return called_genotypes[i];
    }
};



}  // namespace ngh
#endif  // SRC_DATA_H_

