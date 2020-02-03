#!/bin/bash
#!/bin/rm
#!/bin/echo
#!/bin/mkdir
#!/usr/bin/g++
echo "REMOVE PREVIOUS BUILD"
rm -rf sonar-build

echo "BUILDING"
echo "./"
mkdir sonar-build
g++ -Wall -fexceptions -std=c++14 -g  -c src/casing.cpp -o sonar-build/casing.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/classifiers.cpp -o sonar-build/classifiers.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers.cpp -o sonar-build/helpers.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/io_manager.cpp -o sonar-build/io_manager.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/main.cpp -o sonar-build/main.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/selectors.cpp -o sonar-build/selectors.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/statistical.cpp -o sonar-build/statistical.o

echo "./command_line"
mkdir sonar-build/command_line
g++ -Wall -fexceptions -std=c++14 -g  -c src/command_line/cla_parser.cpp -o sonar-build/command_line/cla_parser.o

echo "./data_builders"
mkdir sonar-build/data_builders
g++ -Wall -fexceptions -std=c++14 -g  -c src/data_builders/csv_parser.cpp -o sonar-build/data_builders/csv_parser.o

echo "./helpers"
mkdir sonar-build/helpers
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers/collections.cpp -o sonar-build/helpers/collections.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers/combinations.cpp -o sonar-build/helpers/combinations.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers/converters.cpp -o sonar-build/helpers/converters.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers/stringify.cpp -o sonar-build/helpers/stringify.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/helpers/strings.cpp -o sonar-build/helpers/strings.o

echo "./io_manager"
mkdir sonar-build/io_manager
g++ -Wall -fexceptions -std=c++14 -g  -c src/io_manager/csv_reader.cpp -o sonar-build/io_manager/csv_reader.o
g++ -Wall -fexceptions -std=c++14 -g  -c src/io_manager/file_reader.cpp -o sonar-build/io_manager/file_reader.o

echo "./matrix"
mkdir sonar-build/matrix
g++ -Wall -fexceptions -std=c++14 -g  -c src/matrix/matrix.cpp -o sonar-build/matrix/matrix.o

echo "./selectors"
mkdir sonar-build/selectors
g++ -Wall -fexceptions -std=c++14 -g  -c src/selectors/fisher.cpp -o sonar-build/selectors/fisher.o

echo "./statistics"
mkdir sonar-build/statistics
g++ -Wall -fexceptions -std=c++14 -g  -c src/statistics/statistics.cpp -o sonar-build/statistics/statistics.o


RC=$?

if [ $RC -ne 0 ]; then
  echo "BUILD FAILURE"
else
  echo "BUILD SUCCESS"
fi
