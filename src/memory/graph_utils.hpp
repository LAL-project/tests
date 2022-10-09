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

#pragma once

// C++ includes
#include <iostream>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/iterators/E_iterator.hpp>

#include "memory/common_utils.hpp"

#define output_edges(n, g)												\
{																		\
	std::cout << MLINE << "Edges of '" << n << "':{";					\
	for (lal::iterators::E_iterator it(g); not it.end(); it.next()) {	\
		const auto e = it.get_edge();									\
		std::cout << " (" << e.first << "," << e.second << ")";			\
	}																	\
	std::cout << "}\n";													\
}

#define output_graph(n,t)															\
	std::cout << MLINE << n << ".get_num_nodes()= " << t.get_num_nodes() << '\n';	\
	std::cout << MLINE << n << ".get_num_edges()= " << t.get_num_edges() << '\n';	\
	output_edges(n, t)																\

#define output_free_tree_info(n,t)							\
	output_graph(n,t)										\
	for (lal::node u = 0; u < t.get_num_nodes(); ++u) {		\
	std::cout												\
		<< n << ".get_num_nodes_component(" << u << ")= "	\
		<< t.get_num_nodes_component(u) << '\n';			\
	}

#define output_rooted_tree_info(n,t)							\
	output_free_tree_info(n,t)									\
	if (t.has_root()) {											\
		std::cout << MLINE										\
			 << n << ".get_root()= "							\
			 << t.get_root() << '\n';							\
	}															\
	if (t.are_size_subtrees_valid()) {							\
		for (lal::node u = 0; u < t.get_num_nodes(); ++u) {		\
			std::cout << MLINE									\
				 << n << ".get_num_nodes_subtree(" << u << ")= "\
				 << t.get_num_nodes_subtree(u) << '\n';			\
		}														\
	}
