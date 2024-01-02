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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/linear_arrangement.hpp>

namespace tests {
namespace linarr {

template <class graph_t>
uint64_t num_crossings_brute_force(
	const graph_t& g,
	const lal::linear_arrangement& pi
)
noexcept
{
	const std::vector<lal::edge> edges = g.get_edges();
	uint64_t C = 0;

#define sorted_by_pos(pA,pB) \
	(pA < pB ? std::make_pair(pA,pB) : std::make_pair(pB,pA))

	for (std::size_t i = 0; i < edges.size(); ++i) {
		const lal::node_t s = edges[i].first;
		const lal::node_t t = edges[i].second;
		const auto [a,b] = sorted_by_pos(pi[s], pi[t]);

		for (std::size_t j = i + 1; j < edges.size(); ++j) {
			const lal::node_t u = edges[j].first;
			const lal::node_t v = edges[j].second;
			const auto [c,d] = sorted_by_pos(pi[u],pi[v]);

			C += (c < a and a < d and d < b) or (a < c and c < b and b < d);
		}
	}

#undef sorted_by_pos
	return C;
}

template <class graph_t>
std::vector<uint64_t> num_crossings_brute_force(
	const graph_t& g,
	const std::vector<lal::linear_arrangement>& pis
)
noexcept
{
	std::vector<uint64_t> C(pis.size());
	for (std::size_t i = 0; i < pis.size(); ++i) {
		C[i] = num_crossings_brute_force(g, pis[i]);
	}
	return C;
}

} // -- namespace linarr
} // -- namespace tests
