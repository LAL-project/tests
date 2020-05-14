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
#include <lal/numeric/rational.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/all_projective_arrangements.hpp>
#include <lal/graphs/rtree.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace iterators;
using namespace linarr;
using namespace generate;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"
#include "test_utils.hpp"

namespace exe_tests {

pair<uint32_t, linearrgmnt> Dmin_projective_bruteforce(const rtree& t) {
	const uint32_t n = t.n_nodes();
	if (n == 1) { return make_pair(0, linearrgmnt(0,0)); }

	linearrgmnt arrMin;
	uint32_t Dmin = numeric_limits<uint32_t>::max();

	all_proj_arr ArrGen(t);
	while (ArrGen.has_next()) {
		ArrGen.next();
		const linearrgmnt arr = ArrGen.get_arrangement();

		const uint32_t D = sum_length_edges(t, arr);
		if (Dmin > D) {
			Dmin = D;
			arrMin = arr;
		}
	}

	return make_pair(Dmin, arrMin);
}

err_type test_Dmin_rtree_Projective(rtree& t) {
	if (not t.rtree_type_valid()) {
		t.find_rtree_type();
	}
	if (t.need_recalc_size_subtrees()) {
		t.recalc_size_subtrees();
	}

	// compute Dmin using the library's algorithm
	const pair<uint32_t, linearrgmnt> res_library
		= compute_Dmin(t, algorithms_Dmin::Projective);

	// compute Dmin by brute force
	const pair<uint32_t, linearrgmnt> res_bf
		= Dmin_projective_bruteforce(t);

	// compare results
	if (res_library.first != res_bf.first) {
		cerr << ERROR << endl;
		cerr << "    Values of projective Dmin do not coincide." << endl;
		cerr << "    Library:" << endl;
		cerr << "        Value: " << res_library.first << endl;
		cerr << "        Arrangement:     " << res_library.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res_library.second) << endl;
		cerr << "    Bruteforce:" << endl;
		cerr << "        Value: " << res_bf.first << endl;
		cerr << "        Arrangement:     " << res_bf.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res_bf.second) << endl;
		cerr << "    For tree: " << endl;
		cerr << t << endl;
		return err_type::test_exe_error;
	}
	return err_type::no_error;
}

err_type test_projective(ifstream& fin) {
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
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n_inputs << " were specified." << endl;
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

	// read number of nodes
	uint32_t n;
	while (fin >> n) {
		generate::all_ulab_rooted_trees TreeGen(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			rtree tree = TreeGen.get_tree();

			err_type r = test_Dmin_rtree_Projective(tree);
			if (r != err_type::no_error) {
				return r;
			}
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_Dmin_rooted(const string& alg, ifstream& fin) {
	err_type r;
	if (alg == "Projective") {
		r = test_projective(fin);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Test not implemented for algorithm 'alg'." << endl;
		r = err_type::not_implemented;
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
