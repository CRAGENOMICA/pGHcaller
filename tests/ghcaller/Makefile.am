include $(top_srcdir)/common.am

SUFFIXES = .cpp
check_PROGRAMS = ghcaller

# g++ -Wall -g -std=c++0x -O3  *.cpp -o GHcaller
ghcaller_CFLAGS = -Wall -g -std=c++0x -O3   
ghcaller_SOURCES =		\
	common.cpp  	\
	ngh.cpp 	\
	main.cpp 	\
	fasta.cpp 	
