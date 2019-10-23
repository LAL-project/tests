# Input files format

The general format of an input test file is the following

	TYPE type
	INPUT n input_file_1 format_1 ... input_file_n format_n
	BODY
		body

## TYPE

This field MUST be the first in the file and contains a single string, without spaces, with the tag of this test. This is the identifier of this type of test. For example, if we wanted to test the algorithm for path finding in unweighted undirected graphs between two vertices of the graph then we write

	TYPE numeric-rational

Notice that for new tests, the tester programs need to be modified so that they can interpret what comes next correctly.

Therefore, the _id_ of each test is made up of several "hyphenated" words. Each word represents a category. In the example above the test describes the execution of several arithmetic operations using "big ratinoal" values.

## INPUT

This field indicates the input files containing the necessary data to do the test. Usually, these files will contain graphs describde in a certain format. In order to indicate these input files we write:

- For three different input files

	INPUT 3 input_file_1 format_1 input_file_2 format_1 input_file_3

- For just one

	INPUT 1 input_file_1 format_1

The format of each file has to be given. The formats allowed are those supported by the library. See the documentation for details.

## BODY

This field contains the instructions necessary to perform the test. These instructions are dependent on the test and need not be instructions of an actual programming language, only something that the program running the test can understand. This field is allowed to be left empty if the test does not require any special instruction.
