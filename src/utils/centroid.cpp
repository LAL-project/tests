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
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/utils/graphs/trees/tree_centroid.hpp>
#include <lal/utils/graphs/traversal.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"

template<class T>
pair<node,node> straightforward_centroid(const T& t, node x) {
	const uint32_t n = t.n_nodes();

	uint32_t size_cc = 0;
	vector<node> reachable;
	{
	utils::BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_rooted());
	bfs.set_process_current(
	[&](const auto&, node s) -> void { reachable.push_back(s); ++size_cc; }
	);
	bfs.start_at(x);
	}

	// allocate and initialise memory
	char *vis = static_cast<char *>(malloc(n*sizeof(char)));
	uint32_t *sizes = static_cast<uint32_t *>(malloc(n*sizeof(uint32_t)));

	node u1, u2;
	u1 = u2 = n;

	for (const node u : reachable) {
		const bool cc = utils::__lal::is_centroidal(t, size_cc, u, vis, sizes);

		if (cc) {
			if (u1 == n) { u1 = u; }
			else { u2 = u; }
		}
	}

	free(vis);
	free(sizes);
	return (u1 < u2 ? make_pair(u1,u2) : make_pair(u2,u1));
}

namespace exe_tests {

template<class G>
err_type exe_commands_utils_centroid(ifstream& fin) {
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
		else if (option == "find_centroid") {
			node s;
			fin >> s;
			const auto centroid = utils::retrieve_centroid(t, s);
			cout << "centroid: " << centroid.first;
			if (centroid.second < t.n_nodes()) { cout << " " << centroid.second; }
			cout << endl;
		}
		else if (option == "centroid_is") {
			node s;
			fin >> s;
			const auto centroid = utils::retrieve_centroid(t, s);

			uint32_t centroid_size;
			fin >> centroid_size;

			if (centroid_size == 1) {
				node u;
				fin >> u;

				if (centroid.first != u) {
					cerr << ERROR << endl;
					cerr << "    Centroid does not coincide." << endl;
					cerr << "    Result: " << centroid.first << endl;
					cerr << "    Started at: " << s << endl;
					cerr << "    Received: " << u << endl;
					return err_type::test_exe_error;
				}
			}
			else if (centroid_size == 2) {
				node u, v;
				fin >> u >> v;
				if (u > v) {
					cerr << ERROR << endl;
					cerr << "    The first vertex must be smaller than the second." << endl;
					return err_type::test_format_error;
				}
				if (u == v) {
					cerr << ERROR << endl;
					cerr << "    The two vertices are the same: " << u << " <-> " << v << "." << endl;
					return err_type::test_format_error;
				}

				if (centroid.first != u and centroid.second != v) {
					cerr << ERROR << endl;
					cerr << "    Centroids do not coincide." << endl;
					cerr << "    Result: " << centroid.first << " " << centroid.second << endl;
					cerr << "    Started at: " << s << endl;
					cerr << "    Received: " << u << " " << v << endl;
					return err_type::test_exe_error;
				}
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Centroid size has to be either 1 or 2. Instead, received '" << centroid_size << "'." << endl;
				return err_type::test_format_error;
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
			return err_type::test_format_error;
		}
	}
	return err_type::no_error;
}

err_type exe_full_utils_centroid(const string& graph_type, ifstream& fin) {
	string how;
	fin >> how;
	if (how != "exhaustive" and how != "random") {
		cerr << ERROR << endl;
		cerr << "    Method of enumeration '" << how << "' is invalid." << endl;
		cerr << "    It must be either 'exhaustive' or 'random'." << endl;
		return err_type::test_format_error;
	}

#define test_correctness(T)										\
{																\
	const auto lib_centroid = utils::retrieve_centroid(T, 0);	\
	const auto easy_centroid = straightforward_centroid(T, 0);  \
	if (lib_centroid != easy_centroid) {						\
		cerr << ERROR << endl;									\
		cerr << "    Centroids differ." << endl;				\
		cerr << "    Library: " << lib_centroid.first;			\
		if (lib_centroid.second != n) {							\
			cerr << " " << lib_centroid.second;					\
		}														\
		cerr << endl;											\
		cerr << "    Straightforward: " << easy_centroid.first;	\
		if (easy_centroid.second != n) {						\
			cerr << " " << easy_centroid.second;				\
		}														\
		cerr << endl;											\
		cerr << "    For tree:" << endl;						\
		cerr << T << endl;										\
		return err_type::test_exe_error;						\
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
		const auto T = Gen.make_rand_tree();	\
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

err_type exe_utils_centroid(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	string mode, graph_type;
	fin >> mode >> graph_type;

	if (mode != "manual" and mode != "automatic") {
		cerr << ERROR << endl;
		cerr << "    Expected execution mode 'manual' or 'automatic'." << endl;
		cerr << "    Found '" << mode << "'" << endl;
		return err_type::test_format_error;
	}

	if (graph_type != "ftree" and graph_type != "rtree") {
		cerr << ERROR << endl;
		cerr << "    Expected graph type 'ftree' or 'rtree'." << endl;
		cerr << "    Found '" << graph_type << "'" << endl;
		return err_type::test_format_error;
	}

	const err_type err =
	(mode == "manual" ?
		(graph_type == "ftree" ?
		exe_commands_utils_centroid<ftree>(fin) :
		exe_commands_utils_centroid<rtree>(fin))
		:
		exe_full_utils_centroid(graph_type, fin)
	);

	if (err != err_type::no_error) {
		// avoid TEST_GOODBYE
		return err;
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests