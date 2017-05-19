#!/bin/env python

def readfile( fileName ):
    f = open( fileName )
    c = f.read()
    f.close()
    return c

def diff( s1, s2 ):
    counter=0
    for ch1, ch2 in zip( s1, s2 ):
        if not ch1 == ch2:
            break
        counter+=1
    return counter < len( s1 ) and counter or -1

import sys

f1 = readfile( sys.argv[1] )
f2 = readfile( sys.argv[2] )
pos = diff( f1, f2 )
end = pos+200

if pos >= 0:
    print "Different at:", pos
    print ">", f1[pos:end]
    print "<", f2[pos:end]

