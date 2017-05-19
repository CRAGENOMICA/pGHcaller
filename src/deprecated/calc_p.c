#include <iostream>
#include <cmath>  // lgamma, log
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#define ITERATIONS 10000000
#define p_err 0.001
#define SEED 12345
#define MAX_NUMBER 15

float calc_homo_p ( int n1, int n2, int n3, int n4 ){

    float lg1 = lgamma( n1 + 1 ) ;
    float lg2 = lgamma( n2 + 1 ) ;
    float lg3 = lgamma( n3 + 1 ) ;
    float lg4 = lgamma( n4 + 1 ) ;
    float lgt = lgamma( n1 + n2 + n3 + n4 + 1 );
    float res = (lgt-lg1-lg2-lg3-lg4+(n1)*log(1.0-3.0/4.0*p_err)+( n2 + n3 + n4 )*log( p_err /4.0));
    return res;

}

float calc_hetero_p ( int n1, int n2, int n3, int n4 ){
    float lg1 = lgamma( n1 + 1 );
    float lg2 = lgamma( n2 + 1 );
    float lg3 = lgamma( n3 + 1 );
    float lg4 = lgamma( n4 + 1 );
    float lgt = lgamma( n1 + n2 + n3 + n4 + 1 );
    float res = (lgt-lg1-lg2-lg3-lg4+(n1+n2)*log(0.5/2.0-p_err/4.0)+(n3+n4)*log(p_err/4.0));
    return res;

}


int main(void)
{
	//srand(time(NULL));
	srand(SEED);
	float res;
	int n1, n2, n3, n4;

	for(unsigned int i=0; i<ITERATIONS; i++)
	{
		 n1 = rand() % MAX_NUMBER;
		 n2 = rand() % MAX_NUMBER;
		 n3 = rand() % MAX_NUMBER;
		 n4 = rand() % MAX_NUMBER;
		 //printf("Number %d %d %d %d\n", n1, n2, n3, n4);
		 res = calc_homo_p ( n1, n2, n3, n4 ); 
		 res = calc_hetero_p ( n1, n2, n3, n4 ); 
	}	

	
	return(0);
}
