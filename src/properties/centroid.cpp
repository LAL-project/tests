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

// C++ includes
#include <iostream>
#include <cstring>
#include <fstream>
#include <random>
#include <set>
// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/detail/properties/tree_centroid.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/graphs/traversal.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

inline bool is_centroidal(
	const lal::graphs::rooted_tree& t, uint64_t size_cc, lal::node u, uint64_t *sizes
)
{
	memset(sizes, 0, t.get_num_nodes()*sizeof(uint64_t));
	lal::detail::get_size_subtrees(t, u, sizes);
	for (const lal::node v : t.get_out_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
		}
	}
	for (const lal::node v : t.get_in_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
		}
	}
	return true;
}

inline bool is_centroidal(
	const lal::graphs::free_tree& t, uint64_t size_cc, lal::node u, uint64_t *sizes
)
{
	memset(sizes, 0, t.get_num_nodes()*sizeof(uint64_t));
	lal::detail::get_size_subtrees(t, u, sizes);
	for (const lal::node v : t.get_neighbours(u)) {
		if (sizes[v] > size_cc/2) {
			return false;
		}
	}
	return true;
}

template <class T>
std::pair<lal::node,lal::node> straightforward_centroid(const T& t, lal::node x) {
	const uint64_t n = t.get_num_nodes();

	uint64_t size_cc = 0;
	std::vector<lal::node> reachable;
	{
	lal::detail::BFS<T> bfs(t);
	bfs.set_use_rev_edges(t.is_rooted());
	bfs.set_process_current(
	[&](const auto&, lal::node s) -> void { reachable.push_back(s); ++size_cc; }
	);
	bfs.start_at(x);
	}

	// allocate memory
	uint64_t *sizes = static_cast<uint64_t *>(malloc(n*sizeof(uint64_t)));

	lal::node u1, u2;
	u1 = u2 = n;

	for (const lal::node u : reachable) {
		const bool cc = is_centroidal(t, size_cc, u, sizes);
		if (cc) {
			if (u1 == n) { u1 = u; }
			else { u2 = u; }
		}
	}

	free(sizes);
	return (u1 < u2 ? std::make_pair(u1,u2) : std::make_pair(u2,u1));
}

inline
bool are_centroids_equal(
	const lal::graphs::tree& t,
	const std::pair<lal::node,lal::node>& c1, const std::pair<lal::node,lal::node>& c2
)
{
	if (c1.first != c2.first) { return false; }
	if (c1.second < t.get_num_nodes()) {
		return c1.second == c2.second;
	}
	return true;
}

namespace tests {
namespace properties {

template <class TREE_TYPE>
err_type exe_commands_utils_centroid(std::ifstream& fin) {
	TREE_TYPE t;
	uint64_t n;

	std::string option;
	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
			continue;
		}

		if (option == "output") {
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
			t.add_edges(es);
		}
		else if (option == "find_centroid") {
			lal::node s;
			fin >> s;
			const auto centroid = lal::detail::retrieve_centroid(t, s);
			std::cout << "centroid: " << centroid.first;
			if (centroid.second < t.get_num_nodes()) { std::cout << " " << centroid.second; }
			std::cout << '\n';
		}
		else if (option == "centroid_is") {
			lal::node start_at;
			fin >> start_at;
			const auto centroid = lal::detail::retrieve_centroid(t, start_at);

			uint64_t centroid_size;
			fin >> centroid_size;

			if (centroid_size == 1) {
				lal::node u;
				fin >> u;

				if (centroid.first != u) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Centroid does not coincide.\n";
					std::cerr << "    Started at: " << start_at << '\n';
					std::cerr << "    LAL: " << centroid.first << '\n';
					std::cerr << "    Brute force: " << u << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
			}
			else if (centroid_size == 2) {
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

				if (centroid.first != u and centroid.second != v) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Centroids do not coincide.\n";
					std::cerr << "    Started at: " << start_at << '\n';
					std::cerr << "    LAL: " << centroid.first << " " << centroid.second << '\n';
					std::cerr << "    Brute force: " << u << " " << v << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr
					<< "    Centroid size has to be either 1 or 2. Instead, received '"
					<< centroid_size << "'.\n";
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

err_type exe_full_utils_centroid(const std::string& graph_type, std::ifstream& fin) {
	std::string how;
	fin >> how;
	if (how != "exhaustive" and how != "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Method of enumeration '" << how << "' is invalid.\n";
		std::cerr << "    It must be either 'exhaustive' or 'random'.\n";
		return err_type::test_format;
	}

#define test_correctness(T)											\
{																	\
	const auto lib_centroid = lal::detail::retrieve_centroid(T, 0);	\
	const auto easy_centroid = straightforward_centroid(T, 0);		\
	if (not are_centroids_equal(T,lib_centroid, easy_centroid)) {	\
		std::cerr << ERROR << '\n';									\
		std::cerr << "    Centroids differ.\n";						\
		std::cerr << "    Library: " << lib_centroid.first;			\
		if (lib_centroid.second < n) {								\
			std::cerr << " " << lib_centroid.second;				\
		}															\
		std::cerr << '\n';											\
		std::cerr << "    Straightforward: " << easy_centroid.first;\
		if (easy_centroid.second < n) {								\
			std::cerr << " " << easy_centroid.second;				\
		}															\
		std::cerr << '\n';											\
		std::cerr << "    For tree:\n";								\
		std::cerr << T << '\n';										\
		return err_type::test_execution;							\
	}																\
}

#define exe_exhaustive(G, n)					\
{												\
	for (G Gen(n); not Gen.end(); Gen.next()) {	\
		const auto T = Gen.get_tree();			\
		test_correctness(T)						\
	}											\
}

#define exe_random(G, n)						\
{												\
	uint64_t N;									\
	fin >> N;									\
	G Gen(n);									\
	for (uint64_t i = 0; i < N; ++i) {			\
		const auto T = Gen.get_tree();			\
		test_correctness(T)						\
	}											\
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

err_type exe_properties_centroid(const input_list& inputs, std::ifstream& fin)
noexcept
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

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
		exe_commands_utils_centroid<lal::graphs::free_tree>(fin) :
		exe_commands_utils_centroid<lal::graphs::rooted_tree>(fin))
		:
		exe_full_utils_centroid(graph_type, fin)
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
