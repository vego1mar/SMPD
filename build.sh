#!/bin/bash
#!/bin/rm
#!/bin/mkdir
#!/usr/bin/g++
echo "REMOVE PREVIOUS BUILD"
rm -rf build

echo "BUILDING"
mkdir build
g++ -Wall -fexceptions -std=c++14 -g  -c src/main.cpp -o build/main.o
RC=$?

if [ $RC -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "BUILD SUCCESS"
fi
