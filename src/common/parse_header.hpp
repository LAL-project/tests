/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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

#pragma once

// C++ includes
#include <iostream>
#include <fstream>

// common includes
#include "common/definitions.hpp"

namespace tests {

template <typename ... Params>
err_type parse_header(
	err_type (*F)(const input_list&, std::ifstream&, Params...),

	std::ifstream& fin, Params... P
)
noexcept
{
	std::string field;
	fin >> field;

	if (field != "INPUT") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Expected field: INPUT\n";
		std::cerr << "    Found: " << field << '\n';
		return tests::err_type::test_format;
	}

	std::size_t n_inputs;
	fin >> n_inputs;
	input_list inputs(n_inputs);
	for (std::size_t i = 0; i < n_inputs; ++i) {
		std::string file, format;
		fin >> file >> format;
		inputs[i] = std::make_pair(file, format);
	}

	fin >> field;
	if (field != "BODY") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Expected field: BODY\n";
		std::cerr << "    Found: " << field << '\n';
		return tests::err_type::test_format;
	}

	return F(inputs, fin, P...);
}

void mark_wrong_keyword
(const std::vector<std::string>& keywords,
 const std::vector<std::size_t>& k, const std::string& tab)
{
	std::cerr << tab;
	// sure 'keywords' has at least one keyword
	for (std::size_t i = 0; i < keywords.size(); ++i) {
		if (keywords[i].length() >= 2) {
			std::cerr << keywords[i] << " ";
		}
		else {
			std::cerr << keywords[i] << "  ";
		}
	}
	std::cerr << '\n';

	// display the /\ where needed
	std::size_t it = 0;
	std::cerr << tab;
	for (std::size_t i = 0; i < keywords.size(); ++i) {
		std::size_t l = keywords[i].length();
		if (it < k.size()) {
			if (k[it] != i) {
				std::cerr << std::string(l, ' ') << " ";
				if (l < 2) { std::cerr << " "; }
			}
			else {
				std::cerr << "/\\";
				if (l > 2) {
					std::cerr << std::string(l - 2, ' ') << " ";
				}
				else {
					std::cerr << "   ";
				}
				++it;
			}
		}
	}
	std::cerr << '\n';
}

} // -- namespace tests
