#!/bin/bash

TFILE=`mktemp`

mpirun -np 3 ../src/pghcaller -i data/head100.mpileup  -o $TFILE

rm -f $TFILE
