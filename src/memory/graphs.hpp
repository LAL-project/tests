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

#pragma once

// C++ includes
#include <iostream>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/iterators/E_iterator.hpp>

#define MLINE __LINE__ << ":: "

#define begin_function										\
	cout << "+++++++++++++++++++++++++++++++++" << endl;	\
	cout << "function: " << __PRETTY_FUNCTION__ << endl;

#define begin_case											\
	cout << "-----------------------------------" << endl;	\
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

#define output_graph(n,t)												\
	cout << MLINE << n << ".num_nodes()= " << t.num_nodes() << endl;	\
	cout << MLINE << n << ".num_edges()= " << t.num_edges() << endl;	\
	output_edges(n, t)													\

#define output_free_tree_info(n,t)									\
	output_graph(n,t)												\
	for (node u = 0; u < t.num_nodes(); ++u) {						\
	cout << MLINE													\
		 << n << ".num_nodes_component(" << u << ")= "				\
		 << t.num_nodes_component(u) << endl;						\
	}

#define output_rooted_tree_info(n,t)							\
	output_free_tree_info(n,t)									\
	if (t.has_root()) {											\
		cout << MLINE											\
			 << n << ".get_root()= "							\
			 << t.get_root() << endl;							\
	}															\
	if (t.size_subtrees_valid()) {								\
		for (node u = 0; u < t.num_nodes(); ++u) {				\
			cout << MLINE										\
				 << n << ".num_nodes_subtree(" << u << ")= "	\
				 << t.num_nodes_subtree(u) << endl;				\
		}														\
	}

namespace exe_tests {

void test_move_undirected_graph();
void test_copy_undirected_graph();

void test_move_directed_graph();
void test_copy_directed_graph();

void test_move_ugraph_into_ftree();
void test_copy_ugraph_into_ftree();

void test_move_free_tree();
void test_copy_free_tree();

void test_move_rooted_tree();
void test_copy_rooted_tree();

} // -- namespace exe_tests
