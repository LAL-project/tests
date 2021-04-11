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
using namespace std;

// lal includes
#include <lal/generate/all_lab_rooted_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/properties/degrees.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/arrgmnt_validity_check.hpp"
#include "common/std_utils.hpp"
#include "common/tree_validity_check.hpp"

// generate includes
#include "generate/test_exhaustive_enumeration.hpp"

namespace tests {
namespace generate {

namespace alr {
struct extra_params { };

err_type test_for_n(uint32_t n, all_lab_rooted_trees& TreeGen, const extra_params&) {
	uint64_t gen = 0;
	while (TreeGen.has_next()) {
		TreeGen.next();
		const rooted_tree T = TreeGen.get_tree();
		const rtree_check err = test_validity_tree(n, T);
		if (err != rtree_check::correct) {
			cerr << ERROR << endl;
			cerr << "    Tree of index " << gen << " is not correct." << endl;
			cerr << "    Error: " << rtree_check_to_string(err) << endl;
			cerr << T << endl;
			return err_type::test_execution;
		}

		// compute 'statistics'
		gen += 1;
	}

	// Prüfer's formula: make sure that the generator made
	// as many trees as n^(n - 1) <- note that we are dealing
	// with labelled ROOTED trees!
	// https://oeis.org/A000169/list
	const integer nn = integer_from_ui(n);
	const integer total = (n == 1 ? 1 : (nn^(nn - 1)));

	if (gen != total) {
		cerr << ERROR << endl;
		cerr << "    Exhaustive generation of labelled rooted trees" << endl;
		cerr << "    Amount of trees of " << n << " vertices should be: " << total << endl;
		cerr << "    But generated: " << gen << endl;
		cerr << "    For a size of " << n << " vertices" << endl;
		return err_type::test_execution;
	}
	return err_type::no_error;
}
} // -- namespace alr

err_type exe_gen_trees_alr(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	// --- do the tests

	uint32_t n;
	while (fin >> n) {
		const auto err =
			test_exhaustive_enumeration_of_trees<all_lab_rooted_trees>
			(n, alr::test_for_n, alr::extra_params{});

		if (err != err_type::no_error) { return err; }
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
