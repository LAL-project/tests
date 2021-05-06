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
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/time.hpp"

namespace tests {
namespace properties {

err_type exe_properties_expected_D_projective(const input_list& inputs, ifstream& fin)
{
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	uint32_t n;
	fin >> n;

	head_vector hv(n);
	while (fin >> hv[0]) {
		for (uint32_t i = 1; i < n; ++i) { fin >> hv[i]; }

		rooted_tree T = from_head_vector_to_rooted_tree(hv);

		// calculate the same value with tree subsizes
		const rational value_no_sizes =
			lal::properties::exp_sum_edge_lengths_projective_rational(T);

		// calculate the same value with tree subsizes
		T.calculate_size_subtrees();
		const rational value_sizes =
			lal::properties::exp_sum_edge_lengths_projective_rational(T);

		if (value_no_sizes != value_sizes) {
			cerr << ERROR << endl;
			cerr << "    Value computed with tree subsizes in tree" << endl;
			cerr << "    differs from the value computed without tree" << endl;
			cerr << "    subsizes in the tree." << endl;
			cerr << "    With tree subsizes in tree: " << value_sizes << endl;
			cerr << "    Without tree subsizes in tree: " << value_no_sizes << endl;
			return err_type::test_execution;
		}

		string ground_truth_str;
		fin >> ground_truth_str;

		const rational ground_truth(ground_truth_str);
		if (value_no_sizes != ground_truth) {
			cerr << ERROR << endl;
			cerr << "    Value calculated with algorithm does not coincide with" << endl;
			cerr << "    ground truth value." << endl;
			cerr << "    Algorithm: " << value_no_sizes << endl;
			cerr << "    Ground truth: " << ground_truth << endl;
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
