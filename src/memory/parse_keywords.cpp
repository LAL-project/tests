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
using namespace std;

// common includes
#include "common/parse_keywords.hpp"
#include "common/parse_header.hpp"
#include "memory/parse_keywords.hpp"
#include "memory/exe_tests.hpp"

namespace tests {
namespace memory {

err_type call_memory_internal
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "data_array") {
		return parse_header(exe_memory_internal_data_array, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_memory
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "graphs") {
		return parse_header(exe_memory_graphs, fin);
	}
	if (key == "numeric") {
		return parse_header(exe_memory_numeric, fin);
	}
	if (key == "internal") {
		return call_memory_internal(keywords, i+1, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_main(const vector<string>& keywords, ifstream& fin) {
	const string& key = keywords[0];
	if (key == "memory") {
		return call_memory(keywords, 1, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at 0: '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

} // -- namespace memory
} // -- namespace tests
