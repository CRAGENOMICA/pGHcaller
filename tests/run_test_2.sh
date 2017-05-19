#!/bin/bash

TFILE=`mktemp`

mpirun -np 3 ../src/pghcaller -i data/test.5inds.8X.mpileup  -o $TFILE
diff -qs $TFILE data/pghcaller.test.5inds.8X.fas

rm -f $TFILE
