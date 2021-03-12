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
#include <vector>

// lal includes
#include <lal/definitions.hpp>

namespace exe_tests {

template<class G>
inline uint32_t number_of_crossings_brute_force(
	const G& g,
	const lal::linear_arrangement& pi
)
{
	const std::vector<lal::edge> edges = g.get_edges();
	uint32_t C = 0;

#define sorted_by_pos(A,B) \
	(pi[A] < pi[B] ? std::make_pair(pi[A],pi[B]) : std::make_pair(pi[B],pi[A]))

	for (size_t i = 0; i < edges.size(); ++i) {
		const auto& [s,t] = edges[i];
		const auto [a,b] = sorted_by_pos(s,t);

		for (size_t j = i + 1; j < edges.size(); ++j) {
			const auto& [u,v] = edges[j];
			const auto [c,d] = sorted_by_pos(u,v);

			C += (c < a and a < d and d < b) or (a < c and c < b and b < d);
		}
	}

#undef sorted_by_pos
	return C;
}

template<class G>
inline std::vector<uint32_t> number_of_crossings_brute_force(
	const G& g,
	const std::vector<lal::linear_arrangement>& pis
)
{
	std::vector<uint32_t> C(pis.size());
	for (size_t i = 0; i < pis.size(); ++i) {
		C[i] = number_of_crossings_brute_force(g, pis[i]);
	}
	return C;
}

} // -- exe_tests
