/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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

// C includes
#include <assert.h>
#include <string.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/io/edge_list.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "exe_construction.hpp"

#define ASSERT_GRAPH_EQUAL_GRAPHS "equal_graphs"
#define ASSERT_GRAPH_NOT_EQUAL_GRAPHS "not_equal_graphs"
#define ASSERT_GRAPH_IS_NORMALISED "normalised"
#define ASSERT_GRAPH_NOT_NORMALISED "not_normalised"
#define ASSERT_GRAPH_EXISTS_EDGE "exists_edge"
#define ASSERT_GRAPH_NOT_EXISTS_EDGE "not_exists_edge"
#define ASSERT_GRAPH_NUM_NODES "num_nodes"
#define ASSERT_GRAPH_NUM_EDGES "num_edges"
#define ASSERT_GRAPH_EDGES_ARE "edges_are"
#define ASSERT_GRAPH_ELEMENTS_Q_ARE "elements_Q_are"
#define ASSERT_GRAPH_IS_UNDIRECTED "is_undirected"
#define ASSERT_GRAPH_NOT_UNDIRECTED "not_undirected"
#define ASSERT_GRAPH_IS_DIRECTED "is_directed"
#define ASSERT_GRAPH_NOT_DIRECTED "not_directed"
#define ASSERT_GRAPH_DEGREE "degree"
#define ASSERT_GRAPH_FULL_DEGREE "full_degree"
#define ASSERT_UGRAPH_NEIGHBOURS_ARE "neighbours_are"
#define ASSERT_DGRAPH_IN_NEIGHBOURS_ARE "in_neighbours_are"
#define ASSERT_DGRAPH_OUT_NEIGHBOURS_ARE "out_neighbours_are"
#define ASSERT_DGRAPH_IN_DEGREE "in_degree"
#define ASSERT_DGRAPH_OUT_DEGREE "out_degree"
#define ASSERT_TREE_IS_TREE "is_tree"
#define ASSERT_TREE_IS_NOT_TREE "is_not_tree"
#define ASSERT_TREE_CAN_ADD_EDGE "can_add_edge"
#define ASSERT_TREE_CANT_ADD_EDGE "cant_add_edge"
#define ASSERT_TREE_CAN_ADD_EDGES "can_add_edges"
#define ASSERT_TREE_CANT_ADD_EDGES "cant_add_edges"
#define ASSERT_TREE_SIZE_CC "num_nodes_component"
#define ASSERT_TREE_IS_ROOTED "is_rooted"
#define ASSERT_TREE_IS_NOT_ROOTED "is_not_rooted"
#define ASSERT_TREE_TYPE_VALID "tree_type_valid"
#define ASSERT_TREE_TYPE_NOT_VALID "tree_type_not_valid"
#define ASSERT_RTREE_ROOT_IS_VALID "root_is_valid"
#define ASSERT_RTREE_ROOT_IS_NOT_VALID "root_is_not_valid"
#define ASSERT_RTREE_HAS_ROOT "has_root"
#define ASSERT_RTREE_NOT_HAS_ROOT "not_has_root"
#define ASSERT_RTREE_ROOT_IS "root_is"
#define ASSERT_RTREE_SIZE_SUBTREE "num_nodes_subtree"
#define ASSERT_RTREE_SUBSTREE_SIZES_VALID "subtree_sizes_valid"
#define ASSERT_RTREE_SUBSTREE_SIZES_NOT_VALID "subtree_sizes_not_valid"
#define ASSERT_RTREE_SUBSTREE_CONTAINS_NODE "subtree_contains_node"
#define ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE "subtree_not_contains_node"

namespace tests {
namespace graphs {

err_type process_assert(
	const std::string& assert_what,
	std::map<std::string, lal::graphs::undirected_graph>& ugraphvars,
	std::map<std::string, lal::graphs::directed_graph>& dgraphvars,
	std::map<std::string, lal::graphs::free_tree>& ftreevars,
	std::map<std::string, lal::graphs::rooted_tree>& rtreevars,
	std::map<std::string, std::string>& gtypes,
	std::ifstream& fin
)
{
	std::string g1, g2;
	uint64_t n;
	lal::node u,v;

	if (assert_what == ASSERT_GRAPH_EQUAL_GRAPHS) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_GRAPH_EQUAL_GRAPHS, g1)
		assert_exists_variable(ASSERT_GRAPH_EQUAL_GRAPHS, g2)
		assert_equal_types(ASSERT_GRAPH_EQUAL_GRAPHS, g1, g2)
		if (not are_graphs_equal(g1, g2)) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_EQUAL_GRAPHS)
			std::cerr << "    Graphs '" << g1 << "' and '" << g2 << "' "
				 << "are not equal.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			std::cerr << "    Contents of " << g2 << ":\n";
			output_graph(g2);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_EQUAL_GRAPHS) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g1);
		assert_exists_variable(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g2);
		assert_equal_types(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g1, g2);
		if (are_graphs_equal(g1, g2)) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_EQUAL_GRAPHS);
			std::cerr << "    Graphs '" << g1 << "' and '" << g2 << "' "
				 << " are equal.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			std::cerr << "    Contents of " << g2 << ":\n";
			output_graph(g2);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_NORMALISED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_NORMALISED, g1);
		if (not mfunction(g1, is_normalised())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_IS_NORMALISED)
			std::cerr << "    Graph '" << g1 << "' is not normalised.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_NORMALISED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_NORMALISED, g1);
		if (mfunction(g1, is_normalised())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_NORMALISED);
			std::cerr << "    Graph '" << g1 << "' is normalised.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_EXISTS_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_EXISTS_EDGE, g1);
		if (not mfunction(g1, has_edge(u, v))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_EXISTS_EDGE);
			std::cerr << "    Graph '" << g1 << "' does not have edge "
				 << "(" << u << ", " << v << ").\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_EXISTS_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_NOT_EXISTS_EDGE, g1);
		if (mfunction(g1, has_edge(u, v))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_EXISTS_EDGE);
			std::cerr << "    Graph '" << g1 << "' has edge "
				 << "(" << u << ", " << v << ").\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NUM_NODES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_NUM_NODES, g1);
		if (mfunction(g1, get_num_nodes()) != n) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NUM_NODES);
			std::cerr << "    Graph '" << g1 << "' does not have " << n << " nodes.\n";
			std::cerr << "    Graph '" << g1 << "' has "
				 << mfunction(g1, get_num_nodes()) << " nodes.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NUM_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_NUM_EDGES, g1);
		if (mfunction(g1, get_num_edges()) != n) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NUM_EDGES);
			std::cerr << "    Graph '" << g1 << "' does not have "
				 << n << " edges.\n";
			std::cerr << "    Graph '" << g1 << "' has "
				 << mfunction(g1, get_num_edges()) << " edges.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_EDGES_ARE) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_EDGES_ARE, g1);
		std::vector<lal::edge> edge_list(n);
		for (lal::edge& e : edge_list) { fin >> e.first >> e.second; }
		sort(edge_list.begin(), edge_list.end());

		std::vector<lal::edge> gv = ffunction(g1, enumerate_edges_brute_force);
		sort(gv.begin(), gv.end());
		if (edge_list != gv) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_EDGES_ARE);
			std::cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list.\n";
			std::cerr << "    List (" << edge_list.size() << "):\n";
			for (auto e : edge_list) {
				std::cerr << "    " << edge_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					std::cerr << " <- not in the graph!";
				}
				std::cerr << '\n';
			}
			std::cerr << "    Graph (" << gv.size() << "):\n";
			for (auto e : gv) {
				std::cerr << "    " << edge_out(e) << '\n';
			}
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_ELEMENTS_Q_ARE) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_ELEMENTS_Q_ARE, g1);
		std::vector<lal::edge_pair> edge_pair_list(n);
		for (lal::edge_pair& e : edge_pair_list) {
			fin >> e.first.first >> e.first.second
				>> e.second.first >> e.second.second;
			assert(not share_vertices(e));
		}
		sort(edge_pair_list.begin(), edge_pair_list.end());

		std::vector<lal::edge_pair> gv = ffunction(g1, enumerate_Q_brute_force);
		sort(gv.begin(), gv.end());
		if (edge_pair_list != gv) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_ELEMENTS_Q_ARE);
			std::cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list.\n";
			std::cerr << "    List (" << edge_pair_list.size() << "):\n";
			for (auto e : edge_pair_list) {
				std::cerr << "    " << edge_pair_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					std::cerr << " <- not in the graph!";
				}
				std::cerr << '\n';
			}
			std::cerr << "    Graph (" << gv.size() << "):\n";
			for (auto e : gv) {
				std::cerr << "    " << edge_pair_out(e) << '\n';
			}
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_UNDIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_UNDIRECTED, g1);
		if (gtypes[g1] != UGRAPH) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_IS_UNDIRECTED);
			std::cerr << "    Graph '" << g1 << "' is not undirected.\n";
			return err_type::test_execution;
		}
		if (not mfunction(g1, is_undirected())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_IS_UNDIRECTED);
			std::cerr << "    Graph '" << g1 << "' does not return undirected."
				 << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_UNDIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_UNDIRECTED, g1);
		if (gtypes[g1] == UGRAPH) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_UNDIRECTED);
			std::cerr << "    Graph '" << g1 << "' is undirected.\n";
			return err_type::test_execution;
		}
		if (mfunction(g1, is_undirected())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_UNDIRECTED);
			std::cerr << "    Graph '" << g1 << "' returns undirected.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_DIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_DIRECTED, g1);
		if (gtypes[g1] != DGRAPH) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_IS_DIRECTED);
			std::cerr << "    Graph '" << g1 << "' is not directed.\n";
			return err_type::test_execution;
		}
		if (not mfunction(g1, is_directed())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_IS_DIRECTED);
			std::cerr << "    Graph '" << g1 << "' does not return directed."
				 << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_DIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_DIRECTED, g1);
		if (gtypes[g1] == DGRAPH) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_DIRECTED);
			std::cerr << "    Graph '" << g1 << "' is directed.\n";
			return err_type::test_execution;
		}
		if (mfunction(g1, is_directed())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_NOT_DIRECTED);
			std::cerr << "    Graph '" << g1 << "' returns directed.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_GRAPH_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_DEGREE, g1);
		if (mfunction(g1, get_degree(u)) != v) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_DEGREE);
			std::cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have degree " << v << '\n';
			std::cerr << "    The vertex has degree: " << mfunction(g1, get_degree(u))
				 << '\n';
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}

	// UNDIRECTED GRAPHS
	else if (assert_what == ASSERT_UGRAPH_NEIGHBOURS_ARE) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_UGRAPH_NEIGHBOURS_ARE, g1);
		assert_correct_graph_type(
			ASSERT_UGRAPH_NEIGHBOURS_ARE, graph_type(g1), undirected_types
		);

		std::vector<lal::node> node_list(n);
		for (lal::node& neigh : node_list) { fin >> neigh; }

		auto neighs = mfunction_undir_graphs(g1, get_neighbours(u));

		sort(neighs.begin(), neighs.end());
		sort(node_list.begin(), node_list.end());
		if (neighs != node_list) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_UGRAPH_NEIGHBOURS_ARE);
			std::cerr << "    The list of neighbours of lal::node " << u << " of graph "
				 << g1 << " does not coincide with input.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			std::cerr << "    Contents of list:\n";
			std::cerr << "   ";
			for (lal::node neigh : node_list) { std::cerr << " " << neigh; }
			std::cerr << '\n';
			return err_type::test_execution;
		}
	}

	// DIRECTED GRAPHS
	else if (assert_what == ASSERT_DGRAPH_IN_NEIGHBOURS_ARE) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE, g1);
		assert_correct_graph_type(
			ASSERT_DGRAPH_IN_NEIGHBOURS_ARE, graph_type(g1), directed_types
		);
		std::vector<lal::node> node_list(n);
		for (lal::node& neigh : node_list) { fin >> neigh; }
		sort(node_list.begin(), node_list.end());

		auto actual_neighs = mfunction_dir_graphs(g1, get_in_neighbours(u));
		sort(actual_neighs.begin(), actual_neighs.end());

		if (actual_neighs != node_list) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE);
			std::cerr << "    The list of neighbours of lal::node " << u << " of graph "
				 << g1 << " does not coincide with input.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			std::cerr << "    Contents of input list:\n";
			std::cerr << "   ";
			for (lal::node neigh : node_list) { std::cerr << " " << neigh; }
			std::cerr << '\n';
			std::cerr << "    Actual in-neighbours:\n";
			std::cerr << "   ";
			for (lal::node neigh : actual_neighs) { std::cerr << " " << neigh; }
			std::cerr << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_DGRAPH_OUT_NEIGHBOURS_ARE) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_DGRAPH_OUT_NEIGHBOURS_ARE, g1);
		assert_correct_graph_type(
			ASSERT_DGRAPH_OUT_NEIGHBOURS_ARE, graph_type(g1), directed_types
		);
		std::vector<lal::node> node_list(n);
		for (lal::node& neigh : node_list) { fin >> neigh; }
		sort(node_list.begin(), node_list.end());

		auto actual_neighs = mfunction_dir_graphs(g1, get_out_neighbours(u));
		sort(actual_neighs.begin(), actual_neighs.end());

		if (actual_neighs != node_list) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE);
			std::cerr << "    The list of neighbours of lal::node " << u << " of graph "
				 << g1 << " does not coincide with input.\n";
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			std::cerr << "    Contents of input list:\n";
			std::cerr << "   ";
			for (lal::node neigh : node_list) { std::cerr << " " << neigh; }
			std::cerr << '\n';
			std::cerr << "    Actual out-neighbours:\n";
			std::cerr << "   ";
			for (lal::node neigh : actual_neighs) { std::cerr << " " << neigh; }
			std::cerr << '\n';
			return err_type::test_execution;
		}
	}

	else if (assert_what == ASSERT_GRAPH_FULL_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_FULL_DEGREE, g1)
		assert_correct_graph_type(
			ASSERT_GRAPH_FULL_DEGREE, graph_type(g1), directed_types
		)
		const uint64_t fdegree =
			mfunction_dir_graphs(g1, get_degree(u));
		if (fdegree != v) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_GRAPH_FULL_DEGREE)
			std::cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have full degree " << v << '\n';
			std::cerr << "    The vertex has full degree: " << fdegree << '\n';
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_DGRAPH_IN_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_DGRAPH_IN_DEGREE, g1)
		assert_correct_graph_type(
			ASSERT_DGRAPH_IN_DEGREE, graph_type(g1), directed_types
		)
		if (mfunction_dir_graphs(g1, get_in_degree(u)) != v) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_DGRAPH_IN_DEGREE)
			std::cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have in-degree " << v << '\n';
			std::cerr << "    The vertex has in-degree: "
				 << mfunction_dir_graphs(g1, get_in_degree(u)) << '\n';
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_DGRAPH_OUT_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_DGRAPH_OUT_DEGREE, g1)
		assert_correct_graph_type(
			ASSERT_DGRAPH_OUT_DEGREE, graph_type(g1), directed_types
		)
		if (mfunction_dir_graphs(g1, get_out_degree(u)) != v) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_DGRAPH_OUT_DEGREE)
			std::cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have out-degree " << v << '\n';
			std::cerr << "    The vertex has out-degree: "
				 << mfunction_dir_graphs(g1, get_out_degree(u)) << '\n';
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}

	// TREES
	else if (assert_what == ASSERT_TREE_IS_TREE) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_TREE, g1)
		assert_correct_graph_type(
			ASSERT_TREE_IS_TREE, graph_type(g1), tree_types
		)
		if (not mfunction_trees(g1, is_tree())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_IS_TREE)
			std::cerr << "    Tree '" << g1 << "' is 'not a tree'.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_NOT_TREE) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_NOT_TREE, g1)
		assert_correct_graph_type(
			ASSERT_TREE_IS_NOT_TREE, graph_type(g1), tree_types
		)
		if (mfunction_trees(g1, is_tree())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_IS_TREE)
			std::cerr << "    Tree '" << g1 << "' is 'a tree'.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_CAN_ADD_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_TREE_CAN_ADD_EDGE, g1)
		assert_correct_graph_type(
			ASSERT_TREE_CAN_ADD_EDGE, graph_type(g1), tree_types
		)
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (not can) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_CAN_ADD_EDGE)
			std::cerr << "    Cannot add edge with vertices {" << u << "," << v << "} "
				 << "to graph '" << g1 << "'\n";
			std::cout << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_CANT_ADD_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_TREE_CANT_ADD_EDGE, g1)
		assert_correct_graph_type(
			ASSERT_TREE_CANT_ADD_EDGE, graph_type(g1), tree_types
		)
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (can) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_CANT_ADD_EDGE)
			std::cerr << "    Edge with vertices {" << u << "," << v << "} "
				 << "can be added to graph '" << g1 << "'\n";
			std::cout << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_CAN_ADD_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_TREE_CAN_ADD_EDGES, g1)
		assert_correct_graph_type(
			ASSERT_TREE_CAN_ADD_EDGES, graph_type(g1), tree_types
		)
		std::vector<lal::edge> edge_list(n);
		for (lal::edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (not can) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_CAN_ADD_EDGES)
			std::cerr << "    Cannot add edges\n";
			for (auto e : edge_list) {
			std::cerr << "        {" << e.first << "," << e.second << "}\n";
			}
			std::cerr << "    to graph '" << g1 << "'\n";
			std::cout << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_CANT_ADD_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_TREE_CANT_ADD_EDGES, g1)
		assert_correct_graph_type(
			ASSERT_TREE_CANT_ADD_EDGES, graph_type(g1), tree_types
		)
		std::vector<lal::edge> edge_list(n);
		for (lal::edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (can) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_CANT_ADD_EDGES)
			std::cerr << "    Cannot add edges\n";
			for (auto e : edge_list) {
			std::cerr << "        {" << e.first << "," << e.second << "}\n";
			}
			std::cerr << "    to graph '" << g1 << "'\n";
			std::cout << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_SIZE_CC) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_TREE_SIZE_CC, g1)
		if (mfunction_trees(g1, get_num_nodes_component(u)) != n) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_SIZE_CC)
			std::cerr << "    The component of vertex '" << u << "'"
				 << " in '" << g1 << "' does not have "
				 << n << " vertices.\n";
			std::cerr << "    It actually has: "
				 << mfunction_trees(g1, get_num_nodes_component(u))
				 << '\n';
			std::cerr << "    Contents of " << g1 << ":\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_ROOTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_ROOTED, g1)
		assert_correct_graph_type(
			ASSERT_TREE_IS_ROOTED, graph_type(g1), tree_types
		)
		if (not mfunction_trees(g1, is_rooted())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_IS_ROOTED)
			std::cerr << "    Graph '" << g1 << "' is not rooted.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_NOT_ROOTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_NOT_ROOTED, g1)
		assert_correct_graph_type(
			ASSERT_TREE_IS_NOT_ROOTED, graph_type(g1), tree_types
		)
		if (mfunction_trees(g1, is_rooted())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_IS_NOT_ROOTED)
			std::cerr << "    Graph '" << g1 << "' is rooted.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_TYPE_VALID) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_TYPE_VALID, g1)
		assert_correct_graph_type(
			ASSERT_TREE_TYPE_VALID, graph_type(g1), tree_types
		)
		if (not mfunction_trees(g1, is_tree_type_valid())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_TYPE_VALID)
			std::cerr << "    Tree type of tree '" << g1 << "' is not valid.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_TREE_TYPE_NOT_VALID) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_TYPE_NOT_VALID, g1)
		assert_correct_graph_type(
			ASSERT_TREE_TYPE_NOT_VALID, graph_type(g1), tree_types
		)
		if (mfunction_trees(g1, is_tree_type_valid())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_TREE_TYPE_NOT_VALID)
			std::cerr << "    Tree type of tree '" << g1 << "' is valid.\n";
			return err_type::test_execution;
		}
	}

	// ROOTED TREES
	else if (assert_what == ASSERT_RTREE_SUBSTREE_SIZES_VALID) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_SUBSTREE_SIZES_VALID, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_SUBSTREE_SIZES_VALID, graph_type(g1), rooted_tree_types
		)
		if (not mfunction_rtrees(g1, are_size_subtrees_valid())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_SIZES_VALID)
			std::cerr << "    Sizes of subtrees in tree '" << g1 << "' are not valid."
				 << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_SUBSTREE_SIZES_NOT_VALID) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_SUBSTREE_SIZES_NOT_VALID, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_SUBSTREE_SIZES_NOT_VALID, graph_type(g1), rooted_tree_types
		)
		if (mfunction_rtrees(g1, are_size_subtrees_valid())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_SIZES_NOT_VALID)
			std::cerr << "    Sizes of subtrees in tree '" << g1 << "' are valid."
				 << '\n';
			return err_type::test_execution;
		}
	}

	else if (assert_what == ASSERT_RTREE_ROOT_IS_VALID) {
		fin >> g1 >> u;
		assert_exists_variable(ASSERT_RTREE_ROOT_IS_VALID, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_ROOT_IS_VALID, graph_type(g1), rooted_tree_types
		)
		if (not mfunction_rtrees(g1, is_root_valid(u))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_ROOT_IS_VALID)
			std::cerr << "    Root '" << u << "' is not valid in tree:\n";
			std::cerr << rtreevars[g1] << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_ROOT_IS_NOT_VALID) {
		fin >> g1 >> u;
		assert_exists_variable(ASSERT_RTREE_ROOT_IS_NOT_VALID, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_ROOT_IS_NOT_VALID, graph_type(g1), rooted_tree_types
		)
		if (mfunction_rtrees(g1, is_root_valid(u))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_ROOT_IS_NOT_VALID)
			std::cerr << "    Root '" << u << "' is valid in tree:\n";
			std::cerr << rtreevars[g1] << '\n';
			return err_type::test_execution;
		}
	}

	else if (assert_what == ASSERT_RTREE_HAS_ROOT) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_HAS_ROOT, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_HAS_ROOT, graph_type(g1), rooted_tree_types
		)
		if (not mfunction_rtrees(g1, has_root())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_HAS_ROOT)
			std::cerr << "    Tree '" << g1 << "' does not have a root.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_NOT_HAS_ROOT) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_NOT_HAS_ROOT, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_NOT_HAS_ROOT, graph_type(g1), rooted_tree_types
		)
		if (mfunction_rtrees(g1, has_root())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_NOT_HAS_ROOT)
			std::cerr << "    Tree '" << g1 << "' has a root.\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_ROOT_IS) {
		fin >> g1 >> u;
		assert_exists_variable(ASSERT_RTREE_ROOT_IS, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_ROOT_IS, graph_type(g1), rooted_tree_types
		)
		const lal::node R = mfunction_rtrees(g1, get_root());
		if (R != u) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_ROOT_IS)
			std::cerr << "    The root of tree '" << g1 << "' is not " << u << ".\n";
			std::cerr << "    The root of tree '" << g1 << "' is " << R << ".\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_SIZE_SUBTREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_RTREE_SIZE_SUBTREE, g1)
		assert_correct_graph_type(
			ASSERT_RTREE_SIZE_SUBTREE, graph_type(g1), rooted_tree_types
		)
		assert_is_rtree(g1, ASSERT_RTREE_SIZE_SUBTREE)
		if (not mfunction_rtrees(g1, are_size_subtrees_valid())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SIZE_SUBTREE)
			std::cerr << "    Values of subtree's sizes in tree '"
				 << g1 << "' need recalculating.\n";
			return err_type::test_execution;
		}

		n = mfunction_rtrees(g1, get_num_nodes_subtree(u));
		if (n != v) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SIZE_SUBTREE)
			std::cerr << "    The size of subtree rooted at '" << u
				 << "' is not '" << v << "'.\n";
			std::cerr << "    The size of the subtree is: " << n << '\n';
			std::cerr << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_SUBSTREE_CONTAINS_NODE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_RTREE_SUBSTREE_CONTAINS_NODE, g1);
		assert_correct_graph_type
		(ASSERT_RTREE_SUBSTREE_CONTAINS_NODE, graph_type(g1), rooted_tree_types);

		assert_is_rtree(g1, ASSERT_RTREE_SUBSTREE_CONTAINS_NODE);
		if (not mfunction_rtrees(g1, is_rooted_tree())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_CONTAINS_NODE)
			std::cerr << "    Tree '" << g1 << "' is not a rooted tree.\n";
			return err_type::test_execution;
		}

		if (not mfunction_rtrees(g1, subtree_contains_node(u,v))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_CONTAINS_NODE);
			std::cerr
				<< "    The subtree of '" << g1 << "' rooted at '" << u
				<< "' does not contain '" << v << "'.\n";
			std::cerr << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else if (assert_what == ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE, g1);
		assert_correct_graph_type
		(ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE, graph_type(g1), rooted_tree_types);

		assert_is_rtree(g1, ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE);
		if (not mfunction_rtrees(g1, is_rooted_tree())) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE)
			std::cerr << "    Tree '" << g1 << "' is not a rooted tree.\n";
			return err_type::test_execution;
		}

		if (mfunction_rtrees(g1, subtree_contains_node(u,v))) {
			std::cerr << ERROR << '\n';
			message_in_func(ASSERT_RTREE_SUBSTREE_NOT_CONTAINS_NODE);
			std::cerr
				<< "    The subtree of '" << g1 << "' rooted at '" << u
				<< "' contains '" << v << "'.\n";
			std::cerr << "    Contents of '" << g1 << "':\n";
			output_graph(g1);
			return err_type::test_execution;
		}
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid assertion '" << assert_what << "'.\n";
		return err_type::test_format;
	}
	return err_type::no_error;
}

} // -- namespace graphs
} // -- namespace tests
