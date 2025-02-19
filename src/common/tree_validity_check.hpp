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

#pragma once

// C++ includes
#include <string>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace tests {

/* FREE TREES */

enum class ftree_check : int8_t {
	not_a_tree,
	diff_n_verts,
	diff_n_edges,

	correct
};

inline std::string tree_check_to_string(const ftree_check& fc) noexcept
{
	switch (fc) {
	case ftree_check::not_a_tree: return "Input free tree is not a tree.";
	case ftree_check::diff_n_verts:
		return "Input free tree has an incorrect number of vertices.";
	case ftree_check::diff_n_edges:
		return "Input free tree has an incorrect number of edges.";
	default: return "Input free tree seems to be correct.";
	}
	return "";
}

inline ftree_check
test_validity_tree(const uint64_t n, const lal::graphs::free_tree& T) noexcept
{
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

enum class rtree_check : int8_t {
	not_a_tree,
	diff_n_verts,
	diff_n_edges,
	without_root,
	invalid_edges_orientation,

	correct
};

inline std::string tree_check_to_string(const rtree_check& fc) noexcept
{
	switch (fc) {
	case rtree_check::not_a_tree: return "Input rooted tree is not a tree.";
	case rtree_check::diff_n_verts:
		return "Input rooted tree has an incorrect number of vertices.";
	case rtree_check::diff_n_edges:
		return "Input rooted tree has an incorrect number of edges.";
	case rtree_check::without_root:
		return "Input rooted tree does not have a root.";
	case rtree_check::invalid_edges_orientation:
		return "Input rooted tree's edges are not oriented correctly.";
	default: return "Input rooted tree seems to be correct.";
	}
	return "";
}

inline rtree_check
test_validity_tree(const uint64_t n, const lal::graphs::rooted_tree& T) noexcept
{
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
	return rtree_check::correct;
}

} // namespace tests
