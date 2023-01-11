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

// common includes
#include "exe_construction.hpp"
#include "common/test_utils.hpp"
#include "common/io_wrapper.hpp"

// lal includes
#include <lal/graphs/tree_literals.hpp>
#include <lal/graphs/output.hpp>

namespace tests {
namespace graphs {

err_type exe_graphs_graph_literals(std::ifstream&) noexcept {

	{
	std::cout << "Free tree and root (" << __LINE__ << ")\n";
	const std::pair<lal::graphs::free_tree, lal::node> ftree_and_root
			= "0 1 1 1 1 1"_root_free_tree;
	std::cout << ftree_and_root.first << '\n';
	std::cout << ftree_and_root.second << '\n';
	}

	{
	std::cout << "Free tree and root (" << __LINE__ << ")\n";
	const std::pair<lal::graphs::free_tree, lal::node> ftree_and_root
			= "{0 1} {0 2} {0 3} {0 4} {0 5}"_root_free_tree;
	std::cout << ftree_and_root.first << '\n';
	std::cout << ftree_and_root.second << '\n';
	}

	{
	std::cout << "Free tree and root (" << __LINE__ << ")\n";
	const std::pair<lal::graphs::free_tree, lal::node> ftree_and_root
			= "0 1 2 3 4 5"_root_free_tree;
	std::cout << ftree_and_root.first << '\n';
	std::cout << ftree_and_root.second << '\n';
	}

	{
	std::cout << "Free tree and root (" << __LINE__ << ")\n";
	const std::pair<lal::graphs::free_tree, lal::node> ftree_and_root
			= "{0 1} {1 2} {2 3} {3 4} {4 5}"_root_free_tree;
	std::cout << ftree_and_root.first << '\n';
	std::cout << ftree_and_root.second << '\n';
	}

	{
	std::cout << "Free tree (" << __LINE__ << ")\n";
	const lal::graphs::free_tree ftree = "0 1 1 1 1 1"_free_tree;
	std::cout << ftree << '\n';
	}

	{
	std::cout << "Free tree (" << __LINE__ << ")\n";
	const lal::graphs::free_tree ftree = "{0 1} {0 2} {0 3} {0 4} {0 5}"_free_tree;
	std::cout << ftree << '\n';
	}

	{
	std::cout << "Free tree (" << __LINE__ << ")\n";
	const lal::graphs::free_tree ftree = "0 1 2 3 4 5"_free_tree;
	std::cout << ftree << '\n';
	}

	{
	std::cout << "Free tree (" << __LINE__ << ")\n";
	const lal::graphs::free_tree ftree
			= "{0 1} {1 2} {2 3} {3 4} {4 5}"_free_tree;
	std::cout << ftree << '\n';
	}

	{
	std::cout << "Rooted tree (" << __LINE__ << ")\n";
	const lal::graphs::rooted_tree rtree = "0 1 1 1 1 1"_rooted_tree;
	std::cout << rtree << '\n';
	}

	{
	std::cout << "Rooted tree (" << __LINE__ << ")\n";
	const lal::graphs::rooted_tree rtree = "0 1 1 1 1 1"_rooted_tree;
	std::cout << rtree << '\n';
	}

	{
	std::cout << "Rooted tree (" << __LINE__ << ")\n";
	const lal::graphs::rooted_tree rtree = "0 1 2 3 4 5"_rooted_tree;
	std::cout << rtree << '\n';
	}

	{
	std::cout << "Rooted tree (" << __LINE__ << ")\n";
	const lal::graphs::rooted_tree rtree
			= "{0 1} {1 2} {2 3} {3 4} {4 5}"_rooted_tree;
	std::cout << rtree << '\n';
	}

	return err_type::no_error;
}

} // -- namespace graphs
} // -- namespace tests
