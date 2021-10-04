#!/bin/bash
if [ $1 == 'test' ]
then
    echo "Running tests"
    g++ -o test -Wall -I ./bin/include/ ./bin/tests/test.cpp -I ./bin/obj/*.o
    ./test
else
    echo "running meloe"
    cd ./bin
    make
    rm *.d
    rm ./site/*.html
    rm ./site/wikies/*.html
    ./MeLoE
fi