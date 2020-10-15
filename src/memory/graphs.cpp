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
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

// lal includes
#include <lal/graphs.hpp>
#include <lal/iterators.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
using namespace lal;
using namespace graphs;

// custom includes
#include "definitions.hpp"

#define MLINE __LINE__ << ":: "

#define begin_function								\
	cout << "++++++++++++++++++++" << endl;			\
	cout << "function: " << __PRETTY_FUNCTION__ << endl;

#define begin_case									\
	cout << "--------------------" << endl;			\
	cout << "case starting at line: " << __LINE__ << endl;

#define output_edges(n, g)									\
{															\
	iterators::E_iterator it(g);							\
	cout << MLINE << "Edges of '" << n << "':{";			\
	while (it.has_next()) {									\
		it.next();											\
		const edge e = it.get_edge();						\
		cout << " (" << e.first << "," << e.second << ")";	\
	}														\
	cout << "}" << endl;									\
}

#define output_graph(n,t)											\
	cout << MLINE << n << ".n_nodes()= " << t.n_nodes() << endl;	\
	cout << MLINE << n << ".n_edges()= " << t.n_edges() << endl;	\
	output_edges(n, t)												\

#define output_free_tree_info(n,t)									\
	output_graph(n,t)												\
	for (node u = 0; u < t.n_nodes(); ++u) {						\
	cout << MLINE													\
		 << n << ".n_nodes_component(" << u << ")= "				\
		 << t.n_nodes_component(u) << endl;							\
	}

#define output_rooted_tree_info(n,t)							\
	output_free_tree_info(n,t)									\
	if (t.has_root()) {											\
		cout << MLINE											\
			 << n << ".get_root()= "							\
			 << t.get_root() << endl;							\
	}															\
	if (t.size_subtrees_valid()) {								\
		for (node u = 0; u < t.n_nodes(); ++u) {				\
			cout << MLINE										\
				 << n << ".n_nodes_subtree(" << u << ")= "		\
				 << t.n_nodes_subtree(u) << endl;				\
		}														\
	}

namespace exe_tests {

void test_move_undirected_graph() {
	begin_function

	// move constructor
	{
	begin_case
	undirected_graph g1(10);
	output_graph("g1",g1)

	cout << "Move constructor: 'g2 <- g1'" << endl;
	undirected_graph g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move constructor: 'g2 <- g1'" << endl;
	undirected_graph g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move constructor: 'v[0] <- g1'" << endl;
	vector<undirected_graph> v;
	v.push_back(std::move(g1));
	output_graph("g1",g1)
	output_graph("v[0]",v[0])
	}

	// move operator
	{
	begin_case
	undirected_graph g1(10);
	output_graph("g1",g1)

	cout << "Move operator: 'g2 <- g1'" << endl;
	undirected_graph g2;
	g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move operator: 'g2 <- g1'" << endl;
	undirected_graph g2;
	g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move operator: 'v[0] <- g1'" << endl;
	vector<undirected_graph> v;
	v.push_back(undirected_graph());
	v[0] = std::move(g1);
	output_graph("g1",g1)
	output_graph("v[0]",v[0])
	}
}

void test_copy_undirected_graph() {
	begin_function

	// copy constructor
	{
	begin_case
	undirected_graph g1(10);
	output_graph("g1",g1)

	cout << "Copy constructor: 'g2 <- g1'" << endl;
	undirected_graph g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy constructor: 'g2 <- g1'" << endl;
	undirected_graph g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy constructor: 'v[0] <- g1'" << endl;
	vector<undirected_graph> v;
	v.push_back(g1);
	output_graph("g1",g1)
	output_graph("v[0]",v[0])
	}

	// copy operator
	{
	begin_case
	undirected_graph g1(10);
	output_graph("g1",g1)

	cout << "Copy operator: 'g2 <- g1'" << endl;
	undirected_graph g2;
	g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy operator: 'g2 <- g1'" << endl;
	undirected_graph g2;
	g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy operator: 'v[0] <- g1'" << endl;
	vector<undirected_graph> v;
	v.push_back(undirected_graph());
	v[0] = g1;
	output_graph("g1",g1)
	output_graph("v[0]",v[0])
	}
}

void test_move_directed_graph() {
	begin_function

	// move constructor
	{
	begin_case
	directed_graph g1(10);
	output_graph("g1",g1)

	cout << "Move constructor: 'g2 <- g1'" << endl;
	directed_graph g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move constructor: 'g2 <- g1'" << endl;
	directed_graph g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move constructor: 'v[0] <- g1'" << endl;
	vector<directed_graph> v;
	v.push_back(std::move(g1));
	output_graph("g1",g1)
	output_graph("v[0]", v[0])
	}

	// move operator
	{
	begin_case
	directed_graph g1(10);
	output_graph("g1",g1)

	cout << "Move operator: 'g2 <- g1'" << endl;
	directed_graph g2;
	g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move operator: 'g2 <- g1'" << endl;
	directed_graph g2;
	g2 = std::move(g1);
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Move operator: 'v[0] <- g1'" << endl;
	vector<directed_graph> v;
	v.push_back(directed_graph());
	v[0] = std::move(g1);
	output_graph("g1",g1)
	output_graph("v[0]", v[0])
	}
}

void test_copy_directed_graph() {
	begin_function

	// copy constructor
	{
	begin_case
	directed_graph g1(10);
	output_graph("g1",g1)

	cout << "Copy constructor: 'g2 <- g1'" << endl;
	directed_graph g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy constructor: 'g2 <- g1'" << endl;
	directed_graph g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy constructor: 'v[0] <- g1'" << endl;
	vector<directed_graph> v;
	v.push_back(g1);
	output_graph("g1",g1)
	output_graph("v[0]", v[0])
	}

	// copy operator
	{
	begin_case
	directed_graph g1(10);
	output_graph("g1",g1)

	cout << "Copy operator: 'g2 <- g1'" << endl;
	directed_graph g2;
	g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy operator: 'g2 <- g1'" << endl;
	directed_graph g2;
	g2 = g1;
	output_graph("g1",g1)
	output_graph("g2",g2)
	}

	{
	begin_case
	directed_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,2), edge(1,5), edge(2,4)});
	output_graph("g1",g1)

	cout << "Copy operator: 'v[0] <- g1'" << endl;
	vector<directed_graph> v;
	v.push_back(directed_graph());
	v[0] = g1;
	output_graph("g1",g1)
	output_graph("v[0]", v[0])
	}
}

void test_move_free_trees() {
	begin_function

	// move constructor
	{
	begin_case
	free_tree t1(5);
	output_free_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	free_tree t2 = std::move(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(5);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4)});
	output_free_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	free_tree t2 = std::move(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(6);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4), edge(3,5)});
	output_free_tree_info("t1", t1)

	cout << "Move constructor: 'v[0] <- t1'" << endl;
	vector<free_tree> v;
	v.push_back(std::move(t1));
	output_free_tree_info("t1", t1)
	output_free_tree_info("v[0]", v[0])
	}

	// move operator
	{
	begin_case
	free_tree t1(5);
	output_free_tree_info("t1", t1)

	cout << "Move operator: 't2 <- t1'" << endl;
	free_tree t2;
	t2 = std::move(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(5);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4)});
	output_free_tree_info("t1", t1)

	cout << "Move operator: 't2 <- t1'" << endl;
	free_tree t2;
	t2 = std::move(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(6);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4), edge(3,5)});
	output_free_tree_info("t1", t1)

	cout << "Move operator: 'v[0] <- t1'" << endl;
	vector<free_tree> v;
	v.push_back(free_tree());
	v[0] = std::move(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("v[0]", v[0])
	}
}

void test_copy_free_trees() {
	begin_function

	// copy constructor
	{
	begin_case
	free_tree t1(5);
	output_free_tree_info("t1", t1)

	cout << "Copy constructor: 't2 <- t1'" << endl;
	free_tree t2 = t1;
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(5);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4)});
	output_free_tree_info("t1", t1)

	cout << "Copy constructor: 't2 <- t1'" << endl;
	free_tree t2 = t1;
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(6);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4), edge(3,5)});
	output_free_tree_info("t1", t1)

	cout << "Copy constructor: 'v[0] <- t1'" << endl;
	vector<free_tree> v;
	v.push_back(t1);
	output_free_tree_info("t1", t1)
	output_free_tree_info("v[0]", v[0])
	}

	// copy operator
	{
	begin_case
	free_tree t1(5);
	output_free_tree_info("t1", t1)

	cout << "Copy operator: 't2 <- t1'" << endl;
	free_tree t2;
	t2 = t1;
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(5);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4)});
	output_free_tree_info("t1", t1)

	cout << "Copy operator: 't2 <- t1'" << endl;
	free_tree t2;
	t2 = t1;
	output_free_tree_info("t1", t1)
	output_free_tree_info("t2", t2)
	}

	{
	begin_case
	free_tree t1(6);
	t1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(0,3), edge(2,4), edge(3,5)});
	output_free_tree_info("t1", t1)

	cout << "Copy operator: 'v[0] <- t1'" << endl;
	vector<free_tree> v;
	v.push_back(free_tree());
	v[0] = t1;
	output_free_tree_info("t1", t1)
	output_free_tree_info("v[0]", v[0])
	}
}

void test_move_rooted_trees() {
	begin_function

	// move constructor
	{
	begin_case
	rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	rooted_tree t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	rooted_tree t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 'v[0] <- t1'" << endl;
	vector<rooted_tree> v;
	v.push_back(std::move(t1));
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}

	// move operator
	{
	begin_case
	rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	rooted_tree t2;
	t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 't2 <- t1'" << endl;
	rooted_tree t2;
	t2 = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Move constructor: 'v[0] <- t1'" << endl;
	vector<rooted_tree> v;
	v.push_back(rooted_tree());
	v[0] = std::move(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}
}

void test_copy_rooted_trees() {
	begin_function

	// copy constructor
	{
	begin_case
	rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	cout << "Copy constructor: 't2 <- t1'" << endl;
	rooted_tree t2 = t1;
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Copy constructor: 't2 <- t1'" << endl;
	rooted_tree t2 = t1;
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Copy constructor: 'v[0] <- t1'" << endl;
	vector<rooted_tree> v;
	v.push_back(t1);
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}

	// copy operator
	{
	begin_case
	rooted_tree t1(5);
	output_rooted_tree_info("t1", t1)

	cout << "Copy operator: 't2 <- t1'" << endl;
	rooted_tree t2;
	t2 = t1;
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Copy operator: 't2 <- t1'" << endl;
	rooted_tree t2;
	t2 = t1;
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("t2", t2)
	}

	{
	begin_case
	rooted_tree t1(5);
	t1.add_edges(vector<edge>{edge(3,1), edge(3,2), edge(3,0), edge(2,4)});
	t1.set_root(3);
	t1.set_valid_orientation(true);
	t1.calculate_size_subtrees();
	output_rooted_tree_info("t1", t1)

	cout << "Copy operator: 'v[0] <- t1'" << endl;
	vector<rooted_tree> v;
	v.push_back(rooted_tree());
	v[0] = t1;
	output_rooted_tree_info("t1", t1)
	output_rooted_tree_info("v[0]", v[0])
	}
}

void test_generate() {
	begin_function

	{
	begin_case
	generate::all_ulab_free_trees Gen(10);
	Gen.next();
	const free_tree f = Gen.get_tree();
	}

	{
	begin_case
	generate::all_ulab_free_trees Gen(10);
	Gen.next();
	free_tree f = Gen.get_tree();
	}

	{
	begin_case
	generate::all_ulab_rooted_trees Gen(10);
	Gen.next();
	const rooted_tree f = Gen.get_tree();
	}

	{
	begin_case
	generate::all_ulab_rooted_trees Gen(10);
	Gen.next();
	rooted_tree f = Gen.get_tree();
	}
}

err_type exe_memory_graphs(const input_list& inputs, ifstream&) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	test_move_undirected_graph();
	test_copy_undirected_graph();
	test_move_directed_graph();
	test_copy_directed_graph();
	test_move_free_trees();
	test_copy_free_trees();
	test_move_rooted_trees();
	test_copy_rooted_trees();
	test_generate();

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
