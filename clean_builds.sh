#!/bin/bash

for d in build-release build-debug build-release-11.3 build-debug-11.3 build-release-12.3 build-debug-12.3 build-release-13.1 build-debug-13.1; do
	cd $d
	make clean
	cd ..
done
