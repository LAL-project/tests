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
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;

// custom includes
#include "definitions.hpp"

/*
 * RANDOM UNLABELLED FREE TREES
 *
 */

namespace exe_tests {

err_type exe_gen_trees_rur(std::ifstream& fin) {

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_inputs;
	fin >> n_inputs;
	if (n_inputs != 0) {
		cerr << ERROR << endl;
		cerr << "    Expected no inputs at all." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
		return err_type::test_format_error;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	// --- do the tests

	uint32_t n;

	rand_ulab_rooted_trees TreeGen;

	while (fin >> n) {
		TreeGen.init(n, 100);
		for (int i = 0; i < 10000; ++i) {
			const rtree T = TreeGen.make_rand_tree();

			if (not T.is_tree()) {
				cerr << ERROR << endl;
				cerr << "    Graph generated is not a tree." << endl;
				cerr << T << endl;
				return err_type::test_exe_error;
			}
			if (T.n_nodes() != n) {
				cerr << ERROR << endl;
				cerr << "    Number of vertices of the tree is not '" << n << "'." << endl;
				cerr << "    T.n_nodes()= " << T.n_nodes() << endl;
				return err_type::test_exe_error;
			}
			if (T.n_edges() != n - 1) {
				cerr << ERROR << endl;
				cerr << "    Number of edges of the tree is not '" << n-1 << "'." << endl;
				cerr << "    T.n_edges()= " << T.n_edges() << endl;
				return err_type::test_exe_error;
			}
			if (not T.has_root()) {
				cerr << ERROR << endl;
				cerr << "    Tree '" << i << "' does not have a root." << endl;
				cerr << T << endl;
				return err_type::test_exe_error;
			}

		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
