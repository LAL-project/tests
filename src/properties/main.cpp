/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library.
 *
 *  Tests of the Linear Arrangement Library is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU Affero
 *  General Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later version.
 *
 *  Tests of the Linear Arrangement Library is distributed in the hope
 *  that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Tests of the Linear Arrangement Library.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C includes
#include <string.h>

// C++ includes
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// common includes
#include "common/definitions.hpp"
#include "properties/parse_keywords.hpp"
using namespace tests;

err_type get_type_keyword(const std::string& filename, std::ifstream& fin, std::string& type) {
	std::string field;
	fin >> field;

	if (field != "TYPE") {
		std::cerr << ERROR << '\n';
		std::cerr << "    In input test file '" << filename << "'.\n";
		std::cerr << "    First field is not 'TYPE'.\n";
		std::cerr << "    Field found: '" << field << "'.\n";
		return err_type::test_format;
	}

	fin >> type;
	return err_type::no_error;
}

void get_keywords(const std::string& type, std::vector<std::string>& keywords) {
	bool finish = false;

	std::size_t dash_pos = 0;
	do {
		std::size_t new_dash_pos = type.find("-", dash_pos);
		if (new_dash_pos != std::string::npos) {
			std::string keyword = type.substr(dash_pos, new_dash_pos - dash_pos);
			keywords.push_back(keyword);
			dash_pos = new_dash_pos + 1;
		}
		else {
			std::string keyword = type.substr(dash_pos, type.length() - dash_pos);
			keywords.push_back(keyword);
			finish = true;
		}
	}
	while (not finish);
}

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
	err_type r = get_type_keyword(input_file, fin, type);

	if (r == err_type::test_format) {
		std::cerr << "***********************\n";
		std::cerr << "Exiting with error type: test_format_error\n";
		return 1;
	}

	std::vector<std::string> keywords_type;
	get_keywords(type, keywords_type);

	r = tests::properties::call_main(keywords_type, fin);

	if (r != err_type::no_error) {
		std::cerr << "***********************\n";
		std::cerr << "Exiting with error type: ";
		switch (r) {
		case err_type::io: std::cerr << "io_error"; break;
		case err_type::test_execution: std::cerr << "test_execution"; break;
		case err_type::invalid_param: std::cerr << "invalid_param"; break;
		case err_type::wrong_keyword: std::cerr << "wrong_keyword"; break;
		case err_type::not_implemented: std::cerr << "not_implemented"; break;
		case err_type::test_format: std::cerr << "test_format"; break;
		case err_type::graph_format: std::cerr << "graph_format"; break;
		case err_type::too_many_keywords: std::cerr << "too_many_keywords"; break;
		default:
			;
		}

		std::cerr << '\n';
		return 1;
	}

	return 0;
}
