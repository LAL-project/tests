/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

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
