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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// lal includes
#include <lal/graphs.hpp>
using namespace lal;
using namespace graphs;

namespace exe_tests {

void test_class_hierarchy() {

	// -- TRUE POSITIVES

	// directed graphs
	static_assert(
		std::is_base_of<dgraph, dgraph>::value,
		"Class directed graph is not derived from directed graph"
	);
	static_assert(
		std::is_base_of<dgraph, dtree>::value,
		"Class directed tree is not derived from directed graph"
	);
	static_assert(
		std::is_base_of<dgraph, drtree>::value,
		"Class directed rooted tree is not derived from directed graph"
	);

	// undirected graphs
	static_assert(
		std::is_base_of<ugraph, ugraph>::value,
		"Class undirected graph is not derived from undirected graph"
	);
	static_assert(
		std::is_base_of<ugraph, utree>::value,
		"Class undirected tree is not derived from undirected graph"
	);
	static_assert(
		std::is_base_of<dgraph, drtree>::value,
		"Class undirected rooted tree is not derived from undirected graph"
	);

	// trees
	static_assert(
		std::is_base_of<graph, tree>::value,
		"Class tree is not derived from graph"
	);
	static_assert(
		std::is_base_of<tree, rtree>::value,
		"Class rooted tree is not derived from tree"
	);
	static_assert(
		std::is_base_of<tree, utree>::value,
		"Class undirected tree is not derived from tree"
	);
	static_assert(
		std::is_base_of<tree, dtree>::value,
		"Class directed tree is not derived from tree"
	);
	static_assert(
		std::is_base_of<rtree, urtree>::value,
		"Class undirected rooted tree is not derived from rooted tree"
	);
	static_assert(
		std::is_base_of<rtree, drtree>::value,
		"Class directed rooted tree is not derived from rooted tree"
	);
	static_assert(
		std::is_base_of<utree, urtree>::value,
		"Class undirected rooted tree is not derived from undirected tree"
	);
	static_assert(
		std::is_base_of<dtree, drtree>::value,
		"Class directed rooted tree is not derived from directed tree"
	);

	// -- TRUE NEGATIVES

	static_assert(
		!std::is_base_of<ugraph, tree>::value,
		"Abstract class tree is derived from undirected graph"
	);
	static_assert(
		!std::is_base_of<dgraph, tree>::value,
		"Abstract class tree is derived from directed graph"
	);
	static_assert(
		!std::is_base_of<ugraph, rtree>::value,
		"Abstract class rooted tree is derived from undirected graph"
	);
	static_assert(
		!std::is_base_of<dgraph, rtree>::value,
		"Abstract class rooted tree is derived from directed graph"
	);
	static_assert(
		!std::is_base_of<rtree, utree>::value,
		"Class undirected free tree is derived from rooted tree"
	);
	static_assert(
		!std::is_base_of<rtree, dtree>::value,
		"Class directed free tree is derived from rooted tree"
	);
}

} // -- namespace exe_tests
