[ -z $1 ] && echo "ERR: No files provided" && exit 1
[ -z $2 ] && echo "ERR: You must provide a file to compare with" && exit 1

ERASE=0
DATA1=$1
DATA2=$2

if [ -z $3 ] ; then
	# check number of lines
	NL1=`cat $DATA1 | wc -l`
	NL2=`cat $DATA2 | wc -l`
	[ $NL1 != $NL2 ] && echo "ERR: File 1 has $NL1 lines, but  File 2 has $NL2" && exit 1
else
	NL1=$3; NL1=$((NL1*2))
	NL2=$NL1
fi


ISZERO=$((NL1%2))
[ $ISZERO != 0  ] && echo "ERR: A file with a odd number of lines is required" && exit 1 


for seq in `seq 2 2 $NL1`; do
	echo -n "Working with sequence $(( seq/ 2)): "
	head -n $seq $DATA1 | tail -n 1 > p1$seq
	head -n $seq $DATA2 | tail -n 1 > p2$seq
	diff -qs p1$seq p2$seq
	[ $ERASE -eq 1 ] && rm -f  p1$seq p2$seq 
done

