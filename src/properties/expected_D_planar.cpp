/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/time.hpp"

namespace tests {
namespace properties {

err_type exe_properties_expected_D_planar_brute_force
(const input_list& inputs, std::ifstream& fin)
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	uint64_t n;
	fin >> n;

	lal::head_vector hv(n);
	while (fin >> hv[0]) {
		for (uint64_t i = 1; i < n; ++i) { fin >> hv[i]; }

		const lal::graphs::free_tree T = lal::graphs::from_head_vector_to_free_tree(hv).first;

		// calculate the value with the algorithm
		const auto algo_value = lal::properties::exp_sum_edge_lengths_planar_rational(T);

		std::string ground_truth_str;
		fin >> ground_truth_str;

		const lal::numeric::rational ground_truth(ground_truth_str);
		if (algo_value != ground_truth) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Value calculated with algorithm does not coincide with\n";
			std::cerr << "    ground truth value.\n";
			std::cerr << "    Algorithm: " << algo_value << '\n';
			std::cerr << "    Ground truth: " << ground_truth << '\n';
			std::cerr << "    For tree:\n";
			std::cerr << T << '\n';
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

lal::numeric::rational quadratic_E_pr_D(const lal::graphs::free_tree& t) {
	const uint64_t n = t.get_num_nodes();
	const lal::numeric::rational correction_factor((n - 1)*(n - 2), 6*n);
	lal::numeric::rational S = 0;
	for (lal::node u = 0; u < n; ++u) {
		lal::graphs::rooted_tree rt(t, u);
		rt.calculate_size_subtrees();
		S += lal::properties::exp_sum_edge_lengths_projective_rational(rt);
	}
	return correction_factor + S/n;
}

err_type exe_properties_expected_D_planar_quadratic
(const input_list& inputs, std::ifstream& fin)
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::string option;
	fin >> option;

	uint64_t n;
	if (option == "exhaustive") {
		while (fin >> n) {
			std::size_t idx = 0;
			lal::generate::all_ulab_free_trees gen(n);
			while (not gen.end()) {
				const auto t = gen.yield_tree();

				// linear time algorithm
				const lal::numeric::rational linear_algo =
					lal::properties::exp_sum_edge_lengths_planar_rational(t);

				// quadratic time algorithm
				const lal::numeric::rational quadratic_algo = quadratic_E_pr_D(t);

				if (linear_algo != quadratic_algo) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Value calculated with O(n) algorithm differs from the\n";
					std::cerr << "    value calculated with the O(n^2) algorithm.\n";
					std::cerr << "    Linear-time: " << linear_algo << '\n';
					std::cerr << "    Quadratic-time: " << quadratic_algo << '\n';
					std::cerr << "    For tree of index: " << idx << '\n';
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
				++idx;
			}
		}
	}
	else {
		uint64_t nrand;
		while (fin >> n >> nrand) {
			lal::generate::rand_ulab_free_trees gen(n, 1234);
			for (uint64_t i = 0; i < nrand; ++i) {
				const auto t = gen.yield_tree();

				// linear time algorithm
				const lal::numeric::rational linear_algo =
					lal::properties::exp_sum_edge_lengths_planar_rational(t);

				// quadratic time algorithm
				const lal::numeric::rational quadratic_algo = quadratic_E_pr_D(t);

				if (linear_algo != quadratic_algo) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Value calculated with O(n) algorithm differs from the\n";
					std::cerr << "    value calculated with the O(n^2) algorithm.\n";
					std::cerr << "    Linear-time: " << linear_algo << '\n';
					std::cerr << "    Quadratic-time: " << quadratic_algo << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
