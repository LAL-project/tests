# Test suite for LAL

The algorithms in this library are thoroughly tested during their development. It is our intention to continue extending the library, but also to make the algorithms more efficient every day. For this reason, we need automated tests that free us from the burden of carrying out the thousands of tests needed to be completely sure that the algorithms work every time we make the slightest change.

## Compilation of the C++ tests

We wrote different sets of instructions for compiling the tests for

- Windows in [this](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/compilation-tests-windows.md) file.
- Linux in [this](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/compilation-tests-linux.md) file.

## Graphs for testing

[This](https://github.com/lluisalemanypuig/LAL-tests/blob/master/graphs) directory contains several graphs used for testing.

## Compilation of the C++ tests

The compilation of the C++ tests and the execution of Python3 tests require the library and its dependencies to be installed in the system. Once this is done, the Python3 tester can be executed directly. However, the C++ tests need to be compiled first. We wrote different sets of instructions for this step for

- Windows in [this](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/installation-library-windows.md) file.
- Linux in [this](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/installation-library-linux.md) file.

## Execution of the tests

See [this](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/tests-instructions.md) file for instructions on how to execute the tests.

## Format of the tests

The way we test LAL in an automatic manner is explained [here](https://github.com/lluisalemanypuig/LAL-tests/blob/master/instructions/tests-format.md).
