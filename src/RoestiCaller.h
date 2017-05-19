/*
 * RoestiCaller.h
 *
 *  Created on: Feb 20, 2015
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

#ifndef ROESTICALLER_H_
#define ROESTICALLER_H_

#include "CCommon.h"
#include "Data.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <set>
#include <cstring>
#include <cmath>  // lgamma, log
#include <algorithm>
#include <locale>
#include <sstream>


#define DEBUG_ROESTI 0
#if DEBUG_ROESTI
#define ROESTI_MSG(str) do { std::cout << "[ROESTI_SNPCALL: " << __PRETTY_FUNCTION__ << ":"<< __LINE__ << "] - " << str << std::endl; } while( false )
#else
#define ROESTI_MSG(str) do { } while ( false )
#endif


//#define GHCALLER_ALLWAYS_INLINE __attribute__((always_inline))
#define GHCALLER_ALLWAYS_INLINE inline
#define GHCALLER_INLINE inline

// SNP CALLING DEFAULT ALGORITHM OPTIONS 
 
#define LRT_NOT_SIGNIFICANT -1         //!< LRT between 2 most likely genotypes/haplotypes is not significant
#define ROESTI_DEFAULT_MINREADS     6         //!< genotype / haplotype call threshold
#define ROESTI_DEFAULT_P_ERR        0.01f     //!< raw sequencing error
#define ROESTI_DEFAULT_CHIVALUE     3.841459f //!< 1 d.f., 0.05 (for LRT)
#define ROESTI_DEFAULT_HAPLOCALLS   true      //!< Enable haplotype calls


// TODO(jnavarro) Check LUT size
#define DEFAULT_PS_SIZE	     10
#define MAXLUT               512
#define LRT_NOT_SIGNIFICANT -1         //!< LRT between 2 most likely genotypes/haplotypes is not significant


namespace ngh {

class RoestiCaller : public ngh::CCommon {

private:
    float LUT_lgamma[MAXLUT];
    static float log_value_p_err1;
    static float log_value_p_err2;
    static float log_value_p_err3;
    static unsigned int minreads;          // genotype / haplotype call threshold
    static float p_err;                    // raw sequencing error
    static float chivalue;                 // 1 d.f., 0.05 (for LRT)
    static bool haplocalls;                // to enable/disable haplotype calls

    const static char HAPLOTYPES[];
    const static char GENOTYPES_PLOYDI_0[];
    const static char GENOTYPES_PLOYDI_1[];

    GHCALLER_INLINE int lrt_genotypes(const float ps[]);
    GHCALLER_INLINE int lrt_haplotypes(const float ps[]);
    GHCALLER_INLINE void calc_genos_probs ( std::string reads, float ps[] );
    GHCALLER_ALLWAYS_INLINE float calc_homo_p ( const int& n1, const int& n2, const int& n3, const int& n4 );
    GHCALLER_ALLWAYS_INLINE float calc_hetero_p ( const int& n1, const int& n2, const int& n3, const int& n4 );
    void setDefaults();

public:
    RoestiCaller();
    RoestiCaller(int mreads, float err, float chi, bool haplocalls);
    void set_mread(int mreads);
    void set_error(float err);
    void set_chi(float chi);
    void set_haplocalls(bool haplo);
    void process(const SeqProperties* properties, const std::vector<ngh::Site>* sites, ngh::CBasecalls *calls);
    void parse_cmd_options();
};
}
#endif /* ROESTICALLER_H_ */
