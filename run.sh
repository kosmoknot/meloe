#!/bin/bash
if [ $1 == 'test' ]
then
    echo "Running tests"
    cd ./bin/test
    g++ -o test -Wall -I ../include test.cpp
    ./test
else
    echo "running meloe"
    cd ./bin
    make
    rm *.d
    ./MeLoE
fi