/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
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
#include <lal/graphs/output.hpp>
#include <lal/generate/rand_bipartite_arrangements.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/linarr/C/C.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/iterators/E_iterator.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace generate {

namespace bipartite {

static constexpr auto blue = lal::properties::bipartite_graph_coloring::blue;
static constexpr auto red = lal::properties::bipartite_graph_coloring::red;
void output_coloring(const lal::properties::bipartite_graph_coloring& c) noexcept {
	for (lal::node u = 0; u < c.size(); ++u) {
		std::cerr
			<< "    " << u << " -> ";

		if (c.get_color_of(u) == blue) {
			std::cerr << "blue";
		}
		else if (c.get_color_of(u) == red) {
			std::cerr << "red";
		}
		else {
			std::cerr << "??";
		}
		std::cerr << '\n';
	}
}

} // -- namespace bipartite

err_type exe_gen_arr_rand_bipartite(std::ifstream& fin) noexcept {
	uint64_t n, ntrees, nit;
	while (fin >> n >> ntrees >> nit) {
		// do 'ntrees' trees of 'n' vertices
		lal::generate::rand_ulab_free_trees TreeGen(n);

		for (uint64_t nt = 0; nt < ntrees; ++nt) {
			const lal::graphs::free_tree T = TreeGen.get_tree();
			const lal::properties::bipartite_graph_coloring c =
				lal::properties::bipartite_coloring(T);

			lal::generate::rand_bipartite_arrangements RandArr(T, 100);

			for (uint64_t it = 0; it < nit; ++it) {
				const lal::linear_arrangement& arr = RandArr.get_arrangement();

				// Do some sanity checks.
				if (not lal::linarr::is_bipartite(T, c, arr)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Random arrangement generated (get) is not bipartite:\n";
					std::cerr << "    Arrangement:     " << arr.direct_as_vector() << '\n';
					std::cerr << "    Inv Arrangement: " << arr.inverse_as_vector() << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << T << '\n';
					std::cerr << T.get_head_vector() << '\n';
					bipartite::output_coloring(c);
					return err_type::test_execution;
				}
			}

			for (uint64_t it = 0; it < nit; ++it) {
				const lal::linear_arrangement& arr = RandArr.yield_arrangement();

				// Do some sanity checks.
				if (not lal::linarr::is_bipartite(T, c, arr)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Random arrangement generated (yield) is not bipartite:\n";
					std::cerr << "    Arrangement:     " << arr.direct_as_vector() << '\n';
					std::cerr << "    Inv Arrangement: " << arr.inverse_as_vector() << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << T.get_head_vector() << '\n';
					bipartite::output_coloring(c);
					return err_type::test_execution;
				}
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
