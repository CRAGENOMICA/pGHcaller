/*
 *  RoestiCaller.cpp
 *
 *  Author: javier.navarro@caos.uab.es
 *  Copyright (c) 2015 Javier Navarro, GNU license.
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

#include "RoestiCaller.h"

namespace ngh {

const char RoestiCaller::HAPLOTYPES[] = {'A', 'C', 'G', 'T'};
const char RoestiCaller::GENOTYPES_PLOYDI_0[] = {'A', 'A', 'A', 'A', 'C', 'C', 'C', 'G', 'G', 'T'};
const char RoestiCaller::GENOTYPES_PLOYDI_1[] = {'A', 'C', 'G', 'T', 'C', 'G', 'T', 'G', 'T', 'T'};
unsigned int RoestiCaller::minreads = ROESTI_DEFAULT_MINREADS;
float RoestiCaller::p_err           = ROESTI_DEFAULT_P_ERR;
float RoestiCaller::chivalue        = ROESTI_DEFAULT_CHIVALUE;
bool RoestiCaller::haplocalls       = ROESTI_DEFAULT_HAPLOCALLS;
float RoestiCaller::log_value_p_err1 = log(1.0-3.0/4.0*p_err);
float RoestiCaller::log_value_p_err2 = log(p_err/4.0);
float RoestiCaller::log_value_p_err3 = log(0.5/2.0-p_err/4.0);

void RoestiCaller::set_mread(int mreads) {
    minreads = mreads;
}

void RoestiCaller::set_error(float err) {
    p_err = err;
    log_value_p_err1 = log(1.0-3.0/4.0*p_err);
    log_value_p_err2 = log(p_err/4.0);
    log_value_p_err3 = log(0.5/2.0-p_err/4.0);
}

void RoestiCaller::set_chi(float chi) {
    chivalue = chi;
}

void RoestiCaller::set_haplocalls(bool haplo) {
    haplocalls = haplo;
}

void RoestiCaller::setDefaults() {
    minreads   = ROESTI_DEFAULT_MINREADS;
    p_err      = ROESTI_DEFAULT_P_ERR;
    chivalue   = ROESTI_DEFAULT_CHIVALUE;
    haplocalls = ROESTI_DEFAULT_HAPLOCALLS;
    // TODO(jnavarro) Why 1024 here? Adjust number to MAX
    for (unsigned int i=0;  i < MAXLUT; i++) {
        LUT_lgamma[i] = lgamma(i);
    }
    log_value_p_err1 = log(1.0-3.0/4.0*p_err);
    log_value_p_err2 = log(p_err/4.0);
    log_value_p_err3 = log(0.5/2.0-p_err/4.0);
}

RoestiCaller::RoestiCaller() {
    // logger = ngh::Logger::getInstance();
    setDefaults();
}

RoestiCaller::RoestiCaller(int mreads, float err, float chi, bool haplo ) {
    // logger = ngh::Logger::getInstance();
    setDefaults();
    minreads = mreads;
    p_err = err;
    chivalue = chi;
    haplocalls = haplo;
}

void
RoestiCaller::process(const SeqProperties* properties, const std::vector<ngh::Site>* sites, ngh::CBasecalls *calls) {
	ROESTI_MSG("Processing sites " << sites->size());
	float ps[DEFAULT_PS_SIZE];	// used to store probs from calc_genotypes (10)
	int32_t best_genotype, best_haplo;
	uint32_t pos = 0;
	ROESTI_MSG("ROESTI sites " << sites->size());
	for (auto site = sites->begin(); site != sites->end(); site++, pos++) {
		ROESTI_MSG("   Site reads/individuals: " << (*site).size());
		ROESTI_MSG("   Site pos: " << pos);
		ROESTI_MSG("ROESTI site " << site->site);
		for(uint32_t i = 0; i < properties->individuals; ++i) {
			// ROESTI_MSG("Read: " << site->reads.at(i));
		    // By default called_genotypes are 'NN'
			// Genotype Call
		    ROESTI_MSG("       ind " << i << " size " << site->reads.at(i).size());
			if ( site->reads.at(i).size() >= minreads ){
				ROESTI_MSG("Attempt Genotype call : minreads " << minreads);
				calc_genos_probs (site->reads.at(i), ps);
				// ROESTI_MSG( "Calls " << calls->size() << " ," << calls[i].called_genotypes.size());
				best_genotype = lrt_genotypes( ps );
				ROESTI_MSG( "Genotype call : best genotype " << best_genotype);
				// Try genotype, if result is NOT LRT_NOT_SIGNIFICANT, do genotype call
                if( best_genotype != LRT_NOT_SIGNIFICANT) {
                    calls->called_genotypes[properties->ploidy*i+0][pos] = GENOTYPES_PLOYDI_0[best_genotype];
                    calls->called_genotypes[properties->ploidy*i+1][pos] = GENOTYPES_PLOYDI_1[best_genotype];
                // If is LRT_NOT_SIGNIFICANT do haplotype call if is enable
                } else if(haplocalls) {
                    // Result is not significant. Attempt a haplotype call
                    ROESTI_MSG( "Genotype call failed! Attempt Haplocall : best haplotype " << best_haplo);
                    best_haplo = lrt_haplotypes(ps);
                    // if LRT_NOT_SIGNIFICANT the called genotypes are NN
                    if( best_haplo != LRT_NOT_SIGNIFICANT) {
                        // Char array has a size of 4 max.
                        calls->called_genotypes[properties->ploidy*i+0][pos] = HAPLOTYPES[best_haplo];
                        // This is N by default
                        // calls->called_genotypes[properties->ploidy*i+1][pos] = HAPLOTYPES[best_haplo];
                    }
                }
				/*
				// Try genotype, if result is LRT_NOT_SIGNIFICANT, do haplotype call
				if( best_genotype == LRT_NOT_SIGNIFICANT) {
					// Result is not significant. Attempt a haplotype call
				    ROESTI_MSG( "Genotype call failed! Attempt Haplocall : best haplotype " << best_haplo);
					best_haplo = lrt_haplotypes(ps);
					// if LRT_NOT_SIGNIFICANT the called genotypes are NN
					if( best_haplo != LRT_NOT_SIGNIFICANT) {
					    // Char array has a size of 4 max.
					    calls->called_genotypes[properties->ploidy*i+0][pos] = HAPLOTYPES[best_haplo];
					    // This is N by default
					    // calls->called_genotypes[properties->ploidy*i+1][pos] = HAPLOTYPES[best_haplo];
					}
				} else {
					// Char array has a size of 10 max.
					// assert(best_genotype <= 9);
					calls->called_genotypes[properties->ploidy*i+0][pos] = GENOTYPES_PLOYDI_0[best_genotype];
					calls->called_genotypes[properties->ploidy*i+1][pos] = GENOTYPES_PLOYDI_1[best_genotype];
				}
				*/

			// Haplotype call
			} else if (site->reads.at(i).size() > 1 && haplocalls) {
				ROESTI_MSG( "Attempt Haplotype call  : minreads " << minreads);
				calc_genos_probs (site->reads.at(i), ps);
				// ROESTI_MSG( "Calls " << calls->size() << " ," << calls[i].called_genotypes.size());
				best_haplo = lrt_haplotypes(ps);
				ROESTI_MSG( "Haplocall : best haplotype " << best_haplo);
				if( best_haplo != LRT_NOT_SIGNIFICANT) {
					// Char array has a size of 4 max.
					// assert(best_haplo <= 3);
					calls->called_genotypes[properties->ploidy*i+0][pos] = HAPLOTYPES[best_haplo];
				}
			}
			// ROESTI_MSG("CALLED " << properties->ploidy*i << ": " << calls->called_genotypes[properties->ploidy*i+0]);
		}
	}
}

GHCALLER_INLINE int RoestiCaller::lrt_genotypes ( const float ps[] ) {
	// returns index of most likely genotype (in AA, AC, AG, AT, CC, CG, CT, GG, GT, TT)
	// -1 LRT between 2 most likely genotypes is not significant

	int maxGL = 0;   // Max value
	int smaxGL = 0;  // Sub max. Ancestor.

	// Search max
	for (int i = 0; i<DEFAULT_PS_SIZE; i++) {
		if(ps[i] > ps[maxGL] ) {
			maxGL = i;
		}
	}

	// Search sub max
	smaxGL = (maxGL+1)%10;   // Set different index. Max index cannot be equal to smax index
	for (int i = 0; i<DEFAULT_PS_SIZE; i++) {
		//LOG_("SMAXGL test" << ps[i] << " >? " << ps[smaxGL] );
		if(ps[i] > ps[smaxGL] &&  i != maxGL) {
			smaxGL = i;
		}
	}
	float lrt = abs(2 * ( ps[maxGL] - ps[smaxGL]));
	if (lrt < chivalue) {
		return LRT_NOT_SIGNIFICANT;
    }
	return(maxGL);
}

GHCALLER_INLINE int
RoestiCaller::lrt_haplotypes ( const float ps[] ) {
    /*
     * Forget and make things easy
	float sps[4] = { ps[0], ps[4], ps[7], ps[9]};
	int m1 = (sps[0]>sps[1])? 0 : 1;
	int m2 = (sps[2]>sps[3])? 2 : 3;
	int maxGL = (sps[m1]>sps[m2])? m1 : m2;
	int smaxGL = (sps[m1]>sps[m2])? m2 : m1;

	LOG_("PS HAPLO " << sps[0] << " " << sps[1] << " " << sps[2] << " " << sps[3]);
	LOG_("PS HAPLO " << m1 << " " << m2 << " " << maxGL << " " << smaxGL);
    float lrt = std::fabs( 2 * ( sps[maxGL] - sps[smaxGL]) );
    LOG_("PS HAPLO SMAXGL NEW " << sps[maxGL] << " " << sps[smaxGL] );
    if(lrt < chivalue){
        return LRT_NOT_SIGNIFICANT;
    }
    return(maxGL);
     */

    // returns index of most likely haplotype (in AA, CC, GG, TT)
    // or -1 if LRT between 2 most likely haplotypes is not significant
    float sps[4] = { ps[0], ps[4], ps[7], ps[9]};
    int maxGL = 0;   // Max value
    int smaxGL = 0;  // Sub max. Ancestor.

    // Search max
    for (int i = 0; i<4; i++) {
        if (sps[i] > sps[maxGL]) {
            maxGL = i;
        }
    }
    // Search sub max
    smaxGL = (maxGL+1)%4;   // Set different index. Max index cannot be equal to smax index
    for (int i = 0; i<4; i++) {
        if(sps[i] > sps[smaxGL] &&  i != maxGL) {
            smaxGL = i;
        }
    }
    //LOG_("PS HAPLO " << sps[0] << " " << sps[1] << " " << sps[2] << " " << sps[3]);
    //LOG_("PS HAPLO SMAXGL NEW " << sps[maxGL] << " " << sps[smaxGL] << " indx " << smaxGL );
    float lrt = abs( 2 * ( sps[maxGL] - sps[smaxGL]) );
    if( lrt < chivalue ) {
        return LRT_NOT_SIGNIFICANT;
    }
    return(maxGL);
}


GHCALLER_INLINE void
RoestiCaller::calc_genos_probs ( std::string reads, float ps[] ){

    // DNAbases = { 'a', 'A', 'c', 'C', 'g', 'G', 't', 'T' };
    int fA = 0;
    int fC = 0;
    int fG = 0;
    int fT = 0;

    // count each base in forward and reverse strands
    for (unsigned int each_read = 0; each_read < reads.size();each_read++) {
        // AA, AC, AG, AT, CC, CG, CT, GG, GT, TT, for each ind
        switch (reads[each_read]) {
        case 'a': case 'A':
            fA++;
            break;
        case 'c': case 'C':
            fC++;
            break;
        case 'g': case 'G':
            fG++;
            break;
        case 't': case 'T':
            fT++;
            break;
        default:
            ERR_("This read cannot be parsed " << reads[each_read] );
            exit(EXIT_FAILURE);
        }
    }

    // calculates GLs for AA, AC, AG, AT, CC, CG, CT, GG, GT, TT
    ps[0] = calc_homo_p   (fA, fC, fG, fT);
    ps[1] = calc_hetero_p (fA, fC, fG, fT);
    ps[2] = calc_hetero_p (fA, fG, fC, fT);
    ps[3] = calc_hetero_p (fA, fT, fC, fG);
    ps[4] = calc_homo_p   (fC, fA, fG, fT);
    ps[5] = calc_hetero_p (fC, fG, fA, fT);
    ps[6] = calc_hetero_p (fC, fT, fG, fA);
    ps[7] = calc_homo_p   (fG, fA, fC, fT);
    ps[8] = calc_hetero_p (fG, fT, fC, fA);
    ps[9] = calc_homo_p   (fT, fA, fG, fC);
    // ROESTI_MSG("FA " << fA << " FC " <<  fC << "FG " <<  fG << " FT " << fT );
}

GHCALLER_ALLWAYS_INLINE float
RoestiCaller::calc_homo_p ( const int& n1, const int& n2, const int& n3, const int& n4 ) {
	/*
	float lg1 = lgamma( n1 + 1 ) ;
    float lg2 = lgamma( n2 + 1 ) ;
    float lg3 = lgamma( n3 + 1 ) ;
    float lg4 = lgamma( n4 + 1 ) ;
    float lgt = lgamma( n1 + n2 + n3 + n4 + 1 );
	 */
    float res = (
            LUT_lgamma[ n1 + n2 + n3 + n4 + 1 ]
            - LUT_lgamma[ n1 + 1 ]
            - LUT_lgamma[ n2 + 1 ]
            - LUT_lgamma[ n3 + 1 ]
            - LUT_lgamma[ n4 + 1 ]
            + (n1) * log_value_p_err1 + ( n2 + n3 + n4 ) * log_value_p_err2);
    //float res = (lgt-lg1-lg2-lg3-lg4+(n1)*log(1.0-3.0/4.0*p_err)+( n2 + n3 + n4 )*log( p_err /4.0));
    return res;
}

GHCALLER_ALLWAYS_INLINE float
RoestiCaller::calc_hetero_p ( const int& n1, const int& n2, const int& n3, const int& n4 ) {
    /*
	float lg1 = lgamma( n1 + 1 );
    float lg2 = lgamma( n2 + 1 );
    float lg3 = lgamma( n3 + 1 );
    float lg4 = lgamma( n4 + 1 );
    float lgt = lgamma( n1 + n2 + n3 + n4 + 1 );
    float res = (lgt-lg1-lg2-lg3-lg4+(n1+n2)*log(0.5/2.0-p_err/4.0)+(n3+n4)*log(p_err/4.0));
     */
    // LOG_("XXXX " << n1 << " " << n2 << " " <<  n3 << " "<< n4 << " sum n3 n4 " << n3 + n4 << " sum " << n1 + n2 + n3 + n4 + 1);
    float res = (
            LUT_lgamma[ n1 + n2 + n3 + n4 + 1 ]
            - LUT_lgamma[ n1 + 1 ]
            - LUT_lgamma[ n2 + 1 ]
            - LUT_lgamma[ n3 + 1 ]
            - LUT_lgamma[ n4 + 1 ]
            +(n1+n2) * log_value_p_err3 + ( n3 + n4 ) * log_value_p_err2);
    return res;
}
}
