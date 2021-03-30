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
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_projective_arrangements.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace generate;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"
#include "arrgmnt_validity_check.hpp"
#include "std_utils.hpp"

namespace exe_tests {

integer factorial(int64_t f) {
	if (f == 0) { return 1; }
	const integer f1 = factorial(f - 1);
	return f1*f;
}

integer amount_projective(const rooted_tree& rT) {
	integer k = 1;
	for (node u = 0; u < rT.get_num_nodes(); ++u) {
		k *= factorial(rT.get_out_degree(u) + 1);
	}
	return k;
}

err_type test_a_tree(rooted_tree& rT, uint32_t nrelabs) {
	vector<edge> edges = rT.get_edges();

	for (uint32_t i = 0; i < nrelabs; ++i) {
		shuffle_tree(edges, rT);

		uint32_t iterations = 0;
		set<linear_arrangement> list_arrs;

		all_projective_arrangements ArrGen(rT);
		while (ArrGen.has_next()) {
			ArrGen.next();
			const linear_arrangement arr = ArrGen.get_arrangement();

			// Do some sanity checks.
			const string err = is_arrangement_projective(rT, arr);
			if (err != "") {
				cerr << ERROR << endl;
				cerr << "    Generation of arrangement failed with error:" << endl;
				cerr << "    '" << err << "'" << endl;
				cerr << "    Arrangement:     " << arr << endl;
				cerr << "    Inv Arrangement: " << invlinarr(arr) << endl;
				cerr << "    For tree:" << endl;
				cerr << rT << endl;
				return err_type::test_execution;
			}

			++iterations;
			list_arrs.insert(arr);
		}

		const integer formula = amount_projective(rT);
		if (formula != iterations or formula != list_arrs.size()) {
			cerr << ERROR << endl;
			cerr << "    Number of projective arrangements generated" << endl;
			cerr << "    does not agree with the formula." << endl;
			cerr << "        formula= " << formula << endl;
			cerr << "        iterations= " << iterations << endl;
			cerr << "        unique amount= " << list_arrs.size() << endl;
			cerr << "    List of arrangements:" << endl;
			for (const auto& v : list_arrs) {
			cerr << "        " << v << endl;
			}
			cerr << "    For tree:" << endl;
			cerr << rT << endl;
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

err_type exe_gen_arr_all_proj(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	uint32_t n, nrelabs;
	while (fin >> n >> nrelabs) {
		// do 'ntrees' trees of 'n' vertices
		all_ulab_rooted_trees TreeGen(n);

		while (TreeGen.has_next()) {
			TreeGen.next();
			rooted_tree rT = TreeGen.get_tree();

			const err_type e = test_a_tree(rT, nrelabs);
			if (e != err_type::no_error) {
				return e;
			}
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
