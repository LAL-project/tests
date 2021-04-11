// C includes
#include <stdlib.h>
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// common includes
#include "common/definitions.hpp"
#include "common/parse_keywords.hpp"
using namespace tests;

int main(int argc, char *argv[]) {
	// error checking
	if (argc == 1) {
		cerr << ERROR << endl;
		cerr << "    No arguments given. Use" << endl;
		cerr << "        ./tests -i" << endl;
		cerr << "        ./tests --input" << endl;
		cerr << "    to specify an input test file." << endl;
		return 1;
	}
	if (argc > 3) {
		cerr << ERROR << endl;
		cerr << "    Too many arguments given. Use" << endl;
		cerr << "        ./tests -i" << endl;
		cerr << "        ./tests --input" << endl;
		cerr << "    to specify an input test file." << endl;
		return 1;
	}
	if (strcmp(argv[1], "-i") != 0 and strcmp(argv[1], "--input") != 0) {
		cerr << ERROR << endl;
		cerr << "    Unrecognised parameter '" << string(argv[1]) << "'. Use" << endl;
		cerr << "        ./tests -i" << endl;
		cerr << "        ./tests --input" << endl;
		cerr << "    to specify an input test file." << endl;
		return 1;
	}

	const string input_file(argv[2]);
	if (not filesystem::exists(input_file)) {
		cerr << ERROR << endl;
		cerr << "    Input test '" << input_file << "' does not exist." << endl;
		return 1;
	}

	ifstream fin;
	fin.open(input_file.c_str());

	string type;
	const err_type r = get_type_keyword(input_file, fin, type);
	fin.close();

	if (r == err_type::test_format) {
		cerr << "***********************" << endl;
		cerr << "Exiting with error type: test_format_error" << endl;
		return 1;
	}

	const vector<string> keywords_type = get_keywords(type);
	const string call_to_exe =
#if defined DEBUG
		"build-debug/"
#else
		"build-release/"
#endif
		+ keywords_type[0] + "/" + keywords_type[0] + " -i " + input_file;

	const int errcode = system(call_to_exe.c_str());
	(void)(errcode);

	return 0;
}
