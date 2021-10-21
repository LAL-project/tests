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

#pragma once

// C++ inlcudes
#include <filesystem>
#include <cstdint>
#include <vector>
#include <string>

#define ERROR \
	"Error [file " << std::filesystem::path(__FILE__).filename() << ", function '" \
	<< __func__ << "', line " << __LINE__ << "]:"

#define INVALID_KEYWORD "¡@/#/?234!"

#define TEST_GOODBYE \
	std::cout << "Test finished without apparent errors.\n";

namespace tests {

typedef std::vector<std::pair<std::string,std::string>> input_list;

enum class err_type : int8_t {
	/// The test completed successfully
	no_error = 0,

	/// wrong parameter value
	invalid_param,

	/// The input test file was not formatted correctly
	test_format,
	/// The format for the input graph is not supported
	graph_format,

	/// Some error occurred while performing the test
	test_execution,

	/// Unknown keyword: either it is not known or it is incorrect
	/// given its parent test type.
	wrong_keyword,
	/// too many keywords
	too_many_keywords,

	/// an error occurred when reading a file
	io,

	/// test not implemented
	not_implemented
};

} // -- namespace tests
