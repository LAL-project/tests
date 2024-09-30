# Executing the tests

Tests can be run by the user to ensure that the library works correctly in their system. After the library and the interface have been installed, the tests can be run. For automatic execution of the tests, the user needs a [Bash](https://www.gnu.org/software/bash/) interpreter. The coming instructions assume that such interpreter is installed in the user's system. Windows users are assumed to have a bash interpreter installed in their system. Besides,

- the user must have the tool [valgrind](http://valgrind.org/) installed in their system for memory leak testing.
- although this script is also tested in Windows, this test script was developed and tested under Ubuntu 18.04 using Bash version 4.4.19(1). There might be slight differences in behaviour.

## Automatic execution of the tests

Running the tests is simple. They have been classified into different categories, each stored in a different folder within the main directory [inputs](https://github.com/LAL-project/tests/tree/24.10/inputs). The tester executable is run using a given test, and its output should be compared to the corresponding output stored in the [outputs](https://github.com/LAL-project/tests/tree/24.10/outputs) directory. However, we provide a script that automatises this procedure. One can run it by issuing the command:

	./test.sh --input=i --output=o

where _i_ is a string denoting the directory with all the input tests that are to be executed and _o_ is the directory where all the correct outputs are stored. The full behaviour of the script can be checked at its usage by issuing the command

	./test.sh --help

Let's see an example. This command

	./test.sh --input=numeric/rational --output=numeric/rational

runs the tester executable with all the tests in the directory

	inputs/numeric/rational

and compares the output produced by the tester with the corresponding outputs in the directory

	outputs/numeric/rational

However, the user can also run all the tests one after the other with a single command

	./test.sh --all

In all the commands above, the script assumes that LAL is to be tested in ```DEBUG``` mode, and will use the _DEBUG_ compilation of the tests. In order to test the ```RELEASE``` compilation of LAL, using the ```RELEASE``` compilation of the tests, the user should issue the commands adding the flag ```--release```, like so:

	./test.sh --all --release

### Memory leak testing

If one wishes to, [LAL](https://github.com/LAL-project/linear-arrangement-library/tree/24.10/) can be tested for memory leaks. However, there are some steps that must be taken beforehand:

1. Compile and install the library in ```DEBUG``` mode as explained in the corresponding instructions file.

2. Compile the sources of the C++ tests in debug mode as explained in the corresponding instructions file.

Now, the user is ready to test for memory leaks:

	./test.sh --valgrind --debug --all

#### Known issues

There is at least one test that fails to execute successfully when being tested for memory leaks. This test is

	inputs/properties/exp-var-C/forests/test-0015
	
The reason it fails has nothing to do with our library since this leak it is related to a call to a function of the OpenMP library we use to paralelise one of our algorithms. This error happens in the function **lal::properties::get_variance_C_freqs_Q_rational**. Moreoever, we believe that it is very likely that the next test, i.e.,

	inputs/properties/exp-var-C/forests/test-0016
	
might also fail. However, when run without valgrind the test completes its execution successfully and all the functions compute their values correctly.
