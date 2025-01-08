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
#include <map>

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/graph_utils.hpp"
#include "memory/graph_functions.hpp"

#define MOVE_UGRAPH
#define COPY_UGRAPH
#define MOVE_DGRAPH
#define COPY_DGRAPH
#define MOVE_UGRAPH_INTO_FTREE
#define COPY_UGRAPH_INTO_FTREE
#define MOVE_FTREE
#define COPY_FTREE
#define MOVE_RTREE
#define COPY_RTREE

namespace tests {
namespace memory {

void test_generate() noexcept {
	begin_function

	{
	begin_case;
	lal::generate::all_ulab_free_trees Gen(10);
	Gen.next();
	const lal::graphs::free_tree f = Gen.get_tree();
	}

	{
	begin_case;
	lal::generate::all_ulab_free_trees Gen(10);
	Gen.next();
	lal::graphs::free_tree f = Gen.get_tree();
	}

	{
	begin_case;
	lal::generate::all_ulab_rooted_trees Gen(10);
	Gen.next();
	const lal::graphs::rooted_tree f = Gen.get_tree();
	}

	{
	begin_case;
	lal::generate::all_ulab_rooted_trees Gen(10);
	Gen.next();
	lal::graphs::rooted_tree f = Gen.get_tree();
	}
}

err_type exe_memory_graphs(std::ifstream&) noexcept {

#if defined MOVE_UGRAPH
	test_move_undirected_graph();
#endif
#if defined COPY_UGRAPH
	test_copy_undirected_graph();
#endif

#if defined MOVE_DGRAPH
	test_move_directed_graph();
#endif
#if defined COPY_DGRAPH
	test_copy_directed_graph();
#endif

#if defined MOVE_UGRAPH_INTO_FTREE
	test_move_ugraph_into_ftree();
#endif
#if defined COPY_UGRAPH_INTO_FTREE
	test_copy_ugraph_into_ftree();
#endif

#if defined MOVE_FTREE
	test_move_free_tree();
#endif
#if defined COPY_FTREE
	test_copy_free_tree();
#endif

#if defined MOVE_RTREE
	test_move_rooted_tree();
#endif
#if defined COPY_RTREE
	test_copy_rooted_tree();
#endif

	test_generate();

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
