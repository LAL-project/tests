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

// C++ includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/internal/std_utils.hpp>
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// custom includes
#include "definitions.hpp"
#include "arrgmnt_validity_check.hpp"
#include "test_utils.hpp"

namespace exe_tests {

err_type exe_linarr_Dmin_planar(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	// read number of nodes
	uint32_t n;
	fin >> n;

	if (n == 1) {
		// nothing to do
		return err_type::no_error;
	}

	vector<uint32_t> node_list(n);
	while (fin >> node_list[0]) {
		for (uint32_t i = 1; i < n; ++i) {
			fin >> node_list[i];
		}

		const free_tree tree = internal::linear_sequence_to_ftree(node_list).first;

		// execute library's algorithm
		const auto res_library = Dmin(tree, algorithms_Dmin::Planar);
		const linear_arrangement& arr = res_library.second;

		// ensure that the arrangement is a projective permutation
		const string err = is_arrangement_planar(tree, arr);
		if (err != "No error") {
			cerr << ERROR << endl;
			cerr << "    The result is not a planar arrangement." << endl;
			cerr << "    Error: '" << err << "'" << endl;
			cerr << "    Arrangement:     " << arr << endl;
			cerr << "    Inv Arrangement: " << invlinarr(arr) << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_exe_error;
		}

		// ensure that value of D matches the evaluation of the arrangement
		const uint32_t D = sum_length_edges(tree, res_library.second);
		if (D != res_library.first) {
			cerr << ERROR << endl;
			cerr << "    Value of D returned by method does not match the" << endl;
			cerr << "    evaluation of the arrangement." << endl;
			cerr << "    Arrangement:     " << res_library.second << endl;
			cerr << "    Inv Arrangement: " << invlinarr(res_library.second) << endl;
			cerr << "    Value of D returned: " << res_library.first << endl;
			cerr << "    Actual value of D:   " << D << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_exe_error;
		}

		// ensure that the value of D is actually minimum
		uint32_t brute_force_D;
		fin >> brute_force_D;
		if (res_library.first != brute_force_D) {
			cerr << ERROR << endl;
			cerr << "    Values of projective Dmin do not coincide." << endl;
			cerr << "    Library:" << endl;
			cerr << "        Value: " << res_library.first << endl;
			cerr << "        Arrangement:     " << res_library.second << endl;
			cerr << "        Inv Arrangement: " << invlinarr(res_library.second) << endl;
			cerr << "    Bruteforce:" << endl;
			cerr << "        Value: " << brute_force_D << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_exe_error;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
