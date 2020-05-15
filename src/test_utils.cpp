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

#include "test_utils.hpp"

// C++ includes
#include <iostream>
#include <string>
#include <set>
using namespace std;

// lal includes
using namespace lal;

namespace exe_tests {

bool command_is_comment(const string& s)
{ return (s == "/*") or (s.find("/*") != string::npos); }

void process_comment(ifstream& fin) {
	string comment;
	do {
		fin >> comment;
	}
	while (comment.find("*/") == string::npos);
}

string read_output_string(ifstream& fin) {

	string total_message = "";
	string msg;

	// first string
	fin >> msg;
	total_message += msg.substr(1, msg.length());

	// first string is also last string
	if (msg[msg.length() - 1] == '"') {
		return total_message.substr(0, total_message.length() - 1);
	}

	// read the whole string
	while (fin >> msg and msg.find('"') == string::npos) {
		total_message += " " + msg;
	}

	// process last string
	total_message += " " + msg.substr(0, msg.length() - 1);
	return total_message;
}

/* -------------------------------------------------------------------------- */
/* ----- Utilities related to the library -- not so much to the tests ------- */

vector<node> invlinarr(const linearrgmnt& arr) {
	vector<node> ilin(arr.size());
	for (uint32_t p : arr) { ilin[ arr[p] ] = p; }
	return ilin;
}

} // -- namespace exe_tests
