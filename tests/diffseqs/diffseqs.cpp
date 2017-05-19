#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


int main ( int argc, char *argv[] ) {
	if ( argc != 5 ) {
		cout << "usage: " << argv[0] << " <seq1> <seq2> <seq3> <seq4>" << std::endl;
		exit(EXIT_FAILURE);
	} /*else {
	    for (int n = 1; n < argc; n++)
	    cout << "sequence " << n << ": " << argv[ n ] << std::endl;
	    }*/

	// We assume argv[1] is a filename to open
	ifstream seq11(argv[1]);
	ifstream seq12(argv[2]);
	ifstream seq21(argv[3]);
	ifstream seq22(argv[4]);

	// Always check to see if file opening succeeded
	if( !( seq11.is_open() && seq12.is_open() && seq21.is_open() && seq22.is_open()) ) {
		cout<<"Could not open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	unsigned long diffs = 0;
	unsigned long nchar = 1;  
	char c11, c12, c21, c22, c1b = NULL, c2b = NULL;
	while (seq11.get(c11) && seq12.get(c12) && seq21.get(c21) && seq22.get(c22)) {
		// All is different
		if( (c11!=c21) && (c12!=c22) ) {
			cout << "Locus: " << nchar << std::endl;
			cout << " " << c1b << "*" << c11 << c12 << "*" << std::endl;
			cout << " " << c2b << "*" << c21 << c22 << "*" << std::endl;
			cout << "---" << std::endl;
			diffs=diffs+2;
		}
		// Check seq1_1 between seq2_1
		else if(c11!=c21) {
			cout << "Locus: " << nchar << std::endl;
			cout << " " << c1b << "*" << c11 << "*" << c12 << std::endl;
			cout << " " << c2b << "*" << c21 << "*" << c22 << std::endl;
			cout << "---" << std::endl;
			diffs++;
		}
		// Check seq1_2 between seq2_2
		else if(c12!=c22) {
			cout << "Locus: " << nchar << std::endl;
			cout << " " << c1b << c11 << "*" << c12 << "*" << std::endl;
			cout << " " << c2b << c21 << "*" << c22 << "*" << std::endl;
			cout << "---" << std::endl;
			diffs++;
		}
		c1b = c11; c2b = c21;
		++nchar;
	}

	if (diffs == 0 ) {
		cout << "OK: All sequences in these files are identical." << std::endl;
	} else {
		cout << "KO: we have found " << diffs << " differences." << std::endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}


