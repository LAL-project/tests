# Compilation of the tests (Linux)

This library comes with a set of tests to make sure that there are no bugs in the code and that the library was compiled succesfully. The tests have to be compiled in the same the library in which it was compiled.

## RELEASE mode

	cd tests; mkdir build-release; cd build-release
	cmake ../src
	make -j 4

## DEBUG mode

	cd tests; mkdir build-debug; cd build-debug
	cmake -DCMAKE_BUILD_TYPE=Debug ../src
	make -j 4
