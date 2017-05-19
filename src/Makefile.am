include $(top_srcdir)/common.am
SUFFIXES = .cpp
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
# bin_PROGRAMS = targetbinary1 targetbinary2 [...] targetbinaryN
# targetbinary1_SOURCES = targetbinary1.c myheader.h [...]
bin_PROGRAMS =  pghcaller 
pghcaller_SOURCES = 		\
	CmdArguments.cpp	\
	COptions.cpp		\
	pghcaller.cpp		\
	FastaFile.cpp        	\
	RoestiCaller.cpp	\
	MpileupParser.cpp	\
	PileupFile.cpp          \
	CFileWriter.cpp         \
	CFileWriterMPIIO.cpp	\
	MasterWorker.cpp	\
	CManager.cpp		\
	CAlgorithm.cpp		\
	CAlgorithmRoesti.cpp	\
	CAlgorithmDummy.cpp	\
	Logger.cpp


# MFastaFile.cpp 	 	\
#ngh.cpp            	
#CManager.cpp	
#MasterWorker.cpp	
#	CManager.cpp	 	
#	common.cpp  		

pghcaller_LDADD = -lstdc++
pghcaller_CFLAGS = -g -std=c++11 -O3

#bin_PROGRAMS =  test_cmd_arguments
#test_cmd_arguments_SOURCES = \
#	CmdArguments.cpp \
#	test_cmd_arguments.c 

#BUILT_SOURCES =   \
#	CmdArguments.cpp

#    check_PROGRAMS = test1 test2
#    TESTS = $(check_PROGRAMS)
#    
#    test1_SOURCES = test1.cc
#    test1_LDADD = libpmf.a -lm

