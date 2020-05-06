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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
#include "exe_tests.hpp"
#include "exe_construction.hpp"
#include "test_utils.hpp"

#define FUNC_GRAPH_CREATE "create_graph"
#define FUNC_GRAPH_READ "read_graph"
#define FUNC_GRAPH_OUTPUT "output_graph"
#define FUNC_GRAPH_INIT "init_graph"
#define FUNC_GRAPH_ASSIGN "assign"
#define FUNC_GRAPH_ADD_EDGE "add_edge"
#define FUNC_GRAPH_ADD_EDGES "add_edges"
#define FUNC_GRAPH_REMOVE_EDGE "remove_edge"
#define FUNC_GRAPH_REMOVE_EDGES "remove_edges"
#define FUNC_GRAPH_NORMALISE "normalise"
#define FUNC_GRAPH_DISJ_UNION "disjoint_union"
#define FUNC_GRAPH_CHECK_EDGE_IT "check_edge_iterator"
#define FUNC_GRAPH_CHECK_Q_IT "check_Q_iterator"
#define FUNC_DGRAPH_TO_UGRAPH "dgraph_to_ugraph"
#define FUNC_FTREE_TO_RTREE "ftree_to_rtree"
#define FUNC_RTREE_SET_ROOT "set_root"
#define FUNC_RTREE_CALC_SIZE_SUBTREE "calculate_size_subtrees"
#define FUNC_RTREE_RETRIEVE_SUBTREE "retrieve_subtree"
#define FUNC_RTREE_FIND_TYPE "find_rtree_type"

namespace exe_tests {

err_type exe_construction_test(ifstream& fin) {
	map<string, ugraph> ugraphvars;
	map<string, dgraph> dgraphvars;
	map<string, ftree> ftreevars;
	map<string, rtree> rtreevars;

	map<string, string> gtypes;

	string option, assert_what;
	string type, g1, g2, g3, file, file_type, Boolean;
	uint32_t n_nodes;
	uint32_t u, v;

	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
		}
		else if (option == "output") {
			cout << read_output_string(fin) << endl;
		}
		else if (option == FUNC_GRAPH_CREATE) {
			fin >> type >> g1 >> n_nodes;
			assert_correct_graph_type(FUNC_GRAPH_CREATE, type, all_types)
			gtypes[g1] = type;
			if (type == UGRAPH) {
				ugraphvars[g1] = ugraph(n_nodes);
			}
			else if (type == DGRAPH) {
				dgraphvars[g1] = dgraph(n_nodes);
			}
			else if (type == FTREE) {
				ftreevars[g1] = ftree(n_nodes);
			}
			else if (type == RTREE) {
				rtreevars[g1] = rtree(n_nodes);
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CREATE)
				cerr << "    Unhandled type '" << type << "'." << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == FUNC_GRAPH_READ) {
			fin >> type >> g1 >> file >> file_type >> Boolean;
			assert_correct_graph_type(FUNC_GRAPH_READ, type, all_types)
			assert_correct_file_type(FUNC_GRAPH_READ, file_type)
			assert_correct_boolean(FUNC_GRAPH_READ, Boolean)
			gtypes[g1] = type;
			bool io_res = false;
			if (type == DGRAPH) {
				dgraphvars[g1] = dgraph();
				io_res = io::read_edge_list(file, dgraphvars[g1], Boolean == "true");
			}
			else if (type == UGRAPH) {
				ugraphvars[g1] = ugraph();
				io_res = io::read_edge_list(file, ugraphvars[g1], Boolean == "true");
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_READ)
				cerr << "    I/O operation not implemented for " << gtypes[g1] << endl;
				return err_type::test_exe_error;
			}
			if (not io_res) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_READ)
				cerr << "    I/O operation failed when attempting to read file '" << file << "'." << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == FUNC_GRAPH_OUTPUT) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_OUTPUT, g1)
			cout << "-----------------------------" << endl;
			cout << "Contents of graph '" << g1 << "'" << endl;
			output_graph_stdout(g1)
			cout << "-----------------------------" << endl;
		}
		else if (option == FUNC_GRAPH_INIT) {
			fin >> g1 >> n_nodes;
			assert_exists_variable(FUNC_GRAPH_INIT, g1)
			if_mfunction(g1, init(n_nodes))
		}
		else if (option == FUNC_GRAPH_ASSIGN) {
			fin >> g1 >> g2;
			assert_exists_variable(FUNC_GRAPH_ASSIGN, g2)
			//assert_not_exists_variable(FUNC_GRAPH_ASSIGN, g1)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == DGRAPH) { dgraphvars[g1] = dgraphvars[g2]; }
			else { ugraphvars[g1] = ugraphvars[g2]; }
		}
		else if (option == FUNC_GRAPH_ADD_EDGE) {
			fin >> g1 >> u >> v >> Boolean;
			assert_exists_variable(FUNC_GRAPH_ADD_EDGE, g1)
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGE, Boolean)
			if_mfunction(g1, add_edge(u, v, Boolean == "true"))
		}
		else if (option == FUNC_GRAPH_ADD_EDGES) {
			fin >> g1 >> n_nodes;
			vector<edge> edge_list(n_nodes);
			for (edge& e : edge_list) { fin >> e.first >> e.second; }
			fin >> Boolean;
			assert_exists_variable(FUNC_GRAPH_ADD_EDGES, g1)
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGES, Boolean)
			if_mfunction(g1, add_edges(edge_list, Boolean == "true"))
		}
		else if (option == FUNC_GRAPH_REMOVE_EDGE) {
			fin >> g1 >> u >> v >> Boolean;
			assert_exists_variable(FUNC_GRAPH_REMOVE_EDGE, g1)
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGE, Boolean)
			if_mfunction(g1, remove_edge(u, v, Boolean == "true"))
		}
		else if (option == FUNC_GRAPH_REMOVE_EDGES) {
			fin >> g1 >> n_nodes;
			vector<edge> edge_list(n_nodes);
			for (edge& e : edge_list) { fin >> e.first >> e.second; }
			fin >> Boolean;
			assert_exists_variable(FUNC_GRAPH_REMOVE_EDGES, g1)
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES, Boolean)
			if_mfunction(g1, remove_edges(edge_list, Boolean == "true"))
		}
		else if (option == FUNC_GRAPH_NORMALISE) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_NORMALISE, g1)
			if_mfunction(g1, normalise())
		}
		else if (option == FUNC_GRAPH_DISJ_UNION) {
			fin >> g1 >> g2 >> g3;
			assert_exists_variable(FUNC_GRAPH_DISJ_UNION, g2)
			assert_exists_variable(FUNC_GRAPH_DISJ_UNION, g3)
			assert_equal_types(FUNC_GRAPH_DISJ_UNION, g2,g3)
			assert_not_exists_variable(FUNC_GRAPH_DISJ_UNION, g1)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == DGRAPH) {
				make_disjoint_union<dgraph>(g1, g2, g3, dgraphvars);
			}
			else if (graph_type(g2) == UGRAPH) {
				make_disjoint_union<ugraph>(g1, g2, g3, ugraphvars);
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_DISJ_UNION)
				cerr << "    Type of graphs '" << g1 << "' and '" << g2 << "' are not" << endl;
				cerr << "    " << UGRAPH << " or " << DGRAPH << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == FUNC_GRAPH_CHECK_EDGE_IT) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_CHECK_EDGE_IT, g1)
			vector<edge> iter_edges;
			if (graph_type(g1) == DGRAPH) {
				E_iterator it(dgraphvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			else {
				E_iterator it(ugraphvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			// check size
			if (iter_edges.size() != mfunction(g1, n_edges())) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_EDGE_IT)
				cerr << "    The amount of edges obtained differs from the amount of edges in the graph." << endl;
				cerr << "    Number of edges obtained: " << iter_edges.size() << endl;
				cerr << "    Number of edges in the graph: " << mfunction(g1, n_edges()) << endl;
				return err_type::test_exe_error;
			}
			sort(iter_edges.begin(), iter_edges.end());
			vector<edge> graph_edges = ffunction(g1, enumerate_edges_brute_force);
			sort(graph_edges.begin(), graph_edges.end());
			if (iter_edges != graph_edges) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_EDGE_IT)
				cerr << "    The edges in graph '" << g1
					 << "' do not coincide with those in the list." << endl;
				cerr << "    List (" << iter_edges.size() << "):" << endl;
				for (auto e : iter_edges) {
					cerr << "    " << edge_out(e);
					auto it = find(graph_edges.begin(), graph_edges.end(), e);
					if (it == graph_edges.end()) {
						cerr << " <- not in the graph!";
					}
					cerr << endl;
				}
				cerr << "    Graph (" << graph_edges.size() << "):" << endl;
				for (auto e : graph_edges) {
					cerr << "    " << edge_out(e) << endl;
				}
				cerr << "    Contents:" << endl;
				output_graph(g1)
				return err_type::test_exe_error;
			}
		}
		else if (option == FUNC_GRAPH_CHECK_Q_IT) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_CHECK_Q_IT, g1)
			vector<edge_pair> iter_pair_edges;
			if (graph_type(g1) == DGRAPH) {
				const dgraph& dg1 = dgraphvars[g1];
				Q_iterator it(dg1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			else {
				const ugraph& ug1 = ugraphvars[g1];
				Q_iterator it(ug1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			// check size
			if (iter_pair_edges.size() != ffunction(g1, properties::size_Q)) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_Q_IT)
				cerr << "    The amount of pairs obtained differs from the size of the set Q." << endl;
				cerr << "    Number of pairs obtained: " << iter_pair_edges.size() << endl;
				cerr << "    Size of the set Q: " << ffunction(g1, properties::size_Q) << endl;
				return err_type::test_exe_error;
			}
			sort(iter_pair_edges.begin(), iter_pair_edges.end());
			vector<edge_pair> gpe = ffunction(g1, enumerate_Q_brute_force);
			sort(gpe.begin(), gpe.end());
			if (iter_pair_edges != gpe) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_Q_IT)
				cerr << "    The pairs in graph '" << g1
					 << "' do not coincide with those in the list." << endl;
				cerr << "    List (" << iter_pair_edges.size() << "):" << endl;
				for (auto e : iter_pair_edges) {
					cerr << "    " << edge_pair_out(e);
					auto it = find(gpe.begin(), gpe.end(), e);
					if (it == gpe.end()) {
						cerr << " <- not in the graph!";
					}
					cerr << endl;
				}
				cerr << "    Graph (" << gpe.size() << "):" << endl;
				for (auto e : gpe) {
					cerr << "    " << edge_pair_out(e) << endl;
				}
				cerr << "    Contents:" << endl;
				output_graph(g1)
				return err_type::test_exe_error;
			}
		}

		// DGRAPH
		else if (option == FUNC_DGRAPH_TO_UGRAPH) {
			fin >> g1 >> g2;
			assert_exists_variable(FUNC_DGRAPH_TO_UGRAPH, g2)
			assert_not_exists_variable(FUNC_DGRAPH_TO_UGRAPH, g1)
			if (graph_type(g2) != DGRAPH) {
				WRONG_TYPE(FUNC_DGRAPH_TO_UGRAPH, g2, DGRAPH)
				return err_type::test_exe_error;
			}
			ugraphvars[g1] = dgraphvars[g2].to_undirected();
			gtypes[g1] = UGRAPH;
		}

		// UTREE
		else if (option == FUNC_FTREE_TO_RTREE) {
			fin >> g1 >> g2 >> u;
			assert_exists_variable(FUNC_FTREE_TO_RTREE, g2)
			assert_not_exists_variable(FUNC_FTREE_TO_RTREE, g1)
			assert_correct_graph_type(FUNC_FTREE_TO_RTREE, graph_type(g2), free_tree_types)
			rtreevars[g1] = rtree(ftreevars[g2], u);
			gtypes[g1] = RTREE;
		}

		// RTREE
		else if (option == FUNC_RTREE_SET_ROOT) {
			fin >> g1 >> u;
			assert_exists_variable(FUNC_RTREE_SET_ROOT, g1)
			assert_correct_graph_type(FUNC_RTREE_SET_ROOT, graph_type(g1), rooted_tree_types)

			if_mfunction_rtrees(g1, set_root(u))
		}
		else if (option == FUNC_RTREE_CALC_SIZE_SUBTREE) {
			fin >> g1;
			assert_exists_variable(FUNC_RTREE_CALC_SIZE_SUBTREE, g1)
			assert_correct_graph_type(FUNC_RTREE_CALC_SIZE_SUBTREE, graph_type(g1), rooted_tree_types)
			assert_is_rtree(g1, FUNC_RTREE_SET_ROOT)

			if (graph_type(g1) == RTREE) {
				fin >> Boolean;
				assert_correct_boolean(FUNC_RTREE_CALC_SIZE_SUBTREE, Boolean)
				rtreevars[g1].recalc_size_subtrees(Boolean == "true");
			}
			else {
				rtreevars[g1].recalc_size_subtrees();
			}
		}
		else if (option == FUNC_RTREE_RETRIEVE_SUBTREE) {
			fin >> g2 >> g1 >> u;
			assert_not_exists_variable(FUNC_RTREE_RETRIEVE_SUBTREE, g2)
			assert_exists_variable(FUNC_RTREE_RETRIEVE_SUBTREE, g1)
			assert_correct_graph_type(FUNC_RTREE_RETRIEVE_SUBTREE, graph_type(g1), rooted_tree_types)
			assert_is_rtree(g1, FUNC_RTREE_RETRIEVE_SUBTREE)

			if (mfunction_rtrees(g1, is_directed())) {
				rtreevars[g2] = rtreevars[g1].get_subtree(u);
				gtypes[g2] = RTREE;
			}
			else {
				rtreevars[g2] = rtreevars[g1].get_subtree(u);
				gtypes[g2] = RTREE;
			}
		}

		// DRTREE
		else if (option == FUNC_RTREE_FIND_TYPE) {
			fin >> g1;
			assert_exists_variable(FUNC_RTREE_FIND_TYPE, g1)
			assert_correct_graph_type(FUNC_RTREE_FIND_TYPE, graph_type(g1), rooted_tree_types)
			assert_is_rtree(g1, FUNC_RTREE_FIND_TYPE)

			rtreevars[g1].find_rtree_type();
		}

		// ASSERT
		else if (option == "assert") {
			fin >> option;
			err_type e = process_assert(
				option,
				ugraphvars, dgraphvars,
				ftreevars, rtreevars,
				gtypes, fin
			);
			if (e != err_type::no_error) { return e; }
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid option '" << option << "'." << endl;
			return err_type::test_format_error;
		}
	}

	return err_type::no_error;
}

err_type exe_construction(ifstream& fin) {
	string field;
	size_t n;

	fin >> field;
	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}
	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	err_type e = exe_construction_test(fin);
	if (e == err_type::no_error) {
		TEST_GOODBYE
	}
	return e;
}

} // -- namespace exe_tests
