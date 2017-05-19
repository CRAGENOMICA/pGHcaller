#!/bin/bash

make && \
time mpirun -np 2 ./src/pghcaller -v -s 100 -i tests/test.5inds.8X.mpileup -o /tmp/p_parser_old.fas 
diff  /tmp/p_parser_old.fas  tests/ghcaller.test.5inds.8X.ori.fas 

exit 0

