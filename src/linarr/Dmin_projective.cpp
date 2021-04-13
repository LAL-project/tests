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
using namespace std;

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/graphs/conversions.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// common includes
#include "common/definitions.hpp"
#include "common/arrgmnt_validity_check.hpp"

// linarr includes
#include "linarr/linarr_brute_force_testing.hpp"

namespace tests {
namespace linarr {

err_type exe_linarr_Dmin_projective(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 1) {
		cerr << ERROR << endl;
		cerr << "    Exactly one input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	const set<string> allowed_algos({"AEF"});

	string algo;
	fin >> algo;

	if (allowed_algos.find(algo) == allowed_algos.end()) {
		cerr << ERROR << endl;
		cerr << "    Unrecognized algorithm '" << algo << "'." << endl;
		cerr << "    Allowed algorithms:" << endl;
		for (const auto& s : allowed_algos) {
		cerr << "    - " << s << endl;
		}
		return err_type::test_format;
	}

	ifstream input_file(inputs[0].first);
	if (not input_file.is_open()) {
		cerr << ERROR << endl;
		cerr << "    Input file '" << inputs[0].first << "' could not be opened." << endl;
		return err_type::io;
	}

	const auto err = linarr_brute_force_testing<rooted_tree>
	(
		[](const rooted_tree& t) {
			return Dmin_Projective(t);
		},
		[](const rooted_tree& t, const linear_arrangement& arr) {
			return sum_length_edges(t, arr);
		},
		[](const rooted_tree& t, const linear_arrangement& arr) {
			return is_arrangement_projective(t, arr);
		},
		[](const vector<node>& v) {
			return from_head_vector_to_rooted_tree(v);
		},
		[](rooted_tree& t) {
			t.calculate_size_subtrees();
		},
		input_file
	);

	if (err != err_type::no_error) {
		return err;
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
