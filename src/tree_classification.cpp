/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
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
using namespace std;

// lal includes
#include <lal/graphs/tree.hpp>
using namespace lal;
using namespace graphs;

namespace exe_tests {

bool is_caterpillar(const tree& t) {
	// number of vertices
	const uint32_t N = t.n_nodes();

	// number of internal vertices
	uint32_t n_internal = 0;
	// degree of the internal vertices
	vector<uint32_t> deg_internal(N, 0);
	for (node u = 0; u < N; ++u) {
		if (t.degree(u) > 1) {
			deg_internal[u] = t.degree(u);
			++n_internal;
		}
	}

	// reduce the degree of the internal vertices
	// as many times as leaves are connected to them
	for (node u = 0; u < N; ++u) {
		if (t.degree(u) == 1) {
			deg_internal[ t.get_neighbours(u)[0] ] -= 1;
		}
	}

	// If we are left with 2, or 0, vertices with degree 1,
	// it means that after removing the leaves of the tree
	// all vertices are internal (degree 2), i.e., they are
	// part of a linear tree. Needless to say that these
	// two vertices of degree 1 are the endpoints of the
	// linear tree.
	uint32_t n1 = 0;
	for (node u = 0; u < N; ++u) {
		n1 += deg_internal[u] == 1;
	}
	return n1 == 2 or n1 == 0;
}

} // -- namespace exe_tests
