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

#include "generate/tree_validity_check.hpp"

// C++ includes
#include <iostream>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;

namespace exe_tests {

/* FREE TREES */

string ftree_check_to_string(const ftree_check& fc) {
	switch (fc) {
	case ftree_check::not_a_tree: return "Input free tree is not a tree.";
	case ftree_check::diff_n_verts: return "Input free tree has an incorrect number of vertices.";
	case ftree_check::diff_n_edges: return "Input free tree has an incorrect number of edges.";
	default:
		return "Input free tree seems to be correct.";
	}
	return "";
}

ftree_check test_validity_tree(const uint32_t n, const free_tree& T) {
	if (not T.is_tree()) {
		return ftree_check::not_a_tree;
	}
	if (T.get_num_nodes() != n) {
		return ftree_check::diff_n_verts;
	}
	if (T.get_num_edges() != n - 1) {
		return ftree_check::diff_n_edges;
	}
	return ftree_check::correct;
}

/* ROOTED TREES */

string rtree_check_to_string(const rtree_check& fc) {
	switch (fc) {
	case rtree_check::not_a_tree: return "Input rooted tree is not a tree.";
	case rtree_check::diff_n_verts: return "Input rooted tree has an incorrect number of vertices.";
	case rtree_check::diff_n_edges: return "Input rooted tree has an incorrect number of edges.";
	case rtree_check::without_root: return "Input rooted tree does not have a root.";
	case rtree_check::invalid_edges_orientation: return "Input rooted tree's edges are not oriented correctly.";
	default:
		return "Input rooted tree seems to be correct.";
	}
	return "";
}

rtree_check test_validity_tree(const uint32_t n, const rooted_tree& T) {
	if (not T.is_tree()) {
		return rtree_check::not_a_tree;
	}
	if (T.get_num_nodes() != n) {
		return rtree_check::diff_n_verts;
	}
	if (T.get_num_edges() != n - 1) {
		return rtree_check::diff_n_edges;
	}
	if (not T.has_root()) {
		return rtree_check::without_root;
	}
	if (not T.is_orientation_valid()) {
		return rtree_check::invalid_edges_orientation;
	}
	return rtree_check::correct;
}

} // -- namespace exe_tests
