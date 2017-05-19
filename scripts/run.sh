#!/bin/bash

# Original
# /scratch/034-MKT/jnavarro/software/GHcaller-v0.1 -outfile data18.543785.fas.0 -maxdep 12,21,21,19,15,19 -mindep 4 -baseq 20 -platform 33,33,33,33,33,33 -reference /scratch/034-MKT/jnavarro/GenoReference/reference.chr18.fa -names LW22F01,LW22F02,LW22F03,LR24F08,LR30F02,LR21M03

#make ghcaller 

#./GHcaller -outfile data18.543785.fas.0 -maxdep 12,21,21,19,15,19 -mindep 4,4,4,4,4,4 -baseq 20,20,20,20,20,20 -platform 33,33,33,33,33,33 -reference ./reference.fa -names LW22F01,LW22F02,LW22F03,LR24F08,LR30F02,LR21M03 | tr "\t" " "

case $2 in

	1)
	mpirun -np $1 ./pGHcaller -chunksize 2 -infile ../examples/test.5inds.8X.mpileup -outfile pepe.fas -maxdep 100,100,100,100,100 -mindep 3,3,3,3,3 -baseq 20,20,20,20,20 -platform 33,33,33,33,33  -names LW22F01,LW22F02,LW22F03,LR24F08,LR30F02 -numind 5
	;;

	2)
	mpirun -np $1 ./pGHcaller  -infile ./data2.mpileup -outfile pepe.fas -numind 5 
# -maxdep 12,21,21,19,15 -mindep 4,4,4,4,4 -baseq 20,20,20,20,20 -platform 33,33,33,33,33  -names LW22F01,LW22F02,LW22F03,LR24F08,LR30F02 
	;;
	
	3)
	mpirun -np $1 ./pGHcaller -infile ./data.mpileup -outfile pepe -maxdep 12,21,21,19,15,19 -mindep 4,4,4,4,4,4 -baseq 20,20,20,20,20,20 -platform 33,33,33,33,33,33 -reference ./reference.fa -names LW22F01,LW22F02,LW22F03,LR24F08,LR30F02,LR21M03 -numind 5
	;;

	*)
	;;
esac


