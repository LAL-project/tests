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
#include <cassert>
#include <fstream>
using namespace std;

// lal includes
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "definitions.hpp"
#include "generate/tree_validity_check.hpp"

namespace exe_tests {

err_type exe_gen_trees_aur(const input_list& inputs, ifstream& fin) {

	/* BUILD TESTING DATA */

	// size of the vector with the number of unlabelled rooted trees
	const uint32_t SIZE_URT = 31;

	// from: https://oeis.org/A000081/list
	// amount of unlabelled rooted trees
	vector<integer> URT(SIZE_URT, 0);
	URT[0] = 0;
	URT[1] = 1;
	URT[2] = 1;
	URT[3] = 2;
	URT[4] = 4;
	URT[5] = 9;
	URT[6] = 20;
	URT[7] = 48;
	URT[8] = 115;
	URT[9] = 286;
	URT[10] = 719;
	URT[11] = 1842;
	URT[12] = 4766;
	URT[13] = 12486;
	URT[14] = 32973;
	URT[15] = 87811;
	URT[16] = 235381;
	URT[17] = 634847;
	URT[18] = 1721159;
	URT[19] = 4688676;
	URT[20] = 12826228;
	URT[21] = 35221832;
	URT[22] = 97055181;
	URT[23] = 268282855;
	URT[24] = 743724984;
	URT[25] = integer("2067174645");
	URT[26] = integer("5759636510");
	URT[27] = integer("16083734329");
	URT[28] = integer("45007066269");
	URT[29] = integer("126186554308");
	URT[30] = integer("354426847597");

	// -------------------------------------------------------------------------

	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	// --- do the tests

	all_ulab_rooted_trees TreeGen;

	uint32_t n;
	integer gen;
	while (fin >> n) {
		// number of generated trees
		gen = 0;

		// generate all trees
		TreeGen.init(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			const rtree T = TreeGen.get_tree();

			const rtree_check err = test_validity_tree(n, T);
			if (err != rtree_check::correct) {
				cerr << ERROR << endl;
				cerr << "    Tree of index " << gen << " is not correct." << endl;
				cerr << "    Error: " << rtree_check_to_string(err) << endl;
				cerr << T << endl;
				return err_type::test_exe_error;
			}

			// compute 'statistics'
			gen += 1;
		}

		// make sure that the amount of trees generate coincides
		// with the series from the OEIS
		if (n < SIZE_URT and gen != URT[n]) {
			cerr << ERROR << endl;
			cerr << "    Exhaustive generation of unlabelled rooted trees" << endl;
			cerr << "    Amount of trees should be: " << URT[n] << endl;
			cerr << "    But generated: " << gen << endl;
			cerr << "    For a size of " << n << " vertices" << endl;
			return err_type::test_exe_error;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
