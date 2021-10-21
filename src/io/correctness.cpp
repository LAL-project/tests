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

// C++ includes
#include <iostream>
#include <fstream>
#include <set>

// lal includes
#include <lal/io/check_correctness.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace io {

err_type exe_io_correctness(const input_list& inputs, std::ifstream& fin) {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	const std::set<std::string> allowed_file_types{"treebank", "treebank_dataset"};

	std::string file_type;
	std::string file_path;

	while (fin >> file_type >> file_path) {
		if (allowed_file_types.find(file_type) == allowed_file_types.end()) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Wrong file type '" << file_type << "'.\n";
			std::cerr << "    Allowed file types:\n";
			for (const auto& v : allowed_file_types) {
			std::cerr << "    - " << v << '\n';
			}
			return err_type::test_format;
		}

		if (file_type == "treebank") {
			const auto errs = lal::io::check_correctness_treebank(file_path);
			std::cout << "A total of " << errs.size() << " errors were found for treebank file '"
				 << file_path << "'.\n";
			if (errs.size() > 0) {
				for (const auto& err : errs) {
					std::cout << "    * At line: " << err.get_line_number() << '\n';
					std::cout << "      What:    " << err.get_error_message() << '\n';
				}
			}
		}
		else {
			const auto errs = lal::io::check_correctness_treebank_collection(file_path);
			std::cout << "A total of " << errs.size() << " errors were found for treebank dataset '"
				 << file_path << "'.\n";
			if (errs.size() > 0) {
			for (const auto& err : errs) {
			std::cout << "    * Line within main file: " << err.get_line_within_main_file() << '\n';
			std::cout << "      At treebank file:      " << err.get_treebank_file_name() << '\n';
			std::cout << "      At treebank file line: " << err.get_treebank_file_line() << '\n';
			std::cout << "      What:                  " << err.get_error_message() << '\n';
			}
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace io
} // -- namespace tests
