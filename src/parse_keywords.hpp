/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
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

#pragma once

// C++ includes
#include <fstream>
#include <vector>
#include <string>

// custom includes
#include "definitions.hpp"

namespace exe_tests {

void mark_wrong_keyword(
	const std::vector<std::string>& keywords,
	const std::vector<size_t>& k,
	const std::string& tab = ""
);

/* FUNCTIONS FOR KEYWORD PARSING */

// parses the first keyword and calls the appropriate main type function
err_type call_main(const std::vector<std::string>& keywords, std::ifstream& fin);

	// Functions to test the integer and rational classes

err_type call_numeric
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

	// Functions to test the properties functions

err_type call_properties
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

	// Functions to test the linear arrangement-related functions

err_type call_linarr
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

err_type call_linarr_C
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

err_type call_linarr_klevel
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

err_type call_linarr_Dmin
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

	// Functions to test the generation functions and classes

err_type call_generate
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);
err_type call_generate_trees
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);
err_type call_generate_arrangements
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

	// Functions to test the library's utilities

err_type call_internal
(const std::vector<std::string>& keywords, size_t i, std::ifstream& fin);

} // -- namespace exe_tests
