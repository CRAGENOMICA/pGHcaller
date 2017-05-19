#!/bin/bash

TFILE=`mktemp`

mpirun -np 2 ../src/pghcaller -i data/head.mpileup  -o $TFILE

rm -f $TFILE
