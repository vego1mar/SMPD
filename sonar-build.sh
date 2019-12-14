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
g++ -Wall -fexceptions -std=c++14 -g  -c src/classifiers.cpp -o sonar-build/classifiers.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/io_manager.cpp -o sonar-build/io_manager.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/casing.cpp -o sonar-build/casing.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers.cpp -o sonar-build/helpers.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/selectors.cpp -o sonar-build/selectors.o
mkdir sonar-build/matrix
g++ -Wall -fexceptions -std=c++14 -g  -c src/matrix/matrix.cpp -o sonar-build/matrix/matrix.o
RC=$?

if [ $RC -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "BUILD SUCCESS"
fi
