#!/bin/bash


# [jnavarro@devgpu sources]$ cat ../examples/test.5inds.8X.mpileup | ./GHcaller -mindep 3,3,3,3,3 -maxdep 100,100,100,100,100 -platform 33,33,33,33,33 -baseq 20,20,20,20,20 -minreads 6,6,6,6,6 -outfile ../../pdGHcaller/examples/ghcaller.test.5inds.8X.ori.fas 
# <GHcaller> v 0.0.1 05122013, waiting for input from STDIN...
# <GHcaller> Num inds: 5, mindeps: 3 3 3 3 3, maxdeps: 100 100 100 100 100, platforms: 33 33 33 33 33, baseqs: 20 20 20 20 20
# <GHcaller> SNP calling algorithm: Roesti, minreads for genotype: 6, error: 0.01, LRT chi-square threshold: 3.84146, haplotype calls: enabled
# <GHcaller> Output format: fasta, using unphased haplotypes, output file: ../../pdGHcaller/examples/ghcaller.test.5inds.8X.ori.fas
# <GHcaller> Writing window 1 to file ../../pdGHcaller/examples/ghcaller.test.5inds.8X.ori.fas, region:1-99992

WIDTH=80

if [ -z $1 ]; then
	cat ../examples/ghcaller.test.5inds.8X.ori.fas  | fold -w $WIDTH | head -n $1 > ori
	cat pepe.fas   | fold -w $WIDTH | head -n $1 > p
else
	cat ../examples/ghcaller.test.5inds.8X.ori.fas  | fold -w $WIDTH > ori
	cat  pepe.fas   | fold -w $WIDTH > p
fi

diff ori p
