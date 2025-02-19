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
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace properties {

err_type exe_properties_expected_D_bipartite(std::ifstream& fin) noexcept
{

	uint64_t n;
	fin >> n;

	lal::head_vector hv(n);
	while (fin >> hv[0]) {
		for (uint64_t i = 1; i < n; ++i) {
			fin >> hv[i];
		}

		lal::graphs::free_tree T =
			lal::graphs::from_head_vector_to_free_tree(hv).first;

		// calculate the same value with tree subsizes
		const lal::numeric::rational value_no_sizes =
			lal::properties::exp_sum_edge_lengths_bipartite_rational(T);

		std::string ground_truth_str;
		fin >> ground_truth_str;

		const lal::numeric::rational ground_truth(ground_truth_str);
		if (value_no_sizes != ground_truth) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Value calculated with algorithm does not "
						 "coincide with\n";
			std::cerr << "    ground truth value.\n";
			std::cerr << "    Algorithm: " << value_no_sizes << '\n';
			std::cerr << "    Ground truth: " << ground_truth << '\n';
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace properties
} // namespace tests
