#!/bin/bash
echo "Copy to main.debug.cpp"
cp -f main.cpp main.debug.cpp
echo "Delete tagged DEBUG lines"
sed '/SED_DELETE_LINE/d' main.cpp
exit 0

