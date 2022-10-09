/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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

#include "graph_utils.hpp"

namespace tests {
namespace memory {

// NOTE: blank lines left intentionally. DO NOT REMOVE or the tests will
// fail with error (because the number lines of the new execution will not
// match the number lines in the base output).
//
//
//

void test_move_rooted_tree() noexcept {
	begin_function;

	// move constructor
	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 't2 <- t1'\n";
	lal::graphs::rooted_tree t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	t1.add_edges(std::vector<lal::edge>{lal::edge(3,1), lal::edge(3,2), lal::edge(3,0), lal::edge(2,4)});
	t1.set_root(3);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 't2 <- t1'\n";
	lal::graphs::rooted_tree t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	t1.add_edges(std::vector<lal::edge>{lal::edge(3,1), lal::edge(3,2), lal::edge(3,0), lal::edge(2,4)});
	t1.set_root(3);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 'v[0] <- t1'\n";
	std::vector<lal::graphs::rooted_tree> v;
	v.push_back(std::move(t1));
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}

	// move operator
	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 't2 <- t1'\n";
	lal::graphs::rooted_tree t2;
	t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	t1.add_edges(std::vector<lal::edge>{lal::edge(3,1), lal::edge(3,2), lal::edge(3,0), lal::edge(2,4)});
	t1.set_root(3);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 't2 <- t1'\n";
	lal::graphs::rooted_tree t2;
	t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case;
	lal::graphs::rooted_tree t1(5);
	t1.add_edges(std::vector<lal::edge>{lal::edge(3,1), lal::edge(3,2), lal::edge(3,0), lal::edge(2,4)});
	t1.set_root(3);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	std::cout << "Move constructor: 'v[0] <- t1'\n";
	std::vector<lal::graphs::rooted_tree> v;
	v.push_back(lal::graphs::rooted_tree());
	v[0] = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}
}

} // -- namespace memory
} // -- namespace tests
