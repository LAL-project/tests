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

// C++ includes
#include <iostream>
#include <fstream>
#include <random>
#include <set>

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/detail/properties/tree_centre.hpp>
#include <lal/detail/graphs/traversal.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

namespace tests {
namespace properties {

bool are_centres_equal(
	const uint64_t n,
	const std::pair<lal::node,lal::node>& c1,
	const std::pair<lal::node,lal::node>& c2
)
noexcept
{
	if (c1.first != c2.first) { return false; }
	if (c1.second < n and c2.second >= n) { return false; }
	if (c1.second >= n and c2.second < n) { return false; }
	if (c1.second >= n and c2.second >= n) { return true; }
	if (c1.second != c2.second) { return false; }
	return true;
}

// this is a very simple algorithm to calculate the centre
template <class T>
std::pair<lal::node, lal::node> straightforward_centre(const T& tree, lal::node u)
noexcept
{
	const auto n = tree.get_num_nodes();

	// First simple case:
	// in case the component of x has only one lal::node (lal::node x)...
	if (tree.get_degree(u) == 0) {
		return std::make_pair(u, n);
	}

	lal::detail::BFS<T> bfs(tree);
	bfs.set_use_rev_edges(tree.is_directed());

	// ------------------------------------
	// 1. find vertex 'v' farthest from 'u'

	std::vector<uint64_t> dists(tree.get_num_nodes());
	dists[u] = 0;

	// farthest vertex from 'u' and distance
	uint64_t max_dist = 0;
	lal::node v = n + 1;

	bfs.set_process_neighbour(
	[&](const auto&, lal::node s, lal::node t, bool) {
		dists[t] = dists[s] + 1;
		if (max_dist < dists[t]) {
			max_dist = dists[t];
			v = t;
		}
	}
	);
	bfs.start_at(u);

#if defined DEBUG
	assert(v < n);
#endif

	// early termination
	if (v == u) {
		return std::make_pair(u, n);
	}

	// ------------------------------------
	// 2. find vertex 'w' farthest from 'v' and keep
	// track of the vertices from 'v' to 'w'

	std::fill(dists.begin(), dists.end(), 0);
	std::vector<std::vector<lal::node>> paths(tree.get_num_nodes());
	paths[v] = {v};

	max_dist = 0;
	std::vector<lal::node> longest_path;
	lal::node w = n + 1;

	bfs.clear_visited();
	bfs.clear_queue();
	bfs.set_process_neighbour(
	[&](const auto&, lal::node s, lal::node t, bool) {
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

#if defined DEBUG
	assert(w < n);
#endif

	// return centre

	if (longest_path.size()%2 == 0) {
		const std::size_t hm1 = longest_path.size()/2 - 1;
		const std::size_t h = longest_path.size()/2;

		const lal::node c1 = longest_path[hm1];
		const lal::node c2 = longest_path[h];
		return (c1 < c2 ? std::make_pair(c1,c2) : std::make_pair(c2,c1));
	}
	const std::size_t h = longest_path.size()/2;
	return std::make_pair(longest_path[h], n);
}

template <class tree_t>
err_type exe_commands_utils_centre(std::ifstream& fin) noexcept {
	tree_t t;
	uint64_t n;

	std::string option;
	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
		}
		else if (option == "output") {
			std::cout << read_output_string(fin) << '\n';
		}
		else if (option == "init") {
			fin >> n;
			t.init(n);
		}
		else if (option == "add_edges") {
			fin >> n;
			std::vector<lal::edge> es(n);
			for (uint64_t i = 0; i < n; ++i) {
				fin >> es[i].first >> es[i].second;
			}
			if (n == t.get_num_nodes() - 1) {
				t.set_edges(es);
			}
			else {
				t.add_edges(es);
			}
		}
		else if (option == "find_centre") {
			lal::node s;
			fin >> s;
			const auto centre = lal::detail::retrieve_centre(t, s);
			std::cout << "centre: " << centre.first;
			if (centre.second < t.get_num_nodes()) { std::cout << " " << centre.second; }
			std::cout << '\n';
		}
		else if (option == "centre_is") {
			lal::node s;
			fin >> s;
			const auto centre = lal::detail::retrieve_centre(t, s);

			uint64_t centre_size;
			fin >> centre_size;

			if (centre_size == 1) {
				lal::node u;
				fin >> u;

				if (centre.first != u) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Centre does not coincide.\n";
					return err_type::test_execution;
				}
			}
			else if (centre_size == 2) {
				lal::node u, v;
				fin >> u >> v;
				if (u > v) {
					std::cerr << ERROR << '\n';
					std::cerr << "    The first vertex must be smaller than the second.\n";
					return err_type::test_format;
				}
				if (u == v) {
					std::cerr << ERROR << '\n';
					std::cerr << "    The two vertices are the same: " << u << " <-> " << v << ".\n";
					return err_type::test_format;
				}

				if (centre.first != u and centre.second != v) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Centres do not coincide.\n";
					return err_type::test_execution;
				}
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr << "    Centre size has to be either 1 or 2. Instead, received '" << centre_size << "'.\n";
				return err_type::test_format;
			}
		}
		else if (option == "output_graph") {
			std::cout << t << '\n';
		}
		else if (option == "remove_edge") {
			lal::node u,v;
			fin >> u >> v;
			t.remove_edge(u,v);
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Invalid command '" << option << "'.\n";
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

err_type exe_full_utils_centre(const std::string& graph_type, std::ifstream& fin)
noexcept
{
	std::string how;
	fin >> how;
	if (how != "exhaustive" and how != "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Method of enumeration '" << how << "' is invalid.\n";
		std::cerr << "    It must be either 'exhaustive' or 'random'.\n";
		return err_type::test_format;
	}

#define test_correctness(T)										\
{																\
	const auto lib_centre = lal::detail::retrieve_centre(T, 0);	\
	const auto easy_centre = straightforward_centre(T, 0);		\
	if (not are_centres_equal(T.get_num_nodes(), lib_centre, easy_centre)) { \
		std::cerr << ERROR << '\n';								\
		std::cerr << "    Centres differ.\n";					\
		std::cerr << "    Library: " << lib_centre.first;		\
		if (lib_centre.second < n) {							\
			std::cerr << " " << lib_centre.second;				\
		}														\
		std::cerr << '\n';										\
		std::cerr << "    Straightforward: " << easy_centre.first;	\
		if (easy_centre.second < n) {							\
			std::cerr << " " << easy_centre.second;				\
		}														\
		std::cerr << '\n';										\
		std::cerr << "    For tree:\n";							\
		std::cerr << T << '\n';									\
		return err_type::test_execution;						\
	}															\
}

#define exe_exhaustive(G, n)					\
{												\
	for (G Gen(n); not Gen.end(); Gen.next()) {	\
		const auto T = Gen.get_tree();			\
		test_correctness(T)						\
	}											\
}

#define exe_random(G, n)				\
{										\
	uint64_t N;							\
	fin >> N;							\
	G Gen(n);							\
	for (uint64_t i = 0; i < N; ++i) {	\
		const auto T = Gen.get_tree();	\
		test_correctness(T)				\
	}									\
}

	uint64_t n;
	while (fin >> n) {
		if (graph_type == "ftree") {
			if (how == "exhaustive") {
				exe_exhaustive(lal::generate::all_ulab_free_trees, n);
			}
			else {
				exe_random(lal::generate::rand_ulab_free_trees, n);
			}
		}
		else {
			if (how == "exhaustive") {
				exe_exhaustive(lal::generate::all_ulab_rooted_trees, n);
			}
			else {
				exe_random(lal::generate::rand_ulab_free_trees, n);
			}
		}
	}
	return err_type::no_error;
}

err_type exe_properties_centre(std::ifstream& fin) noexcept {

	std::string mode, graph_type;
	fin >> mode >> graph_type;

	if (mode != "manual" and mode != "automatic") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Expected execution mode 'manual' or 'automatic'.\n";
		std::cerr << "    Found '" << mode << "'\n";
		return err_type::test_format;
	}

	if (graph_type != "ftree" and graph_type != "rtree") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Expected graph type 'ftree' or 'rtree'.\n";
		std::cerr << "    Found '" << graph_type << "'\n";
		return err_type::test_format;
	}

	const err_type err =
	(mode == "manual" ?
		(graph_type == "ftree" ?
		exe_commands_utils_centre<lal::graphs::free_tree>(fin) :
		exe_commands_utils_centre<lal::graphs::rooted_tree>(fin))
		:
		exe_full_utils_centre(graph_type, fin)
	);

	if (err != err_type::no_error) {
		// avoid TEST_GOODBYE;
		return err;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
