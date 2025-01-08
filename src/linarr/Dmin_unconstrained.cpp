/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/Dmin.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/graphs/conversions.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"
#include "common/parse_header.hpp"

// linarr includes
#include "linarr/test_optimum_algorithm_one.hpp"
#include "linarr/arrangement_check.hpp"

typedef std::pair<uint64_t, lal::linear_arrangement> algo_result;

namespace tests {
namespace linarr {

namespace dmin_unconstrained {

bool test_correctness_arr_formula(
	const lal::graphs::free_tree& tree,
	const std::pair<uint64_t, lal::linear_arrangement>& res_lib,
	const std::string& tree_class,
	const uint64_t val_formula,
	const std::string& algorithm
)
noexcept
{
	const bool arr_check =
		check_correctness_arr
		(
			tree, res_lib, ERROR_str,
			algorithm + " applied to class of trees: " + tree_class,
			lal::linarr::is_arrangement
		);

	if (not arr_check) {
		return false;
	}
	/* compare results obtained by the library and by a formula */
	if (res_lib.first != val_formula) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Values of unconstrained Dmin do not coincide.\n";
		std::cerr << "    Library:\n";
		std::cerr << "        Value: " << res_lib.first << '\n';
		std::cerr << "        Arrangement:     " << res_lib.second.direct_as_vector() << '\n';
		std::cerr << "        Inv Arrangement: " << res_lib.second.inverse_as_vector() << '\n';
		std::cerr << "    Formula for class of trees '" << tree_class << "':\n";
		std::cerr << "        Value: " << val_formula << '\n';
		std::cerr << "    For tree: \n";
		std::cerr << tree << '\n';
		return false;
	}
	return true;
}

// -----------------------------------------------------------------------------

template <typename func_Dmin>
err_type test_bf_algorithm(const func_Dmin& A, std::ifstream& fin) noexcept
{
	return single_arrangement::test_optimum_algorithm<lal::graphs::free_tree>
	(
		// function to initialize a (free) tree
		[](lal::graphs::free_tree&) { },
		// calculate result
		[&](const lal::graphs::free_tree& t) {
			return A(t);
		},
		// evaluate input tree with an arrangement
		[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
			return lal::linarr::sum_edge_lengths(t, arr);
		},
		// check correctness of "arrangement"
		[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
			return lal::linarr::is_arrangement(t, arr);
		},
		// convert what you read from input to a (free) tree
		[](const std::vector<lal::node>& v) {
			return std::move(lal::graphs::from_head_vector_to_free_tree(v).first);
		},
		// where to read from
		fin
	);
}

template <typename func_Dmin>
err_type test_class_algorithm
(const func_Dmin& A, const std::string& algorithm, std::ifstream& fin)
noexcept
{
	std::string tree_class;
	uint64_t n1, n2;

	// read class of tree to test
	// read size of the tree
	while (fin >> tree_class) {
		fin >> n1 >> n2;
		std::cout << tree_class;

		if (tree_class == "linear") {
			std::cout << " -> [" << n1 << ", " << n2 << "]\n";

			for (uint64_t n = n1; n <= n2; ++n) {
				std::cout << "    " << n << '\n';
				lal::graphs::free_tree T(n);

				if (n > 1) {
					std::vector<lal::edge> es;
					for (lal::node u = 0; u < n - 1; ++u) { es.push_back({u,u+1}); }
					T.set_edges(es);
				}

				// Dmin for linear tree is
				const uint64_t Dmin_lintree = n - 1;
				// Dmin from the algorithm is
				const auto algo_res = A(T);

				const bool correct = test_correctness_arr_formula
					(T, algo_res, "Linear", Dmin_lintree, algorithm);

				if (not correct) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When dealing with a linear tree of " << n << " vertices.\n";
					std::cerr << "    Tree:\n";
					std::cerr << T << '\n';
					return err_type::test_execution;
				}
			}
		}
		else if (tree_class == "caterpillar") {
			std::cout << " -> [" << n1 << ", " << n2 << "]\n";

			for (uint64_t n = n1; n <= n2; ++n) {
				std::cout << "    " << n << '\n';

				lal::generate::all_ulab_free_trees TreeGen(n);
				while (not TreeGen.end()) {
					const lal::graphs::free_tree tree = TreeGen.get_tree();
					TreeGen.next();

					// filter non-caterpillar
					if (not tree.is_of_tree_type(lal::graphs::tree_type::caterpillar))
					{ continue; }

					// Dmin for caterpillar trees
					uint64_t Dmin_cat = 0;
					for (lal::node u = 0; u < n; ++u) {
						const uint64_t du = tree.get_degree(u);
						Dmin_cat += du*du + (du%2 == 1 ? 1 : 0);
					}
					Dmin_cat /= 4;

					// compute Dmin using the library's algorithm
					const auto res_algo = A(tree);

					const bool correct = test_correctness_arr_formula
						(tree, res_algo, "Caterpillar", Dmin_cat, algorithm);

					if (not correct) {
						std::cerr << ERROR << '\n';
						std::cerr << "    When dealing with a caterpillat tree of " << n << " vertices.\n";
						std::cerr << "    Tree:\n";
						std::cerr << tree << '\n';
						return err_type::test_execution;
					}
				}
			}
		}
		else if (tree_class == "binary-complete") {
			std::cout << " -> [" << n1 << ", " << n2 << "]\n";

			for (uint64_t n = n1; n <= n2; ++n) {
				std::cout << "    " << n << '\n';

				// We should interpret 'n' as the *heigh*.
				// The height of a single vertex is h=1
				const uint64_t h = n;

				// total amount of vertices
				uint64_t N;
				{
				lal::numeric::integer _N = 2;
				_N.powt(h);
				_N -= 1;
				N = _N.to_uint();
				}

				// build k-complete tree of height h
				lal::graphs::free_tree T(N);
				{
				lal::node next_node = 1;
				std::queue<lal::node> attach_to;
				attach_to.push(0);

				std::vector<lal::edge> edges(N - 1);
				std::size_t e_it = 0;

				while (not attach_to.empty()) {
					const lal::node s = attach_to.front();
					attach_to.pop();

					for (uint64_t d = 0; d < 2 and next_node < N; ++d, ++next_node) {
						const lal::node t = next_node;
						attach_to.push(t);

						edges[e_it] = lal::edge(s,t);
						++e_it;
					}
				}
				T.add_edges(edges, false);
				}

				uint64_t Dmin_bin_complete = 0;
				{
				lal::numeric::rational F = h;
				F /= 3; // k/3
				F += lal::numeric::rational(5,18); // k/3 + 5/18
				F *= lal::numeric::integer(2).power(h); // 2^k(k/3 + 5/18)

				F += lal::numeric::rational(2,9)*(h%2 == 0 ? 1 : -1); // 2^k(k/3 + 5/18) + (-1)^k(2/9)
				F -= 2; // 2^k(k/3 + 5/18) + (-1)^k(2/9) - 2
				Dmin_bin_complete = F.to_integer().to_uint();

				assert(F == Dmin_bin_complete);
				}

				// compute Dmin using the library's algorithm
				const auto res_algo = A(T);

				const bool correct = test_correctness_arr_formula
					(T, res_algo, "Binary complete", Dmin_bin_complete, algorithm);

				if (not correct) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When dealing with a complete binary tree of " << n << " vertices.\n";
					std::cerr << "    Tree:\n";
					std::cerr << T << '\n';
					return err_type::test_execution;
				}
			}
		}
		else if (tree_class == "star") {
			std::cout << " -> [" << n1 << ", " << n2 << "]\n";

			for (uint64_t n = n1; n <= n2; ++n) {
				std::cout << "    " << n << '\n';

				lal::graphs::free_tree T(n);
				for (lal::node u = 1; u < n; ++u) { T.add_edge(0, u); }

				const uint64_t Dmin_star = (n*n - n%2)/4;

				// compute Dmin using the library's algorithm
				const auto res_algo = A(T);

				const bool correct = test_correctness_arr_formula
					(T, res_algo, "Star", Dmin_star, algorithm);

				if (not correct) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When dealing with a star tree of " << n << " vertices.\n";
					std::cerr << "    Tree:\n";
					std::cerr << T << '\n';
					return err_type::test_execution;
				}
			}
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Tree class '" << tree_class << "' not recognised.\n";
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

template <typename func_Dmin>
err_type test_tree_algorithm
(const func_Dmin& A, const std::string& algorithm, std::ifstream& fin)
noexcept
{
	// read number of vertices
	uint64_t n;
	while (fin >> n) {
		lal::graphs::free_tree T(n);
		lal::node u,v;
		for (uint64_t i = 0; i < n - 1; ++i) {
			fin >> u >> v;
			T.add_edge(u,v);
		}

		if (not T.is_tree()) {
			std::cerr << ERROR << '\n';
			std::cerr << "Input tree is not a tree.\n";
			return err_type::test_execution;
		}

		const auto& res_algo = A(T);
		const bool correct =
			check_correctness_arr
			(T, res_algo, ERROR_str, algorithm, lal::linarr::is_arrangement);

		if (not correct) {
			return err_type::test_execution;
		}
		std::cout << res_algo.first << '\n';
	}

	return err_type::no_error;
}

} // -- namespace dmin_unconstrained

err_type exe_linarr_Dmin_unconstrained(std::ifstream& fin) noexcept {
	const input_list inputs = read_input_list(fin);

	std::string what;
	fin >> what;

	err_type r;
	// --------------
	// YOSSI SHILOACH
	if (what == "YS_bruteforce") {
		if (inputs.size() != 1) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Exactly one input files are allowed in this test.\n";
			std::cerr << "    Instead, " << inputs.size() << " were given.\n";
			return err_type::test_format;
		}

		if (not std::filesystem::exists(inputs[0].first)) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Input file '" << inputs[0].first << "' does not exist.\n";
			return err_type::io;
		}
		std::ifstream input_file(inputs[0].first);

		r =
		dmin_unconstrained::test_bf_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Shiloach);
			},
			input_file
		);

		input_file.close();
	}
	else if (what == "YS_class") {
		r =
		dmin_unconstrained::test_class_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Shiloach);
			},
			"Shiloach",
			fin
		);
	}
	else if (what == "YS_tree") {
		r =
		dmin_unconstrained::test_tree_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Shiloach);
			},
			"Shiloach",
			fin
		);
	}
	// ---------------
	// FAN R. K. CHUNG
	else if (what == "FC_bruteforce") {
		if (inputs.size() != 1) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Exactly one input files are allowed in this test.\n";
			std::cerr << "    Instead, " << inputs.size() << " were given.\n";
			return err_type::test_format;
		}

		if (not std::filesystem::exists(inputs[0].first)) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Input file '" << inputs[0].first << "' does not exist.\n";
			return err_type::io;
		}
		std::ifstream input_file(inputs[0].first);

		r =
		dmin_unconstrained::test_bf_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Chung_2);
			},
			input_file
		);

		input_file.close();
	}
	else if (what == "FC_class") {
		r =
		dmin_unconstrained::test_class_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Chung_2);
			},
			"Chung_2",
			fin
		);
	}
	else if (what == "FC_tree") {
		r =
		dmin_unconstrained::test_tree_algorithm(
			[](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Chung_2);
			},
			"Chung_2",
			fin
		);
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Test not implemented for '" << what << "'.\n";
		r = err_type::not_implemented;
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
