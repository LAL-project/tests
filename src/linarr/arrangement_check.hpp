/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>

// lal includes
#include <lal/linarr/D.hpp>
#include <lal/linear_arrangement.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace linarr {

template <typename tree_t>
bool check_correctness_arr(
	const tree_t& tree,
	const std::pair<uint64_t, lal::linear_arrangement>& res,
	const std::string& error_header,
	const std::string& algorithm,
	bool (*check_arrangement)(const tree_t&, const lal::linear_arrangement&)
)
noexcept
{
	const lal::linear_arrangement& arr = res.second;

	/* ensure planarity of arrangement */
	if (not check_arrangement(tree, arr)) {
		std::cerr << error_header << '\n';
		std::cerr << "    The arrangement produced by the library is not an actual\n";
		std::cerr << "    Algorithm executed: " << algorithm << '\n';
		std::cerr << "    arrangement or is not planar.\n";
		std::cerr << "        Arrangement:     " << arr.direct_as_vector() << '\n';
		std::cerr << "        Inv Arrangement: " << arr.inverse_as_vector() << '\n';
		std::cerr << "    For tree: \n";
		std::cerr << "        Head vector: [" << tree.get_head_vector(0) << "]\n";
		std::cerr << "        Edge list: " << tree.get_edges() << '\n';
		std::cerr << tree << '\n';
		return false;
	}
	/* ensure that value of D is correct */
	const uint64_t D = lal::linarr::sum_edge_lengths(tree, arr);
	if (D != res.first) {
		std::cerr << error_header << '\n';
		std::cerr << "    Value of D returned by method is incorrect.\n";
		std::cerr << "    Algorithm executed: " << algorithm << '\n';
		std::cerr << "    Arrangement:     " << res.second.direct_as_vector() << '\n';
		std::cerr << "    Inv Arrangement: " << res.second.inverse_as_vector() << '\n';
		std::cerr << "    Value of D returned: " << res.first << '\n';
		std::cerr << "    Actual value of D:   " << D << '\n';
		std::cerr << "    For tree: \n";
		std::cerr << "        Head vector: [" << tree.get_head_vector(0) << "]\n";
		std::cerr << "        Edge list: " << tree.get_edges() << '\n';
		std::cerr << tree << '\n';
		return false;
	}
	return true;
}

} // -- namespace linarr
} // -- namespace tests
