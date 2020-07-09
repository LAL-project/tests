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
#include <cassert>
#include <queue>
#include <set>
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/graphs/ftree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/utils/std_utils.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace numeric;
using namespace generate;

// custom includes
#include "definitions.hpp"
#include "arrgmnt_validity_check.hpp"
#include "test_utils.hpp"
#include "tree_classification.hpp"

namespace exe_tests {

err_type consume_beginning(ifstream& F) {
	string field;
	F >> field;
	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	size_t n_inputs;
	F >> n_inputs;
	if (n_inputs != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n_inputs << " were specified." << endl;
		return err_type::test_format_error;
	}
	F >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	return err_type::no_error;
}

pair<uint32_t, linearrgmnt> Dmin_Unconstrained_bruteforce(const ftree& t) {
	const uint32_t n = t.n_nodes();
	if (n == 1) { return make_pair(0, linearrgmnt(0,0)); }

	linearrgmnt arr(n);
	std::iota(arr.begin(), arr.end(), 0);

	linearrgmnt arrMin;
	uint32_t Dmin = numeric_limits<uint32_t>::max();

	do {
		const uint32_t D = sum_length_edges(t, arr);
		if (Dmin > D) {
			Dmin = D;
			arrMin = arr;
		}
	}
	while (std::next_permutation(arr.begin(), arr.end()));

	return make_pair(Dmin, arrMin);
}

bool test_correctness_arr_1(const ftree& tree, const pair<uint32_t, linearrgmnt>& res) {
	const linearrgmnt& arr = res.second;
	/* ensure that the result is an arrangement */
	if (not is_arrangement(arr)) {
		cerr << ERROR << endl;
		cerr << "    The result is not an arrangement (permutation)." << endl;
		cerr << "    Arrangement: " << arr << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	/* ensure that value of D is correct */
	const uint32_t D = sum_length_edges(tree, arr);
	if (D != res.first) {
		cerr << ERROR << endl;
		cerr << "    Value of D returned by method is incorrect." << endl;
		cerr << "    Arrangement:     " << res.second << endl;
		cerr << "    Inv Arrangement: " << invlinarr(res.second) << endl;
		cerr << "    Value of D returned: " << res.first << endl;
		cerr << "    Actual value of D:   " << D << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	return true;
}

bool test_correctness_arr_bf(
	const ftree& tree, const pair<uint32_t, linearrgmnt>& res_lib
)
{
	if (not test_correctness_arr_1(tree, res_lib)) {
		return false;
	}
	/* compute Dmin by brute force */
	const pair<uint32_t, linearrgmnt> res_bf = Dmin_Unconstrained_bruteforce(tree);
	/* compare results obtained by the library and by brute force */
	if (res_lib.first != res_bf.first) {
		cerr << ERROR << endl;
		cerr << "    Values of unconstrained Dmin do not coincide." << endl;
		cerr << "    Library:" << endl;
		cerr << "        Value: " << res_lib.first << endl;
		cerr << "        Arrangement:     " << res_lib.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res_lib.second) << endl;
		cerr << "    Bruteforce:" << endl;
		cerr << "        Value: " << res_bf.first << endl;
		cerr << "        Arrangement:     " << res_bf.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res_bf.second) << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	return true;
}

bool test_correctness_arr_formula(
	const ftree& tree, const pair<uint32_t, linearrgmnt>& res_lib,
	const string& tree_class, const uint32_t val_formula
)
{
	if (not test_correctness_arr_1(tree, res_lib)) {
		return false;
	}
	/* compare results obtained by the library and by a formula */
	if (res_lib.first != val_formula) {
		cerr << ERROR << endl;
		cerr << "    Values of unconstrained Dmin do not coincide." << endl;
		cerr << "    Library:" << endl;
		cerr << "        Value: " << res_lib.first << endl;
		cerr << "        Arrangement:     " << res_lib.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res_lib.second) << endl;
		cerr << "    Formula for class of trees '" << tree_class << "':" << endl;
		cerr << "        Value: " << val_formula << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	return true;
}

// -----------------------------------------------------------------------------

err_type test_Unconstrained(ifstream& fin) {
	return err_type::no_error;
}

err_type test_Unconstrained_class_algorithm(
	const function< pair<uint32_t, linearrgmnt> (const ftree&) >& A,
	ifstream& fin
)
{
	if (const auto err = consume_beginning(fin); err != err_type::no_error) {
		return err;
	}

	string tree_class;
	uint32_t n;

	// read class of tree to test
	// read size of the tree
	while (fin >> tree_class) {
		fin >> n;
		if (tree_class == "linear") {
			ftree T(n);

			if (n > 1) {
				for (uint32_t u = 0; u < n - 1; ++u) { T.add_edge(u, u+1, false); }
			}

			// Dmin for linear tree is
			const uint32_t Dmin_lintree = n - 1;
			// Dmin from the algorithm is
			const auto algo_res = A(T);

			const bool correct =
			test_correctness_arr_formula(T, algo_res, "Linear", Dmin_lintree);
			if (not correct) { return err_type::test_exe_error; }
		}
		else if (tree_class == "caterpillar") {

			generate::all_ulab_free_trees TreeGen(n);
			while (TreeGen.has_next()) {
				TreeGen.next();
				const ftree tree = TreeGen.get_tree();

				// filter non-caterpillar
				if (not is_caterpillar(tree)) { continue; }

				// Dmin for caterpillar trees
				uint32_t Dmin_cat = 0;
				for (node u = 0; u < n; ++u) {
					const uint32_t du = tree.degree(u);
					Dmin_cat += du*du + (du%2 == 1 ? 1 : 0);
				}
				Dmin_cat /= 4;

				// compute Dmin using the library's algorithm
				const auto res_algo = A(tree);

				const bool correct =
				test_correctness_arr_formula(tree, res_algo, "Caterpillar", Dmin_cat);
				if (not correct) { return err_type::test_exe_error; }
			}
		}
		else if (tree_class == "binary-complete") {
			// We should interpret 'n' as the *heigh*.
			// The height of a single vertex is h=1
			const uint32_t h = n;

			// total amount of vertices
			uint32_t N;
			{
			integer _N = 2;
			_N ^= h;
			_N -= 1;
			N = static_cast<uint32_t>(_N.to_uint());
			}

			// build k-complete tree of height h
			ftree T(N);
			{
			node next_node = 1;
			queue<node> attach_to;
			attach_to.push(0);

			vector<edge> edges(N - 1);
			size_t e_it = 0;

			while (not attach_to.empty()) {
				const node s = attach_to.front();
				attach_to.pop();

				for (uint32_t d = 0; d < 2 and next_node < N; ++d, ++next_node) {
					const node t = next_node;
					attach_to.push(t);

					edges[e_it] = edge(s,t);
					++e_it;
				}
			}
			T.add_edges(edges, false);
			}

			uint32_t Dmin_bin_complete = 0;
			{
			rational F = h;
			F /= 3; // k/3
			F += rational(5,18); // k/3 + 5/18
			F *= integer(2)^h; // 2^k(k/3 + 5/18)

			F += rational(2,9)*(h%2 == 0 ? 1 : -1); // 2^k(k/3 + 5/18) + (-1)^k(2/9)
			F -= 2; // 2^k(k/3 + 5/18) + (-1)^k(2/9) - 2
			Dmin_bin_complete = static_cast<uint32_t>(F.to_integer().to_uint());

			assert(F == Dmin_bin_complete);
			}

			// compute Dmin using the library's algorithm
			const auto res_algo = A(T);

			const bool correct =
			test_correctness_arr_formula(T, res_algo, "Binary complete", Dmin_bin_complete);
			if (not correct) { return err_type::test_exe_error; }
		}
		else if (tree_class == "star") {
			ftree T(n);
			for (node u = 1; u < n; ++u) { T.add_edge(0, u); }

			const uint32_t Dmin_star = (n*n - n%2)/4;

			// compute Dmin using the library's algorithm
			const auto res_algo = A(T);

			const bool correct =
			test_correctness_arr_formula(T, res_algo, "Star", Dmin_star);
			if (not correct) { return err_type::test_exe_error; }
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Tree class '" << tree_class << "' not recognised." << endl;
			return err_type::test_format_error;
		}
	}
	return err_type::no_error;
}

err_type test_Unconstrained_bf_algorithm(
	const function< pair<uint32_t, linearrgmnt> (const ftree&) >& A,
	ifstream& fin
)
{
	if (const auto err = consume_beginning(fin); err != err_type::no_error) {
		return err;
	}

	// read number of vertices
	uint32_t n;
	while (fin >> n) {
		// enumerate all trees of 'n' vertices
		generate::all_ulab_free_trees TreeGen(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			const ftree tree = TreeGen.get_tree();

			// compute Dmin using the library's algorithm
			const auto res_algo = A(tree);

			const bool correct = test_correctness_arr_bf(tree, res_algo);
			if (not correct) { return err_type::test_exe_error; }
		}
	}
	return err_type::no_error;
}

err_type exe_linarr_Dmin_free(const string& alg, ifstream& fin) {
	err_type r;
	if (alg == "Unconstrained") {
		r = test_Unconstrained(fin);
	}
	else if (alg == "Unconstrained_bruteforce_YS") {
		r =
		test_Unconstrained_bf_algorithm(
			[](const ftree& t) -> pair<uint32_t, linearrgmnt> {
				return compute_Dmin(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	else if (alg == "Unconstrained_class_YS") {
		r =
		test_Unconstrained_class_algorithm(
			[](const ftree& t) -> pair<uint32_t, linearrgmnt> {
				return compute_Dmin(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	else if (alg == "Unconstrained_bruteforce_FC") {
		r =
		test_Unconstrained_bf_algorithm(
			[](const ftree& t) -> pair<uint32_t, linearrgmnt> {
				return compute_Dmin(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else if (alg == "Unconstrained_class_FC") {
		r =
		test_Unconstrained_class_algorithm(
			[](const ftree& t) -> pair<uint32_t, linearrgmnt> {
				return compute_Dmin(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Test not implemented for algorithm '" << alg << "'." << endl;
		r = err_type::not_implemented;
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
