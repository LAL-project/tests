#!/bin/bash

function make_single {
	what=$1
	DEBCONF=$2
	
	mkdir -p $what && cd $what
	echo "    $PWD"
	qmake -makefile -o Makefile "ENVIR=CLUSTER" $DEBCONF ../../src/$what/$what.pro
	cd ..
}

function make_makefiles {
	if [ ! -z $1 ]; then
		declare DEBCONF=$1
	fi

	echo "    $PWD"
	qmake -makefile -o Makefile "ENVIR=CLUSTER" $DEBCONF ../src/tests.pro

	make_single common $DEBCONF

	make_single detail $DEBCONF

	make_single generate $DEBCONF

	make_single graphs $DEBCONF

	make_single io $DEBCONF

	make_single linarr $DEBCONF

	make_single memory $DEBCONF

	make_single numeric $DEBCONF

	make_single properties $DEBCONF

	make_single tests $DEBCONF

	make_single utilities $DEBCONF
}

# ----------------------------------------------------------------------
# RELEASE BUILD
echo "Release build..."
mkdir -p build-release && cd build-release
make_makefiles
cd ..

# ----------------------------------------------------------------------
# DEBUG BUILD
echo "Debug build..."
mkdir -p build-debug && cd build-debug
make_makefiles "CONFIG+=debug"
cd ..
