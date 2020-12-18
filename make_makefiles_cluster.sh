#!/bin/bash

# ----------------------------------------------------------------------
mkdir -p build-release && cd build-release
qmake -makefile -o Makefile "ENVIR=CLUSTER" ../src/tests.pro
cd ..

# ----------------------------------------------------------------------
mkdir -p build-debug && cd build-debug
qmake -makefile -o Makefile "ENVIR=CLUSTER" CONFIG+=debug ../src/tests.pro
cd ..
