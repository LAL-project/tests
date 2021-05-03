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
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace generate;
using namespace graphs;
using namespace numeric;
using namespace properties;

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace properties {

err_type exe_properties_MHD_All_trees(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	uint32_t n;
	while (fin >> n) {
		cout << "----------------------------------------" << endl;
		cout << "n= " << n << endl;

		int i = 0;
		all_ulab_free_trees TreeGen(n);
		while (not TreeGen.end()) {
			const free_tree t = TreeGen.get_tree();
			TreeGen.next();

			cout << i << ")" << endl;

			// for each node, make a rooted tree at that node
			for (node r = 0; r < t.get_num_nodes(); ++r) {
				const rooted_tree R(t, r);
				const rational mhd =
					lal::properties::mean_hierarchical_distance_rational(R);
				cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << endl;
			}

			++i;
		}
	}

	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
