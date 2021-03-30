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
#include <iostream>
#include <fstream>
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/internal/properties/tree_centre.hpp>
#include <lal/internal/graphs/traversal.hpp>
using namespace lal;
using namespace internal;
using namespace graphs;
using namespace generate;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"

namespace exe_tests {

// this is a very simple algorithm to calculate the centre
template<class T>
pair<node, node> straightforward_centre(const T& tree, node u) {
	const auto n = tree.get_num_nodes();

	// First simple case:
	// in case the component of x has only one node (node x)...
	if (tree.get_degree(u) == 0) {
		return make_pair(u, n);
	}

	BFS<T> bfs(tree);
	bfs.set_use_rev_edges(tree.is_directed());

	// ------------------------------------
	// 1. find vertex 'v' farthest from 'u'

	vector<uint32_t> dists(tree.get_num_nodes());
	dists[u] = 0;

	// farthest vertex from 'u' and distance
	uint32_t max_dist = 0;
	node v;

	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) {
		dists[t] = dists[s] + 1;
		if (max_dist < dists[t]) {
			max_dist = dists[t];
			v = t;
		}
	}
	);
	bfs.start_at(u);

	// early termination
	if (v == u) {
		return make_pair(u, n);
	}

	// ------------------------------------
	// 2. find vertex 'w' farthest from 'v' and keep
	// track of the vertices from 'v' to 'w'

	fill(dists.begin(), dists.end(), 0);
	vector<vector<node>> paths(tree.get_num_nodes());
	paths[v] = vector<node>(1, v);

	max_dist = 0;
	vector<node> longest_path;
	node w;

	bfs.reset_visited();
	bfs.set_process_neighbour(
	[&](const auto&, node s, node t, bool) {
		dists[t] = dists[s] + 1;

		paths[t] = paths[s];
		paths[t].push_back(t);

		if (max_dist < dists[t]) {
			max_dist = dists[t];
			w = t;
			longest_path = paths[t];
		}
	}
	);
	bfs.start_at(v);

	// return centre

	if (longest_path.size()%2 == 0) {
		const size_t hm1 = longest_path.size()/2 - 1;
		const size_t h = longest_path.size()/2;

		const node c1 = longest_path[hm1];
		const node c2 = longest_path[h];
		return (c1 < c2 ? make_pair(c1,c2) : make_pair(c2,c1));
	}
	const size_t h = longest_path.size()/2;
	return make_pair(longest_path[h], n);
}

template<class G>
err_type exe_commands_utils_centre(ifstream& fin) {
	G t;
	uint32_t n;

	string option;
	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
		}
		else if (option == "output") {
			cout << read_output_string(fin) << endl;
		}
		else if (option == "init") {
			fin >> n;
			t.init(n);
		}
		else if (option == "add_edges") {
			fin >> n;
			vector<edge> es(n);
			for (uint32_t i = 0; i < n; ++i) {
				fin >> es[i].first >> es[i].second;
			}
			t.add_edges(es);
		}
		else if (option == "find_centre") {
			node s;
			fin >> s;
			const auto centre = internal::retrieve_centre(t, s);
			cout << "centre: " << centre.first;
			if (centre.second < t.get_num_nodes()) { cout << " " << centre.second; }
			cout << endl;
		}
		else if (option == "centre_is") {
			node s;
			fin >> s;
			const auto centre = internal::retrieve_centre(t, s);

			uint32_t centre_size;
			fin >> centre_size;

			if (centre_size == 1) {
				node u;
				fin >> u;

				if (centre.first != u) {
					cerr << ERROR << endl;
					cerr << "    Centre does not coincide." << endl;
					return err_type::test_execution;
				}
			}
			else if (centre_size == 2) {
				node u, v;
				fin >> u >> v;
				if (u > v) {
					cerr << ERROR << endl;
					cerr << "    The first vertex must be smaller than the second." << endl;
					return err_type::test_format;
				}
				if (u == v) {
					cerr << ERROR << endl;
					cerr << "    The two vertices are the same: " << u << " <-> " << v << "." << endl;
					return err_type::test_format;
				}

				if (centre.first != u and centre.second != v) {
					cerr << ERROR << endl;
					cerr << "    Centres do not coincide." << endl;
					return err_type::test_execution;
				}
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Centre size has to be either 1 or 2. Instead, received '" << centre_size << "'." << endl;
				return err_type::test_format;
			}
		}
		else if (option == "output_graph") {
			cout << t << endl;
		}
		else if (option == "remove_edge") {
			node u,v;
			fin >> u >> v;
			t.remove_edge(u,v);
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid command '" << option << "'." << endl;
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

err_type exe_full_utils_centre(const string& graph_type, ifstream& fin) {
	string how;
	fin >> how;
	if (how != "exhaustive" and how != "random") {
		cerr << ERROR << endl;
		cerr << "    Method of enumeration '" << how << "' is invalid." << endl;
		cerr << "    It must be either 'exhaustive' or 'random'." << endl;
		return err_type::test_format;
	}

#define test_correctness(T)										\
{																\
	const auto lib_centre = internal::retrieve_centre(T, 0);		\
	const auto easy_centre = straightforward_centre(T, 0);		\
	if (lib_centre != easy_centre) {							\
		cerr << ERROR << endl;									\
		cerr << "    Centres differ." << endl;					\
		cerr << "    Library: " << lib_centre.first;			\
		if (lib_centre.second != n) {							\
			cerr << " " << lib_centre.second;					\
		}														\
		cerr << endl;											\
		cerr << "    Straightforward: " << easy_centre.first;	\
		if (easy_centre.second != n) {							\
			cerr << " " << easy_centre.second;					\
		}														\
		cerr << endl;											\
		cerr << "    For tree:" << endl;						\
		cerr << T << endl;										\
		return err_type::test_execution;						\
	}															\
}

#define exe_exhaustive(G, n)			\
{										\
	G Gen(n);							\
	while (Gen.has_next()) {			\
		Gen.next();						\
		const auto T = Gen.get_tree();	\
		test_correctness(T)				\
	}									\
}

#define exe_random(G, n)						\
{												\
	uint32_t N;									\
	fin >> N;									\
	G Gen(n);									\
	for (uint32_t i = 0; i < N; ++i) {			\
		const auto T = Gen.get_tree();			\
		test_correctness(T)						\
	}											\
}

	uint32_t n;
	while (fin >> n) {
		if (graph_type == "ftree") {
			if (how == "exhaustive") {
				exe_exhaustive(all_ulab_free_trees, n)
			}
			else {
				exe_random(rand_ulab_free_trees, n)
			}
		}
		else {
			if (how == "exhaustive") {
				exe_exhaustive(all_ulab_rooted_trees, n)
			}
			else {
				exe_random(rand_ulab_free_trees, n)
			}
		}
	}
	return err_type::no_error;
}

err_type exe_internal_centre(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	string mode, graph_type;
	fin >> mode >> graph_type;

	if (mode != "manual" and mode != "automatic") {
		cerr << ERROR << endl;
		cerr << "    Expected execution mode 'manual' or 'automatic'." << endl;
		cerr << "    Found '" << mode << "'" << endl;
		return err_type::test_format;
	}

	if (graph_type != "ftree" and graph_type != "rtree") {
		cerr << ERROR << endl;
		cerr << "    Expected graph type 'ftree' or 'rtree'." << endl;
		cerr << "    Found '" << graph_type << "'" << endl;
		return err_type::test_format;
	}

	const err_type err =
	(mode == "manual" ?
		(graph_type == "ftree" ?
		exe_commands_utils_centre<free_tree>(fin) :
		exe_commands_utils_centre<rooted_tree>(fin))
		:
		exe_full_utils_centre(graph_type, fin)
	);

	if (err != err_type::no_error) {
		// avoid TEST_GOODBYE
		return err;
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
