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
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/properties/hierarchical_distance.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace properties {

err_type exe_properties_MHD_All_trees(std::ifstream& fin) noexcept
{

	uint64_t n;
	while (fin >> n) {
		std::cout << "----------------------------------------\n";
		std::cout << "n= " << n << '\n';

		int i = 0;
		lal::generate::all_ulab_free_trees TreeGen(n);
		while (not TreeGen.end()) {
			const lal::graphs::free_tree t = TreeGen.get_tree();
			TreeGen.next();

			std::cout << i << ")\n";

			// for each lal::node, make a rooted tree at that lal::node
			for (lal::node r = 0; r < t.get_num_nodes(); ++r) {
				const lal::graphs::rooted_tree R(t, r);
				const lal::numeric::rational mhd =
					lal::properties::mean_hierarchical_distance_rational(R);
				std::cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd
						  << '\n';
			}

			++i;
		}
	}

	return err_type::no_error;
}

} // namespace properties
} // namespace tests
