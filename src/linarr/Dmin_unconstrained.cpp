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
#include <algorithm>
#include <filesystem>
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
#include <lal/linarr/formal_constraints.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/graphs/conversions.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace numeric;

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

// linarr includes
#include "linarr/linarr_brute_force_testing.hpp"

typedef pair<uint64_t, linear_arrangement> algo_result;

namespace tests {
namespace linarr {

namespace dmin_unconstrained {

inline
bool check_correctness_arr(const free_tree& tree, const pair<uint64_t, linear_arrangement>& res) {
	const linear_arrangement& arr = res.second;
	// ensure that the result is an arrangement
	if (not lal::linarr::is_permutation(arr)) {
		cerr << ERROR << endl;
		cerr << "    The result is not an arrangement (permutation)." << endl;
		cerr << "        Arrangement: " << arr << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	// ensure that value of D is correct
	const uint64_t D = sum_edge_lengths(tree, arr);
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
	const free_tree& tree, const pair<uint64_t, linear_arrangement>& res_lib,
	const string& tree_class, const uint64_t val_formula
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

err_type test_bf_algorithm(
	// A is a function that encapsulates an algorithm
	// -- be it Shiloach's, be it Fan Chung's
	const function< pair<uint64_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
	const auto err = linarr_brute_force_testing<free_tree>
	(
		// calculate result
		[&](const free_tree& t) {
			return A(t);
		},
		// evaluate input tree with an arrangement
		[](const free_tree& t, const linear_arrangement& arr) {
			return sum_edge_lengths(t, arr);
		},
		// check correctness of "arrangement"
		[](const free_tree& t, const linear_arrangement& arr) {
			return is_arrangement(t, arr);
		},
		// convert what you read from input to a (free) tree
		[](const vector<node>& v) {
			return from_head_vector_to_free_tree(v).first;
		},
		// function to initialise a (free) tree
		[](free_tree&) { },
		// where to read from
		fin
	);
	return err;
}

err_type test_class_algorithm(
	const function< pair<uint64_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
	string tree_class;
	uint64_t n1, n2;

	// read class of tree to test
	// read size of the tree
	while (fin >> tree_class) {
		fin >> n1 >> n2;
		cout << tree_class;

		if (tree_class == "linear") {
			cout << " -> [" << n1 << ", " << n2 << "]" << endl;

			for (uint64_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;
				free_tree T(n);

				if (n > 1) {
					vector<edge> es;
					for (node u = 0; u < n - 1; ++u) { es.push_back({u,u+1}); }
					T.set_edges(es);
				}

				// Dmin for linear tree is
				const uint64_t Dmin_lintree = n - 1;
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

			for (uint64_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

				generate::all_ulab_free_trees TreeGen(n);
				while (not TreeGen.end()) {
					const free_tree tree = TreeGen.get_tree();
					TreeGen.next();

					// filter non-caterpillar
					if (not tree.is_of_tree_type(tree_type::caterpillar))
					{ continue; }

					// Dmin for caterpillar trees
					uint64_t Dmin_cat = 0;
					for (node u = 0; u < n; ++u) {
						const uint64_t du = tree.get_degree(u);
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

			for (uint64_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

				// We should interpret 'n' as the *heigh*.
				// The height of a single vertex is h=1
				const uint64_t h = n;

				// total amount of vertices
				uint64_t N;
				{
				integer _N = 2;
				_N.powt(h);
				_N -= 1;
				N = static_cast<uint64_t>(_N.to_uint());
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

					for (uint64_t d = 0; d < 2 and next_node < N; ++d, ++next_node) {
						const node t = next_node;
						attach_to.push(t);

						edges[e_it] = edge(s,t);
						++e_it;
					}
				}
				T.add_edges(edges, false);
				}

				uint64_t Dmin_bin_complete = 0;
				{
				rational F = h;
				F /= 3; // k/3
				F += rational(5,18); // k/3 + 5/18
				F *= integer(2).pow(h); // 2^k(k/3 + 5/18)

				F += rational(2,9)*(h%2 == 0 ? 1 : -1); // 2^k(k/3 + 5/18) + (-1)^k(2/9)
				F -= 2; // 2^k(k/3 + 5/18) + (-1)^k(2/9) - 2
				Dmin_bin_complete = static_cast<uint64_t>(F.to_integer().to_uint());

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

			for (uint64_t n = n1; n <= n2; ++n) {
				cout << "    " << n << endl;

				free_tree T(n);
				for (node u = 1; u < n; ++u) { T.add_edge(0, u); }

				const uint64_t Dmin_star = (n*n - n%2)/4;

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

err_type test_tree_algorithm(
	const function< pair<uint64_t, linear_arrangement> (const free_tree&) >& A,
	ifstream& fin
)
{
	// read number of vertices
	uint64_t n;
	while (fin >> n) {
		free_tree T(n);
		node u,v;
		for (uint64_t i = 0; i < n - 1; ++i) {
			fin >> u >> v;
			T.add_edge(u,v);
		}

		const auto& r = A(T);
		cout << r.first << endl;
	}

	return err_type::no_error;
}

} // -- namespace dmin_unconstrained

err_type exe_linarr_Dmin_unconstrained(const input_list& inputs, ifstream& fin) {
	string what;
	fin >> what;

	err_type r;
	// --------------
	// YOSSI SHILOACH
	if (what == "YS_bruteforce") {
		if (inputs.size() != 1) {
			cerr << ERROR << endl;
			cerr << "    Exactly one input files are allowed in this test." << endl;
			cerr << "    Instead, " << inputs.size() << " were given." << endl;
			return err_type::test_format;
		}

		if (not filesystem::exists(inputs[0].first)) {
			cerr << ERROR << endl;
			cerr << "    Input file '" << inputs[0].first << "' does not exist." << endl;
			return err_type::io;
		}
		ifstream input_file(inputs[0].first);

		r =
		dmin_unconstrained::test_bf_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_YS);
			}
		, input_file
		);

		input_file.close();
	}
	else if (what == "YS_class") {
		r =
		dmin_unconstrained::test_class_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	else if (what == "YS_tree") {
		r =
		dmin_unconstrained::test_tree_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_YS);
			}
		, fin
		);
	}
	// ---------------
	// FAN R. K. CHUNG
	else if (what == "FC_bruteforce") {
		if (inputs.size() != 1) {
			cerr << ERROR << endl;
			cerr << "    Exactly one input files are allowed in this test." << endl;
			cerr << "    Instead, " << inputs.size() << " were given." << endl;
			return err_type::test_format;
		}

		if (not filesystem::exists(inputs[0].first)) {
			cerr << ERROR << endl;
			cerr << "    Input file '" << inputs[0].first << "' does not exist." << endl;
			return err_type::io;
		}
		ifstream input_file(inputs[0].first);

		r =
		dmin_unconstrained::test_bf_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_FC);
			}
		, input_file
		);

		input_file.close();
	}
	else if (what == "FC_class") {
		r =
		dmin_unconstrained::test_class_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else if (what == "FC_tree") {
		r =
		dmin_unconstrained::test_tree_algorithm(
			[](const free_tree& t) -> pair<uint64_t, linear_arrangement> {
				return min_sum_edge_lengths(t, algorithms_Dmin::Unconstrained_FC);
			}
		, fin
		);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Test not implemented for '" << what << "'." << endl;
		r = err_type::not_implemented;
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
