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

static constexpr auto no_error = lal::io::treebank_file_error_type::no_error;

err_type exe_io_correctness(std::ifstream& fin) noexcept {

	const std::set<std::string> allowed_file_types{"treebank", "treebank_dataset"};

	std::string file_type;
	std::string file_path;

	while (fin >> file_type >> file_path) {
		std::cout << "===============================\n";

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
			std::cout
				<< "A total of " << errs.get_num_errors()
				<< " errors were found for treebank file '"
				<< file_path
				<< "'.\n";
			if (errs.get_num_errors() > 0) {
				if (errs.get_treebank_error().get_error_type() != no_error) {
					std::cout
						<< "Problems with the treebank file: '"
						<< errs.get_treebank_error().get_error_message()
						<< "'.\n";
				}
				for (const auto& [line_number, error] : errs.get_head_vector_errors()) {
					std::cout << "    * At line: " << line_number << '\n';
					std::cout << "      What:    " << error.get_error_message() << '\n';
				}
			}
		}
		else {
			const auto errors = lal::io::check_correctness_treebank_collection(file_path);
			std::cout
				<< "A total of "
				<< errors.get_num_errors()
				<< " errors were found for treebank dataset '"
				<< file_path << "'.\n";

			if (errors.get_num_errors() > 0) {
				if (errors.get_treebank_error().get_error_type() != no_error) {
					std::cout
						<< "    "
						<< "Problems with the treebank file: '"
						<< errors.get_treebank_error().get_error_message()
						<< "'.\n";
				}

				for (const auto& [line_number, treebank_filename, _, report] : errors.get_treebank_reports()) {
					std::cout << "-------------------------------\n";
					std::cout << "    * Line within main file: " << line_number << '\n';
					std::cout << "      At treebank file:      " << treebank_filename << '\n';

					if (report.get_treebank_error().get_error_type() != no_error) {
						std::cout
							<< "    Problems with the treebank file: '"
							<< report.get_treebank_error().get_error_message()
							<< "'.\n";
					}
					for (const auto& [treebank_line_number, error] : report.get_head_vector_errors()) {
						std::cout << "        + At line: " << treebank_line_number << '\n';
						std::cout << "          What:    " << error.get_error_message() << '\n';
					}
				}
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace io
} // -- namespace tests
