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

// common includes
#include "common/definitions.hpp"

namespace tests {

input_list read_input_list(std::ifstream& fin) noexcept
{
	{
		std::string INPUT;
		fin >> INPUT;
		if (INPUT != "INPUT" and INPUT != "INPUTS") {
			std::cerr << ERROR << '\n';
			std::cerr << "    INPUT keyword is neither 'INPUT' or 'INPUTS'.\n";
			std::cerr << "    INPUT: '" << INPUT << "'.\n";
			return {};
		}
	}

	std::size_t n_inputs;
	fin >> n_inputs;

	input_list inputs(n_inputs);
	for (std::size_t i = 0; i < n_inputs; ++i) {
		fin >> inputs[i].first >> inputs[i].second;
	}
	return inputs;
}

void mark_wrong_keyword(
	const std::vector<std::string>& keywords,
	const std::vector<std::size_t>& k,
	const std::string& tab
) noexcept
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
				if (l < 2) {
					std::cerr << " ";
				}
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

} // namespace tests
