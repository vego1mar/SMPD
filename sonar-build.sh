#!/bin/bash
#!/bin/rm
#!/bin/echo
#!/bin/mkdir
#!/usr/bin/g++
echo "REMOVE PREVIOUS BUILD"
rm -rf sonar-build

echo "BUILDING"
mkdir sonar-build
g++ -Wall -fexceptions -std=c++14 -g  -c src/main.cpp -o sonar-build/main.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/statistical.cpp -o sonar-build/statistical.o
RC=$?

if [ $RC -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "BUILD SUCCESS"
fi
