#!/bin/bash

THRD=`cat /proc/cpuinfo  | grep "cpu cores" | uniq | cut -f 2 -d":" | sed -e 's/ //'`
PROC=`cat /proc/cpuinfo  | grep "physical id" | uniq | wc -l`

# Calc number of procs and threads
[ $PROC == 1 ] && PROC=2
THRD=$(( THRD / PROC ))

INPUT=data/test.5inds.8X.mpileup
REFERENCE=data/test.reference.fas
ORIGINAL=data/test.5inds.8X.mpileup.fas
ORIGINAL_WREFERENCE=data/test.5inds.8X.mpileup.outgroup.fas
OUTPUT=`mktemp`

function diff_check() {
	f1=$1
	f2=$2
	[ -z $f1 -o -z $f2 ] && echor "ERROR: run function with new and original files as params"
	diff -qs $f1 $f2 &>/dev/null
	if [ $? == 0 ] ; then
		echo "[ok]" 
	else
		echo "[ERROR]"
	fi
}

#
# Check without reference 
#
ARGS=""
echo "* Running pGHcaller using NP=$PROC and THRD=$THRD ARGS: $ARGS"
export OMP_NUM_THREADS=$THRD
mpirun -np $PROC ../src/pghcaller  -i $INPUT -o $OUTPUT $ARGS 
#&>/dev/null
echo -n "  Checking results: "
diff_check $OUTPUT $ORIGINAL

#
# Check with reference
#
ARGS="-O $REFERENCE"
echo "* Running pGHcaller using NP=$PROC and THRD=$THRD ARGS: $ARGS"
export OMP_NUM_THREADS=$THRD
mpirun -np $PROC ../src/pghcaller  -i $INPUT -o $OUTPUT $ARGS 
#&>/dev/null
echo -n "  Checking results: "
diff_check $OUTPUT $ORIGINAL_WREFERENCE

# Clean files
echo rm -f $OUTPUT

exit 0
