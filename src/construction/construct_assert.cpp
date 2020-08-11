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
using namespace std;

// lal includes
#include <lal/properties/Q.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/io/edge_list.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace iterators;

// Custom includes
#include "exe_construction.hpp"

#define ASSERT_GRAPH_EQUAL_GRAPHS "equal_graphs"
#define ASSERT_GRAPH_NOT_EQUAL_GRAPHS "not_equal_graphs"
#define ASSERT_GRAPH_NEIGHBOURS_ARE "neighbours_are"
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
#define ASSERT_DGRAPH_IN_NEIGHBOURS_ARE "in_neighbours_are"
#define ASSERT_GRAPH_FULL_DEGREE "full_degree"
#define ASSERT_DGRAPH_IN_DEGREE "in_degree"
#define ASSERT_DGRAPH_OUT_DEGREE "out_degree"
#define ASSERT_TREE_IS_TREE "is_tree"
#define ASSERT_TREE_IS_NOT_TREE "is_not_tree"
#define ASSERT_TREE_CAN_ADD_EDGE "can_add_edge"
#define ASSERT_TREE_CANT_ADD_EDGE "cant_add_edge"
#define ASSERT_TREE_CAN_ADD_EDGES "can_add_edges"
#define ASSERT_TREE_CANT_ADD_EDGES "cant_add_edges"
#define ASSERT_TREE_IS_ROOTED "is_rooted"
#define ASSERT_TREE_IS_NOT_ROOTED "is_not_rooted"
#define ASSERT_RTREE_NO_NEED_RECALC_SUBSTREE_SIZE "no_need_recalc_subtree_size"
#define ASSERT_RTREE_NEED_RECALC_SUBSTREE_SIZE "need_recalc_subtree_size"
#define ASSERT_RTREE_HAS_ROOT "has_root"
#define ASSERT_RTREE_NOT_HAS_ROOT "not_has_root"
#define ASSERT_RTREE_SIZE_SUBTREE "num_nodes_subtree"
#define ASSERT_rooted_tree_type_valid "rtree_type_valid"
#define ASSERT_rooted_tree_type_NOT_VALID "rtree_type_not_valid"
#define ASSERT_RTREE_IS_TYPE "is_rtree_type"
#define ASSERT_RTREE_IS_NOT_TYPE "is_not_rtree_type"

namespace exe_tests {

err_type process_assert(
	const string& assert_what,
	map<string, ugraph>& ugraphvars,
	map<string, dgraph>& dgraphvars,
	map<string, ftree>& ftreevars,
	map<string, rtree>& rtreevars,
	map<string, string>& gtypes,
	ifstream& fin
)
{
	string g1, g2;
	uint32_t n;
	node u,v;

	if (assert_what == ASSERT_GRAPH_EQUAL_GRAPHS) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_GRAPH_EQUAL_GRAPHS, g1)
		assert_exists_variable(ASSERT_GRAPH_EQUAL_GRAPHS, g2)
		assert_equal_types(ASSERT_GRAPH_EQUAL_GRAPHS, g1, g2)
		if (not are_graphs_equal(g1, g2)) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_EQUAL_GRAPHS)
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are not equal." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of " << g2 << ":" << endl;
			output_graph(g2)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_EQUAL_GRAPHS) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g1)
		assert_exists_variable(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g2)
		assert_equal_types(ASSERT_GRAPH_NOT_EQUAL_GRAPHS, g1, g2)
		if (are_graphs_equal(g1, g2)) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_EQUAL_GRAPHS)
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are equal." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of " << g2 << ":" << endl;
			output_graph(g2)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NEIGHBOURS_ARE) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_GRAPH_NEIGHBOURS_ARE, g1)
		vector<node> node_list(n);
		for (node& neigh : node_list) { fin >> neigh; }
		auto neighs = mfunction(g1, get_neighbours(u));
		sort(neighs.begin(), neighs.end());
		sort(node_list.begin(), node_list.end());
		if (neighs != node_list) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NEIGHBOURS_ARE)
			cerr << "    The list of neighbours of node " << u << " of graph "
				 << g1 << " does not coincide with input." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of list:" << endl;
			cerr << "   ";
			for (node neigh : node_list) { cerr << " " << neigh; }
			cerr << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_NORMALISED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_NORMALISED, g1)
		if (not mfunction(g1, is_normalised())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_IS_NORMALISED)
			cerr << "    Graph '" << g1 << "' is not normalised." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_NORMALISED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_NORMALISED, g1)
		if (mfunction(g1, is_normalised())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_NORMALISED)
			cerr << "    Graph '" << g1 << "' is normalised." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_EXISTS_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_EXISTS_EDGE, g1)
		if (not mfunction(g1, has_edge(u, v))) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_EXISTS_EDGE)
			cerr << "    Graph '" << g1 << "' does not have edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_EXISTS_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_NOT_EXISTS_EDGE, g1)
		if (mfunction(g1, has_edge(u, v))) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_EXISTS_EDGE)
			cerr << "    Graph '" << g1 << "' has edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NUM_NODES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_NUM_NODES, g1)
		if (mfunction(g1, n_nodes()) != n) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NUM_NODES)
			cerr << "    Graph '" << g1 << "' does not have " << n << " nodes." << endl;
			cerr << "    Graph '" << g1 << "' has " << mfunction(g1, n_nodes()) << " nodes." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NUM_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_NUM_EDGES, g1)
		if (mfunction(g1, n_edges()) != n) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NUM_EDGES)
			cerr << "    Graph '" << g1 << "' does not have " << n << " edges." << endl;
			cerr << "    Graph '" << g1 << "' has " << mfunction(g1, n_edges()) << " edges." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_EDGES_ARE) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_EDGES_ARE, g1)
		vector<edge> edge_list(n);
		for (edge& e : edge_list) { fin >> e.first >> e.second; }
		sort(edge_list.begin(), edge_list.end());

		vector<edge> gv = ffunction(g1, enumerate_edges_brute_force);
		sort(gv.begin(), gv.end());
		if (edge_list != gv) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_EDGES_ARE)
			cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list." << endl;
			cerr << "    List (" << edge_list.size() << "):" << endl;
			for (auto e : edge_list) {
				cerr << "    " << edge_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					cerr << " <- not in the graph!";
				}
				cerr << endl;
			}
			cerr << "    Graph (" << gv.size() << "):" << endl;
			for (auto e : gv) {
				cerr << "    " << edge_out(e) << endl;
			}
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_ELEMENTS_Q_ARE) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_GRAPH_ELEMENTS_Q_ARE, g1)
		vector<edge_pair> edge_pair_list(n);
		for (edge_pair& e : edge_pair_list) {
			fin >> e.first.first >> e.first.second
				>> e.second.first >> e.second.second;
			assert(not share_vertices(e));
		}
		sort(edge_pair_list.begin(), edge_pair_list.end());

		vector<edge_pair> gv = ffunction(g1, enumerate_Q_brute_force);
		sort(gv.begin(), gv.end());
		if (edge_pair_list != gv) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_ELEMENTS_Q_ARE)
			cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list." << endl;
			cerr << "    List (" << edge_pair_list.size() << "):" << endl;
			for (auto e : edge_pair_list) {
				cerr << "    " << edge_pair_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					cerr << " <- not in the graph!";
				}
				cerr << endl;
			}
			cerr << "    Graph (" << gv.size() << "):" << endl;
			for (auto e : gv) {
				cerr << "    " << edge_pair_out(e) << endl;
			}
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_UNDIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_UNDIRECTED, g1)
		if (gtypes[g1] != UGRAPH) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_IS_UNDIRECTED)
			cerr << "    Graph '" << g1 << "' is not undirected." << endl;
			return err_type::test_exe_error;
		}
		if (not mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_IS_UNDIRECTED)
			cerr << "    Graph '" << g1 << "' does not return undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_UNDIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_UNDIRECTED, g1)
		if (gtypes[g1] == UGRAPH) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_UNDIRECTED)
			cerr << "    Graph '" << g1 << "' is undirected." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_UNDIRECTED)
			cerr << "    Graph '" << g1 << "' returns undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_IS_DIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_IS_DIRECTED, g1)
		if (gtypes[g1] != DGRAPH) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_IS_DIRECTED)
			cerr << "    Graph '" << g1 << "' is not directed." << endl;
			return err_type::test_exe_error;
		}
		if (not mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_IS_DIRECTED)
			cerr << "    Graph '" << g1 << "' does not return directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_NOT_DIRECTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_GRAPH_NOT_DIRECTED, g1)
		if (gtypes[g1] == DGRAPH) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_DIRECTED)
			cerr << "    Graph '" << g1 << "' is directed." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_NOT_DIRECTED)
			cerr << "    Graph '" << g1 << "' returns directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_DEGREE, g1)
		if (mfunction(g1, degree(u)) != v) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_DEGREE)
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have degree " << v << endl;
			cerr << "    The vertex has degree: " << mfunction(g1, degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}

	// DIRECTED GRAPHS
	else if (assert_what == ASSERT_DGRAPH_IN_NEIGHBOURS_ARE) {
		fin >> g1 >> u >> n;
		assert_exists_variable(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE, g1)
		assert_correct_graph_type(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE, graph_type(g1), directed_types)
		vector<node> node_list(n);
		for (node& neigh : node_list) { fin >> neigh; }
		auto neighs = mfunction_dir_graphs(g1, get_in_neighbours(u));
		sort(neighs.begin(), neighs.end());
		sort(node_list.begin(), node_list.end());
		if (neighs != node_list) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_DGRAPH_IN_NEIGHBOURS_ARE)
			cerr << "    The list of neighbours of node " << u << " of graph "
				 << g1 << " does not coincide with input." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of list:" << endl;
			cerr << "   ";
			for (node neigh : node_list) { cerr << " " << neigh; }
			cerr << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_GRAPH_FULL_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_GRAPH_FULL_DEGREE, g1)
		assert_correct_graph_type(ASSERT_GRAPH_FULL_DEGREE, graph_type(g1), directed_types)
		const uint32_t fdegree =
			mfunction_dir_graphs(g1, in_degree(u)) +
			mfunction_dir_graphs(g1, out_degree(u));
		if (fdegree != v) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_GRAPH_FULL_DEGREE)
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have full degree " << v << endl;
			cerr << "    The vertex has full degree: " << fdegree << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_DGRAPH_IN_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_DGRAPH_IN_DEGREE, g1)
		assert_correct_graph_type(ASSERT_DGRAPH_IN_DEGREE, graph_type(g1), directed_types)
		if (mfunction_dir_graphs(g1, in_degree(u)) != v) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_DGRAPH_IN_DEGREE)
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have in-degree " << v << endl;
			cerr << "    The vertex has in-degree: " << mfunction_dir_graphs(g1, in_degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_DGRAPH_OUT_DEGREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_DGRAPH_OUT_DEGREE, g1)
		assert_correct_graph_type(ASSERT_DGRAPH_OUT_DEGREE, graph_type(g1), directed_types)
		if (mfunction_dir_graphs(g1, degree(u)) != v) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_DGRAPH_OUT_DEGREE)
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have out-degree " << v << endl;
			cerr << "    The vertex has out-degree: " << mfunction_dir_graphs(g1, degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}

	// TREES
	else if (assert_what == ASSERT_TREE_IS_TREE) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_TREE, g1)
		assert_correct_graph_type(ASSERT_TREE_IS_TREE, graph_type(g1), tree_types)
		if (not mfunction_trees(g1, is_tree())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_IS_TREE)
			cerr << "    Tree '" << g1 << "' is 'not a tree'." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_NOT_TREE) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_NOT_TREE, g1)
		assert_correct_graph_type(ASSERT_TREE_IS_NOT_TREE, graph_type(g1), tree_types)
		if (mfunction_trees(g1, is_tree())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_IS_TREE)
			cerr << "    Tree '" << g1 << "' is 'a tree'." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_CAN_ADD_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_TREE_CAN_ADD_EDGE, g1)
		assert_correct_graph_type(ASSERT_TREE_CAN_ADD_EDGE, graph_type(g1), tree_types)
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (not can) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_CAN_ADD_EDGE)
			cerr << "    Cannot add edge with vertices " << u << ", " << v << " "
				 << "to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_CANT_ADD_EDGE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_TREE_CANT_ADD_EDGE, g1)
		assert_correct_graph_type(ASSERT_TREE_CANT_ADD_EDGE, graph_type(g1), tree_types)
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (can) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_CANT_ADD_EDGE)
			cerr << "    Edge with vertices " << u << ", " << v << " "
				 << "can be added to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_CAN_ADD_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_TREE_CAN_ADD_EDGES, g1)
		assert_correct_graph_type(ASSERT_TREE_CAN_ADD_EDGES, graph_type(g1), tree_types)

		vector<edge> edge_list(n);
		for (edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (not can) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_CAN_ADD_EDGES)
			cerr << "    Cannot add edges" << endl;
			for (auto e : edge_list) {
			cerr << "        " << e.first << ", " << e.second << endl;
			}
			cerr << "    to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_CANT_ADD_EDGES) {
		fin >> g1 >> n;
		assert_exists_variable(ASSERT_TREE_CANT_ADD_EDGES, g1)
		assert_correct_graph_type(ASSERT_TREE_CANT_ADD_EDGES, graph_type(g1), tree_types)

		vector<edge> edge_list(n);
		for (edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (can) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_CANT_ADD_EDGES)
			cerr << "    Cannot add edges" << endl;
			for (auto e : edge_list) {
			cerr << "        " << e.first << ", " << e.second << endl;
			}
			cerr << "    to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_ROOTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_ROOTED, g1)
		assert_correct_graph_type(ASSERT_TREE_IS_ROOTED, graph_type(g1), tree_types)
		if (not mfunction_trees(g1, is_rooted())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_IS_ROOTED)
			cerr << "    Graph '" << g1 << "' is not rooted." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_TREE_IS_NOT_ROOTED) {
		fin >> g1;
		assert_exists_variable(ASSERT_TREE_IS_NOT_ROOTED, g1)
		assert_correct_graph_type(ASSERT_TREE_IS_NOT_ROOTED, graph_type(g1), tree_types)
		if (mfunction_trees(g1, is_rooted())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_TREE_IS_NOT_ROOTED)
			cerr << "    Graph '" << g1 << "' is rooted." << endl;
			return err_type::test_exe_error;
		}
	}

	// ROOTED TREES
	else if (assert_what == ASSERT_RTREE_NO_NEED_RECALC_SUBSTREE_SIZE) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_NO_NEED_RECALC_SUBSTREE_SIZE, g1)
		assert_correct_graph_type(ASSERT_RTREE_NO_NEED_RECALC_SUBSTREE_SIZE, graph_type(g1), rooted_tree_types)
		if (mfunction_rtrees(g1, size_subtrees_valid())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_NO_NEED_RECALC_SUBSTREE_SIZE)
			cerr << "    Sizes of subtrees in tree '" << g1 << "' have to be recalculated." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_RTREE_NEED_RECALC_SUBSTREE_SIZE) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_NEED_RECALC_SUBSTREE_SIZE, g1)
		assert_correct_graph_type(ASSERT_RTREE_NEED_RECALC_SUBSTREE_SIZE, graph_type(g1), rooted_tree_types)
		if (not mfunction_rtrees(g1, size_subtrees_valid())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_NEED_RECALC_SUBSTREE_SIZE)
			cerr << "    Sizes of subtrees in tree '" << g1 << "' do not need to be recalculated." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_RTREE_HAS_ROOT) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_HAS_ROOT, g1)
		assert_correct_graph_type(ASSERT_RTREE_HAS_ROOT, graph_type(g1), rooted_tree_types)
		if (not mfunction_rtrees(g1, has_root())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_HAS_ROOT)
			cerr << "    Tree '" << g1 << "' does not have a root." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_RTREE_NOT_HAS_ROOT) {
		fin >> g1;
		assert_exists_variable(ASSERT_RTREE_NOT_HAS_ROOT, g1)
		assert_correct_graph_type(ASSERT_RTREE_NOT_HAS_ROOT, graph_type(g1), rooted_tree_types)
		if (mfunction_rtrees(g1, has_root())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_NOT_HAS_ROOT)
			cerr << "    Tree '" << g1 << "' has a root." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_RTREE_SIZE_SUBTREE) {
		fin >> g1 >> u >> v;
		assert_exists_variable(ASSERT_RTREE_SIZE_SUBTREE, g1)
		assert_correct_graph_type(ASSERT_RTREE_SIZE_SUBTREE, graph_type(g1), rooted_tree_types)
		assert_is_rtree(g1, ASSERT_RTREE_SIZE_SUBTREE)
		if (mfunction_rtrees(g1, size_subtrees_valid())) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_SIZE_SUBTREE)
			cerr << "    Values of subtree's sizes in tree '" << g1 << "' need recalculating." << endl;
			return err_type::test_exe_error;
		}

		n = mfunction_rtrees(g1, n_nodes_subtree(u));
		if (n != v) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_SIZE_SUBTREE)
			cerr << "    The size of subtree rooted at '" << u << "' is not '" << v << "." << endl;
			cerr << "    The size of the subtree is: " << n << endl;
			return err_type::test_exe_error;
		}
	}

	// ROOTED DIRECTED TREES
	else if (assert_what == ASSERT_rooted_tree_type_valid) {
		fin >> g1;
		assert_exists_variable(ASSERT_rooted_tree_type_valid, g1)
		assert_correct_graph_type(ASSERT_rooted_tree_type_valid, graph_type(g1), rooted_tree_types)
		if (not rtreevars[g1].rooted_tree_type_valid()) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_rooted_tree_type_valid)
			cerr << "    Tree '" << g1 << "' does not have a valid rooted_tree_type." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_rooted_tree_type_NOT_VALID) {
		fin >> g1;
		assert_exists_variable(ASSERT_rooted_tree_type_valid, g1)
		assert_correct_graph_type(ASSERT_rooted_tree_type_NOT_VALID, graph_type(g1), rooted_tree_types)
		if (rtreevars[g1].rooted_tree_type_valid()) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_rooted_tree_type_valid)
			cerr << "    Tree '" << g1 << "' has a valid rooted_tree_type." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == ASSERT_RTREE_IS_TYPE) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_RTREE_IS_TYPE, g1)
		assert_string_is_rooted_tree_type(ASSERT_RTREE_IS_TYPE, g2)
		assert_correct_graph_type(ASSERT_RTREE_IS_TYPE, graph_type(g1), rooted_tree_types)
		if (not rtreevars[g1].rooted_tree_type_valid()) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_IS_NOT_TYPE)
			cerr << "    Tree '" << g1 << "' does not have a valid rooted_tree_type." << endl;
			return err_type::test_exe_error;
		}
		auto type = rtreevars[g1].get_rooted_tree_type();
		if (g2 == "arborescence") {
			if (type != RTREE_arborescence) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is not an arborescence." << endl;
				cerr << "    Directed rooted tree type: " << rooted_tree_type_to_string(type) << endl;
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "anti_arborescence") {
			if (type != RTREE_anti_arborescence) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is not an anti-arborescence." << endl;
				cerr << "    Directed rooted tree type: " << rooted_tree_type_to_string(type) << endl;
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "none") {
			if (type != RTREE_none) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is not 'none'." << endl;
				cerr << "    Directed rooted tree type: " << rooted_tree_type_to_string(type) << endl;
				return err_type::test_exe_error;
			}
		}
	}
	else if (assert_what == ASSERT_RTREE_IS_NOT_TYPE) {
		fin >> g1 >> g2;
		assert_exists_variable(ASSERT_RTREE_IS_NOT_TYPE, g1)
		assert_string_is_rooted_tree_type(ASSERT_RTREE_IS_NOT_TYPE, g2)
		assert_correct_graph_type(ASSERT_RTREE_IS_NOT_TYPE, graph_type(g1), rooted_tree_types)
		if (not rtreevars[g1].rooted_tree_type_valid()) {
			cerr << ERROR << endl;
			message_in_func(ASSERT_RTREE_IS_NOT_TYPE)
			cerr << "    Tree '" << g1 << "' does not have a valid rooted_tree_type." << endl;
			return err_type::test_exe_error;
		}
		auto type = rtreevars[g1].get_rooted_tree_type();
		if (g2 == "arborescence") {
			if (type == RTREE_arborescence) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_NOT_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is an arborescence." << endl;
				cerr << "    Parameter value: " << g2 << endl;
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "anti_arborescence") {
			if (type == RTREE_anti_arborescence) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_NOT_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is an anti-arborescence." << endl;
				cerr << "    Parameter value: " << g2 << endl;
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "none") {
			if (type == RTREE_none) {
				cerr << ERROR << endl;
				message_in_func(ASSERT_RTREE_IS_NOT_TYPE)
				cerr << "    Directed rooted tree '" << g1 << "' is 'none'." << endl;
				cerr << "    Parameter value: " << g2 << endl;
				return err_type::test_exe_error;
			}
		}
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Invalid assertion '" << assert_what << "'." << endl;
		return err_type::test_format_error;
	}
	return err_type::no_error;
}

} // -- namespace exe_tests
