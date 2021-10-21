// C includes
#include <stdlib.h>
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>

// common includes
#include "common/definitions.hpp"
#include "common/parse_keywords.hpp"

int main(int argc, char *argv[]) {
	// error checking
	if (argc == 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No arguments given. Use\n";
		std::cerr << "        ./tests -i\n";
		std::cerr << "        ./tests --input\n";
		std::cerr << "    to specify an input test file.\n";
		return 1;
	}
	if (argc > 3) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Too many arguments given. Use\n";
		std::cerr << "        ./tests -i\n";
		std::cerr << "        ./tests --input\n";
		std::cerr << "    to specify an input test file.\n";
		return 1;
	}
	if (strcmp(argv[1], "-i") != 0 and strcmp(argv[1], "--input") != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unrecognised parameter '" << std::string(argv[1]) << "'. Use\n";
		std::cerr << "        ./tests -i\n";
		std::cerr << "        ./tests --input\n";
		std::cerr << "    to specify an input test file.\n";
		return 1;
	}

	const std::string input_file(argv[2]);
	if (not std::filesystem::exists(input_file)) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Input test '" << input_file << "' does not exist.\n";
		return 1;
	}

	std::ifstream fin;
	fin.open(input_file.c_str());

	std::string type;
	const tests::err_type r = tests::get_type_keyword(input_file, fin, type);
	fin.close();

	if (r == tests::err_type::test_format) {
		std::cerr << "***********************\n";
		std::cerr << "Exiting with error type: test_format_error\n";
		return 1;
	}

	const std::vector<std::string> keywords_type = tests::get_keywords(type);
	const std::string call_to_exe =
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
