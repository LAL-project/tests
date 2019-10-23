# Compilation of the tests (Windows)

The tests can also be compiled in ```RELEASE``` and in ```DEBUG``` mode. However, the tests must be compiled in the same mode in which the library was compiled.

## RELEASE mode

	cd tests; mkdir build-release; cd build-release
	cmake -G "MSYS Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ../src
	make -j 4

## DEBUG mode

	cd tests; mkdir build-debug; cd build-debug
	cmake -G "MSYS Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug ../src
	make -j 4
