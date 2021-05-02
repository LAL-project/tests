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

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
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
#include <lal/properties/Q.hpp>
using namespace lal;
using namespace graphs;
using namespace iterators;

// common includes
#include "exe_construction.hpp"
#include "common/test_utils.hpp"
#include "common/io_wrapper.hpp"

#define FUNC_GRAPH_CREATE "create_graph"
#define FUNC_GRAPH_READ "read_graph"
#define FUNC_GRAPH_OUTPUT "output_graph"
#define FUNC_GRAPH_INIT "init_graph"
#define FUNC_GRAPH_ASSIGN "assign"
#define FUNC_GRAPH_ADD_EDGE "add_edge"
#define FUNC_GRAPH_ADD_EDGE_BULK "add_edge_bulk"
#define FUNC_GRAPH_FINISH_BULK_ADD "finish_bulk_add"
#define FUNC_GRAPH_ADD_EDGES "add_edges"
#define FUNC_GRAPH_SET_EDGES "set_edges"
#define FUNC_GRAPH_REMOVE_EDGE "remove_edge"
#define FUNC_GRAPH_REMOVE_EDGES "remove_edges"
#define FUNC_GRAPH_REMOVE_EDGES_INCIDENT_TO "remove_edges_incident_to"
#define FUNC_GRAPH_NORMALISE "normalise"
#define FUNC_GRAPH_DISJ_UNION "disjoint_union"
#define FUNC_GRAPH_CHECK_EDGE_IT "check_edge_iterator"
#define FUNC_OUTPUT_EDGES "output_E"
#define FUNC_GRAPH_CHECK_Q_IT "check_Q_iterator"
#define FUNC_OUTPUT_Q "output_Q"
#define FUNC_Q_SIZE "size_Q"
#define FUNC_DGRAPH_TO_UGRAPH "dgraph_to_ugraph"
#define FUNC_FTREE_TO_RTREE "ftree_to_rtree"
#define FUNC_RTREE_SET_ROOT "set_root"
#define FUNC_RTREE_CALC_SIZE_SUBTREE "calculate_size_subtrees"
#define FUNC_RTREE_RETRIEVE_SUBTREE "retrieve_subtree"
#define FUNC_RTREE_FIND_EDGE_ORIENTATION "find_edge_orientation"
#define FUNC_RTREE_DIR_TO_UNDIR "rtree_to_ftree"
#define FUNC_RTREE_OUTPUT_SIZE_SUBTREES "output_size_subtrees"

namespace tests {
namespace graphs {

err_type exe_construction_test(ifstream& fin) {
	map<string, undirected_graph> ugraphvars;
	map<string, directed_graph> dgraphvars;
	map<string, free_tree> ftreevars;
	map<string, rooted_tree> rtreevars;

	map<string, string> gtypes;

	string option, assert_what;
	string type, g1, g2, g3, file, file_type, Boolean1, Boolean2;
	uint32_t n_nodes, n_edges;
	node u, v;

	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
		}
		else if (option == "output") {
			cout << read_output_string(fin) << endl;
		}
		else if (option == FUNC_GRAPH_CREATE) {
			fin >> type >> g1 >> n_nodes;
			assert_correct_graph_type(FUNC_GRAPH_CREATE, type, all_types);
			gtypes[g1] = type;
			if (type == UGRAPH) {
				ugraphvars[g1] = undirected_graph(n_nodes);
			}
			else if (type == DGRAPH) {
				dgraphvars[g1] = directed_graph(n_nodes);
			}
			else if (type == FTREE) {
				ftreevars[g1] = free_tree(n_nodes);
			}
			else if (type == RTREE) {
				rtreevars[g1] = rooted_tree(n_nodes);
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CREATE)
				cerr << "    Unhandled type '" << type << "'." << endl;
				return err_type::test_execution;
			}
		}
		else if (option == FUNC_GRAPH_READ) {
			fin >> type >> g1 >> file >> file_type >> Boolean1;
			assert_correct_graph_type(FUNC_GRAPH_READ, type, all_types);
			assert_correct_boolean(FUNC_GRAPH_READ, Boolean1);
			gtypes[g1] = type;

			err_type err = err_type::no_error;
			if (type == DGRAPH) {
				dgraphvars[g1] = directed_graph();
				err = io_wrapper::read_graph(
					file, file_type, dgraphvars[g1], Boolean1 == "true"
				);
			}
			else if (type == UGRAPH) {
				ugraphvars[g1] = undirected_graph();
				err = io_wrapper::read_graph(
					file, file_type, ugraphvars[g1], Boolean1 == "true"
				);
			}
			else if (type == FTREE) {
				ftreevars[g1] = free_tree();
				err = io_wrapper::read_graph(
					file, file_type, ftreevars[g1], Boolean1 == "true"
				);
			}
			else if (type == RTREE) {
				rtreevars[g1] = rooted_tree();
				err = io_wrapper::read_graph(
					file, file_type, rtreevars[g1], Boolean1 == "true"
				);
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_READ);
				cerr << "    I/O operation not implemented for " << gtypes[g1] << endl;
				return err_type::test_format;
			}

			if (err != err_type::no_error) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_READ);
				cerr << "    I/O operation failed when attempting to read file '" << file << "'." << endl;
				return err_type::test_execution;
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
			assert_exists_variable(FUNC_GRAPH_INIT, g1);
			if_mfunction(g1, init(n_nodes))
		}
		else if (option == FUNC_GRAPH_ASSIGN) {
			fin >> g1 >> g2;
			assert_exists_variable(FUNC_GRAPH_ASSIGN, g2);
			//assert_not_exists_variable(FUNC_GRAPH_ASSIGN, g1)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == DGRAPH) { dgraphvars[g1] = dgraphvars[g2]; }
			else { ugraphvars[g1] = ugraphvars[g2]; }
		}
		else if (option == FUNC_GRAPH_ADD_EDGE) {
			fin >> g1 >> u >> v >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_ADD_EDGE, g1);
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGE, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGE, Boolean2);
			if_mfunction(g1, add_edge(u, v, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_ADD_EDGE_BULK) {
			fin >> g1 >> u >> v;
			assert_exists_variable(FUNC_GRAPH_ADD_EDGE_BULK, g1);
			if_mfunction(g1, add_edge_bulk(u, v))
		}
		else if (option == FUNC_GRAPH_FINISH_BULK_ADD) {
			fin >> g1 >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_FINISH_BULK_ADD, g1);
			assert_correct_boolean(FUNC_GRAPH_FINISH_BULK_ADD, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_FINISH_BULK_ADD, Boolean2);
			if_mfunction(g1, finish_bulk_add(Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_ADD_EDGES) {
			fin >> g1 >> n_edges;
			vector<edge> edge_list(n_edges);
			for (edge& e : edge_list) { fin >> e.first >> e.second; }
			fin >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_ADD_EDGES, g1);
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGES, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_ADD_EDGES, Boolean2);
			if_mfunction(g1, add_edges(edge_list, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_SET_EDGES) {
			fin >> g1 >> n_edges;
			vector<edge> edge_list(n_edges);
			for (edge& e : edge_list) { fin >> e.first >> e.second; }
			fin >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_SET_EDGES, g1);
			assert_correct_boolean(FUNC_GRAPH_SET_EDGES, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_SET_EDGES, Boolean2);
			if_mfunction(g1, set_edges(edge_list, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_REMOVE_EDGE) {
			fin >> g1 >> u >> v >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_REMOVE_EDGE, g1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGE, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGE, Boolean2);
			if_mfunction(g1, remove_edge(u, v, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_REMOVE_EDGES) {
			fin >> g1 >> n_edges;
			vector<edge> edge_list(n_edges);
			for (edge& e : edge_list) { fin >> e.first >> e.second; }
			fin >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_REMOVE_EDGES, g1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES, Boolean2);
			if_mfunction(g1, remove_edges(edge_list, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_REMOVE_EDGES_INCIDENT_TO) {
			fin >> g1 >> u;
			fin >> Boolean1 >> Boolean2;
			assert_exists_variable(FUNC_GRAPH_REMOVE_EDGES_INCIDENT_TO, g1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES_INCIDENT_TO, Boolean1);
			assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES_INCIDENT_TO, Boolean2);
			if_mfunction(g1, remove_edges_incident_to(u, Boolean1 == "true", Boolean2 == "true"))
		}
		else if (option == FUNC_GRAPH_NORMALISE) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_NORMALISE, g1);
			if_mfunction(g1, normalise())
		}
		else if (option == FUNC_GRAPH_DISJ_UNION) {
			fin >> g1 >> g2 >> g3;
			assert_exists_variable(FUNC_GRAPH_DISJ_UNION, g2);
			assert_exists_variable(FUNC_GRAPH_DISJ_UNION, g3);
			assert_equal_types(FUNC_GRAPH_DISJ_UNION, g2,g3);
			//assert_not_exists_variable(FUNC_GRAPH_DISJ_UNION, g1)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == UGRAPH) {
				undirected_graph uG = ugraphvars[g2];
				uG.disjoint_union(ugraphvars[g3]);
				ugraphvars[g1] = uG;
			}
			else if (graph_type(g2) == DGRAPH) {
				directed_graph dG = dgraphvars[g2];
				dG.disjoint_union(dgraphvars[g3]);
				dgraphvars[g1] = dG;
			}
			else if (graph_type(g2) == FTREE) {
				free_tree fT = ftreevars[g2];
				fT.disjoint_union(ftreevars[g3]);
				ftreevars[g1] = fT;
			}
			else if (graph_type(g2) == RTREE) {
				fin >> Boolean1;
				assert_correct_boolean(FUNC_GRAPH_REMOVE_EDGES, Boolean1);

				rooted_tree rT = rtreevars[g2];
				rT.disjoint_union(rtreevars[g3], Boolean1 == "true");
				rtreevars[g1] = rT;
			}
			else {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_DISJ_UNION)
				cerr << "    Type of graph '" << g1 << "' and '" << g2 << "' is not" << endl;
				cerr << "    " << UGRAPH << endl;
				cerr << "    " << DGRAPH << endl;
				cerr << "    " << FTREE << endl;
				cerr << "    " << RTREE << endl;
				return err_type::test_execution;
			}
		}
		else if (option == FUNC_GRAPH_CHECK_EDGE_IT) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_CHECK_EDGE_IT, g1);
			vector<edge> iter_edges;
			if (graph_type(g1) == DGRAPH) {
				for (E_iterator e_it(dgraphvars[g1]); not e_it.end(); e_it.next()) {
					iter_edges.push_back(e_it.get_edge());
				}
			}
			else if (graph_type(g1) == UGRAPH) {
				for (E_iterator e_it(ugraphvars[g1]); not e_it.end(); e_it.next()) {
					iter_edges.push_back(e_it.get_edge());
				}
			}
			else if (graph_type(g1) == FTREE) {
				for (E_iterator e_it(ftreevars[g1]); not e_it.end(); e_it.next()) {
					iter_edges.push_back(e_it.get_edge());
				}
			}
			else if (graph_type(g1) == RTREE) {
				for (E_iterator e_it(rtreevars[g1]); not e_it.end(); e_it.next()) {
					iter_edges.push_back(e_it.get_edge());
				}
			}

			// check size
			if (iter_edges.size() != mfunction(g1, get_num_edges())) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_EDGE_IT);
				cerr << "    The amount of edges obtained differs from the amount of edges in the graph." << endl;
				cerr << "    Number of edges obtained: " << iter_edges.size() << endl;
				cerr << "    Number of edges in the graph: " << mfunction(g1, get_num_edges()) << endl;
				return err_type::test_execution;
			}
			sort(iter_edges.begin(), iter_edges.end());
			vector<edge> graph_edges = ffunction(g1, enumerate_edges_brute_force);
			sort(graph_edges.begin(), graph_edges.end());
			if (iter_edges != graph_edges) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_EDGE_IT);
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
				return err_type::test_execution;
			}
		}
		else if (option == FUNC_OUTPUT_EDGES) {
			fin >> g1;
			assert_exists_variable(FUNC_OUTPUT_EDGES, g1);
			if (graph_type(g1) == DGRAPH) {
				for (E_iterator e_it(dgraphvars[g1]); not e_it.end(); e_it.next()) {
					const edge e = e_it.get_edge();
					cout << e.first << " " << e.second << endl;
				}
			}
			if (graph_type(g1) == UGRAPH) {
				for (E_iterator e_it(ugraphvars[g1]); not e_it.end(); e_it.next()) {
					const edge e = e_it.get_edge();
					cout << e.first << " " << e.second << endl;
				}
			}
		}
		else if (option == FUNC_GRAPH_CHECK_Q_IT) {
			fin >> g1;
			assert_exists_variable(FUNC_GRAPH_CHECK_Q_IT, g1);
			vector<edge_pair> iter_pair_edges;
			if (graph_type(g1) == DGRAPH) {
				for (Q_iterator q_it(dgraphvars[g1]); not q_it.end(); q_it.next()) {
					iter_pair_edges.push_back(q_it.get_edge_pair());
					assert(not share_vertices(it.get_edge_pair()));
				}
			}
			else if (graph_type(g1) == UGRAPH) {
				for (Q_iterator q_it(ugraphvars[g1]); not q_it.end(); q_it.next()) {
					iter_pair_edges.push_back(q_it.get_edge_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			else if (graph_type(g1) == FTREE) {
				for (Q_iterator q_it(ftreevars[g1]); not q_it.end(); q_it.next()) {
					iter_pair_edges.push_back(q_it.get_edge_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			else if (graph_type(g1) == RTREE) {
				for (Q_iterator q_it(rtreevars[g1]); not q_it.end(); q_it.next()) {
					iter_pair_edges.push_back(q_it.get_edge_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}

			// check size
			if (iter_pair_edges.size() != ffunction(g1, properties::num_pairs_independent_edges)) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_Q_IT);
				cerr << "    The amount of pairs obtained differs from the size of the set Q." << endl;
				cerr << "    Number of pairs obtained: " << iter_pair_edges.size() << endl;
				cerr << "    Size of the set Q: " << ffunction(g1, properties::num_pairs_independent_edges) << endl;
				return err_type::test_execution;
			}
			sort(iter_pair_edges.begin(), iter_pair_edges.end());
			vector<edge_pair> gpe = ffunction(g1, enumerate_Q_brute_force);
			sort(gpe.begin(), gpe.end());
			if (iter_pair_edges != gpe) {
				cerr << ERROR << endl;
				message_in_func(FUNC_GRAPH_CHECK_Q_IT);
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
				return err_type::test_execution;
			}
		}
		else if (option == FUNC_OUTPUT_Q) {
			fin >> g1;
			assert_exists_variable(FUNC_OUTPUT_Q, g1);

			if (graph_type(g1) == DGRAPH) {
				for (Q_iterator q_it(dgraphvars[g1]); not q_it.end(); q_it.next()) {
					const edge_pair e = q_it.get_edge_pair();
					cout
						<< "(" << e.first.first << " " << e.first.second << "), ("
						<< e.second.first << " " << e.second.second << ")"
						<< endl;
				}
			}
			if (graph_type(g1) == UGRAPH) {
				for (Q_iterator q_it(ugraphvars[g1]); not q_it.end(); q_it.next()) {
					const edge_pair e = q_it.get_edge_pair();
					cout
						<< "(" << e.first.first << " " << e.first.second << "), ("
						<< e.second.first << " " << e.second.second << ")"
						<< endl;
				}
			}
		}
		else if (option == FUNC_Q_SIZE) {
			fin >> g1;
			assert_exists_variable(FUNC_Q_SIZE, g1)

			cout <<
				(graph_type(g1) == DGRAPH ?
					properties::num_pairs_independent_edges_integer(dgraphvars[g1]) :
					properties::num_pairs_independent_edges_integer(ugraphvars[g1]))
			<< endl;

		}

		// DGRAPH
		else if (option == FUNC_DGRAPH_TO_UGRAPH) {
			fin >> g1 >> g2;
			assert_exists_variable(FUNC_DGRAPH_TO_UGRAPH, g2);
			assert_not_exists_variable(FUNC_DGRAPH_TO_UGRAPH, g1);
			if (graph_type(g2) != DGRAPH) {
				WRONG_TYPE(FUNC_DGRAPH_TO_UGRAPH, g2, DGRAPH);
				return err_type::test_execution;
			}
			ugraphvars[g1] = dgraphvars[g2].to_undirected();
			gtypes[g1] = UGRAPH;
		}

		// UTREE
		else if (option == FUNC_FTREE_TO_RTREE) {
			fin >> g1 >> g2 >> u;
			assert_exists_variable(FUNC_FTREE_TO_RTREE, g2);
			assert_not_exists_variable(FUNC_FTREE_TO_RTREE, g1);
			assert_correct_graph_type(FUNC_FTREE_TO_RTREE, graph_type(g2), free_tree_types);
			rtreevars[g1] = rooted_tree(ftreevars[g2], u);
			gtypes[g1] = RTREE;
		}

		// RTREE
		else if (option == FUNC_RTREE_SET_ROOT) {
			fin >> g1 >> u;
			assert_exists_variable(FUNC_RTREE_SET_ROOT, g1);
			assert_correct_graph_type(FUNC_RTREE_SET_ROOT, graph_type(g1), rooted_tree_types);

			if_mfunction_rtrees(g1, set_root(u))
		}
		else if (option == FUNC_RTREE_CALC_SIZE_SUBTREE) {
			fin >> g1;
			assert_exists_variable(FUNC_RTREE_CALC_SIZE_SUBTREE, g1);
			assert_correct_graph_type(FUNC_RTREE_CALC_SIZE_SUBTREE, graph_type(g1), rooted_tree_types);
			assert_is_rtree(g1, FUNC_RTREE_CALC_SIZE_SUBTREE);

			rtreevars[g1].calculate_size_subtrees();
		}
		else if (option == FUNC_RTREE_RETRIEVE_SUBTREE) {
			fin >> g2 >> g1 >> u;
			assert_not_exists_variable(FUNC_RTREE_RETRIEVE_SUBTREE, g2);
			assert_exists_variable(FUNC_RTREE_RETRIEVE_SUBTREE, g1);
			assert_correct_graph_type(FUNC_RTREE_RETRIEVE_SUBTREE, graph_type(g1), rooted_tree_types);
			assert_is_rtree(g1, FUNC_RTREE_RETRIEVE_SUBTREE);

			if (mfunction_rtrees(g1, is_directed())) {
				rtreevars[g2] = rtreevars[g1].get_subtree(u);
				gtypes[g2] = RTREE;
			}
			else {
				rtreevars[g2] = rtreevars[g1].get_subtree(u);
				gtypes[g2] = RTREE;
			}
		}
		else if (option == FUNC_RTREE_FIND_EDGE_ORIENTATION) {
			fin >> g1;
			assert_exists_variable(FUNC_RTREE_FIND_EDGE_ORIENTATION, g1);
			assert_correct_graph_type(FUNC_RTREE_FIND_EDGE_ORIENTATION, graph_type(g1), rooted_tree_types);
			assert_is_rtree(g1, FUNC_RTREE_FIND_EDGE_ORIENTATION);

			rtreevars[g1].find_edge_orientation();
		}
		else if (option == FUNC_RTREE_DIR_TO_UNDIR) {
			fin >> g1 >> g2;
			assert_exists_variable(FUNC_RTREE_DIR_TO_UNDIR, g1);
			assert_not_exists_variable(FUNC_RTREE_DIR_TO_UNDIR, g2);
			assert_correct_graph_type(FUNC_RTREE_DIR_TO_UNDIR, graph_type(g1), rooted_tree_types);
			assert_is_rtree(g1, FUNC_RTREE_DIR_TO_UNDIR);

			ftreevars[g2] = rtreevars[g1].to_undirected();
			gtypes[g2] = FTREE;
		}
		else if (option == FUNC_RTREE_OUTPUT_SIZE_SUBTREES) {
			fin >> g1;
			assert_exists_variable(FUNC_RTREE_DIR_TO_UNDIR, g1);
			assert_correct_graph_type(FUNC_RTREE_DIR_TO_UNDIR, graph_type(g1), rooted_tree_types);
			assert_is_rtree(g1, FUNC_RTREE_DIR_TO_UNDIR);
			const auto& T = rtreevars[g1];
			if (not T.are_size_subtrees_valid()) {
				cerr << ERROR << endl;
				cerr << "    Tree '" << g1 << "' does not have valid subtree sizes." << endl;
				return err_type::test_execution;
			}
			cout << "Subtree sizes:" << endl;
			for (node ww = 0; ww < T.get_num_nodes(); ++ww) {
				cout << "    " << ww << ": " << T.get_num_nodes_subtree(ww) << endl;
			}
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
			return err_type::test_format;
		}
	}

	return err_type::no_error;
}

err_type exe_graphs_construction(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	err_type e = exe_construction_test(fin);
	if (e == err_type::no_error) {
		TEST_GOODBYE
	}
	return e;
}

} // -- namespace graphs
} // -- namespace tests
