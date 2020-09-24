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
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/internal/std_utils.hpp>
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace numeric;

// custom includes
#include "definitions.hpp"
#include "arrgmnt_validity_check.hpp"
#include "test_utils.hpp"
#include "tree_classification.hpp"

typedef pair<uint32_t, linear_arrangement> algo_result;

namespace exe_tests {

inline
bool check_correctness_arr(const free_tree& tree, const pair<uint32_t, linear_arrangement>& res) {
	const linear_arrangement& arr = res.second;
	// ensure that the result is an arrangement
	if (not is_arrangement(arr)) {
		cerr << ERROR << endl;
		cerr << "    The result is not an arrangement (permutation)." << endl;
		cerr << "        Arrangement: " << arr << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	// ensure that value of D is correct
	const uint32_t D = sum_length_edges(tree, arr);
	if (D != res.first) {
		cerr << ERROR << endl;
		cerr << "    Value of D returned by method is incorrect." << endl;
		cerr << "        Arrangement:     " << res.second << endl;
		cerr << "        Inv Arrangement: " << invlinarr(res.second) << endl;
		cerr << "        Value of D returned: " << res.first << endl;
		cerr << "        Actual value of D:   " << D << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	return true;
}

bool test_correctness_arr_formula(
	const free_tree& tree, const pair<uint32_t, linear_arrangement>& res_lib,
	const string& tree_class, const uint32_t val_formula
)
{
	if (not check_correctness_arr(tree, res_lib)) {
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

err_type test_Unconstrained_bf_algorithm(
	const function< pair<uint32_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
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
		// read input arrangement
		linear_arrangement input_arr(n);
		for (uint32_t i = 0; i < n; ++i) {
			fin >> input_arr[i];
		}
		// read value of D calculated by brute force
		uint32_t brute_force_D;
		fin >> brute_force_D;

		// construct tree
		const free_tree T = internal::linear_sequence_to_ftree(node_list).first;

		// check correctness of input arrangement
		if (uint32_t DD = sum_length_edges(T, input_arr) != brute_force_D) {
			cerr << ERROR << endl;
			cerr << "    Input value of D calculated by brute force does not" << endl;
			cerr << "    agree with the evaluation of the tree." << endl;
			cerr << "        Input arrangement: " << input_arr << endl;
			cerr << "        Input value: " << brute_force_D << endl;
			cerr << "        Evaluation: " << DD << endl;
			return err_type::test_format;
		}

		// execute library's algorithm
		const auto res_A = A(T);

		// ensure that the arrangement is a permutation
		if (not check_correctness_arr(T, res_A)) {
			return err_type::test_execution;
		}

		if (res_A.first != brute_force_D) {
			cerr << ERROR << endl;
			cerr << "    Result of algorithm does not match base result." << endl;
			cerr << "        Base result: " << brute_force_D << endl;
			cerr << "        Algorithm result:" << res_A.first << endl;
			cerr << "    For tree:" << endl;
			cerr << T << endl;
			return err_type::test_execution;
		}
	}

	return err_type::no_error;
}

err_type test_Unconstrained_class_algorithm(
	const function< pair<uint32_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
	string tree_class;
	uint32_t n1, n2;

	// read class of tree to test
	// read size of the tree
	while (fin >> tree_class) {
		fin >> n1 >> n2;
		cout << tree_class;

		if (tree_class == "linear") {
			cout << " -> [" << n1 << ", " << n2 << "]" << endl;

			for (uint32_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;
				free_tree T(n);

				if (n > 1) {
					for (node u = 0; u < n - 1; ++u) { T.add_edge(u, u+1, false); }
				}

				// Dmin for linear tree is
				const uint32_t Dmin_lintree = n - 1;
				// Dmin from the algorithm is
				const auto algo_res = A(T);

				const bool correct =
				test_correctness_arr_formula(T, algo_res, "Linear", Dmin_lintree);
				if (not correct) {
					cerr << ERROR << endl;
					cerr << "    When dealing with a linear tree of " << n << " vertices." << endl;
					cerr << "    Tree:" << endl;
					cerr << T << endl;
					return err_type::test_execution;
				}
			}
		}
		else if (tree_class == "caterpillar") {
			cout << " -> [" << n1 << ", " << n2 << "]" << endl;

			for (uint32_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

				generate::all_ulab_free_trees TreeGen(n);
				while (TreeGen.has_next()) {
					TreeGen.next();
					const free_tree tree = TreeGen.get_tree();

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
					if (not correct) {
						cerr << ERROR << endl;
						cerr << "    When dealing with a caterpillat tree of " << n << " vertices." << endl;
						cerr << "    Tree:" << endl;
						cerr << tree << endl;
						return err_type::test_execution;
					}
				}
			}
		}
		else if (tree_class == "binary-complete") {
			cout << " -> [" << n1 << ", " << n2 << "]" << endl;

			for (uint32_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

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
				free_tree T(N);
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
				if (not correct) {
					cerr << ERROR << endl;
					cerr << "    When dealing with a complete binary tree of " << n << " vertices." << endl;
					cerr << "    Tree:" << endl;
					cerr << T << endl;
					return err_type::test_execution;
				}
			}
		}
		else if (tree_class == "star") {
			cout << " -> [" << n1 << ", " << n2 << "]" << endl;

			for (uint32_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

				free_tree T(n);
				for (node u = 1; u < n; ++u) { T.add_edge(0, u); }

				const uint32_t Dmin_star = (n*n - n%2)/4;

				// compute Dmin using the library's algorithm
				const auto res_algo = A(T);

				const bool correct =
				test_correctness_arr_formula(T, res_algo, "Star", Dmin_star);
				if (not correct) {
					cerr << ERROR << endl;
					cerr << "    When dealing with a star tree of " << n << " vertices." << endl;
					cerr << "    Tree:" << endl;
					cerr << T << endl;
					return err_type::test_execution;
				}
			}
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Tree class '" << tree_class << "' not recognised." << endl;
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

err_type test_Unconstrained_tree_algorithm(
	const function< pair<uint32_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
	// read number of vertices
	uint32_t n;
	while (fin >> n) {
		free_tree T(n);
		node u,v;
		for (uint32_t i = 0; i < n - 1; ++i) {
			fin >> u >> v;
			T.add_edge(u,v);
		}

		const auto& r = A(T);
		cout << r.first << endl;
	}

	return err_type::no_error;
}

err_type exe_linarr_Dmin_unconstrained(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	string alg;
	fin >> alg;

	err_type r;
	// --------------
	// YOSSI SHILOACH
	if (alg == "YS_bruteforce") {
		r =
		test_Unconstrained_bf_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	else if (alg == "YS_class") {
		r =
		test_Unconstrained_class_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	else if (alg == "YS_tree") {
		r =
		test_Unconstrained_tree_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	// ---------------
	// FAN R. K. CHUNG
	else if (alg == "FC_bruteforce") {
		r =
		test_Unconstrained_bf_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else if (alg == "FC_class") {
		r =
		test_Unconstrained_class_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else if (alg == "FC_tree") {
		r =
		test_Unconstrained_tree_algorithm(
			[](const free_tree& t) -> pair<uint32_t, linear_arrangement> {
				return Dmin(t, algorithms_Dmin::Unconstrained_FC);
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
