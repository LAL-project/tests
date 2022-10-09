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

// C++ includes
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// common includes
#include "common/definitions.hpp"

namespace tests {

err_type get_type_keyword(const std::string& filename, std::ifstream& fin, std::string& type)
noexcept
{
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

std::vector<std::string> get_keywords(const std::string& type) noexcept {
	std::vector<std::string> keywords;
	bool finish = false;

	std::size_t dash_pos = 0;
	do {
		std::size_t new_dash_pos = type.find("-", dash_pos);
		if (new_dash_pos != std::string::npos) {
			keywords.push_back(type.substr(dash_pos, new_dash_pos - dash_pos));
			dash_pos = new_dash_pos + 1;
		}
		else {
			keywords.emplace_back(type.substr(dash_pos, type.length() - dash_pos));
			finish = true;
		}
	}
	while (not finish);

	return keywords;
}

} // -- namespace tests
