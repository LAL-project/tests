/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/detail/properties/tree_diameter.hpp>
#include <lal/detail/graphs/traversal.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

namespace tests {
namespace properties {

// this is a very simple algorithm to calculate the centre
template <class tree_t>
uint64_t straightforward_diameter(const tree_t& tree, lal::node u) noexcept
{
	const auto max_distance_from_source = [](const tree_t& t, lal::node a)
	{
		lal::detail::array<uint64_t> dis(t.get_num_nodes(), 0);
		uint64_t max_dist = 0;

		lal::detail::BFS bfs(t);
		if constexpr (std::is_base_of_v<lal::graphs::rooted_tree, tree_t>) {
			bfs.set_use_rev_edges(true);
		}
		bfs.set_process_neighbour(
			[&](const lal::node v, const lal::node w, const bool)
			{
				dis[w] = dis[v] + 1;
				max_dist = std::max(max_dist, dis[w]);
			}
		);

		bfs.start_at(a);
		return max_dist;
	};

	const auto n = tree.get_num_nodes();

	uint64_t max_dist = 0;
	const auto cc_u = tree.get_component_representative(u);
	for (lal::node v = 0; v < n; ++v) {
		if (tree.get_component_representative(v) != cc_u) {
			continue;
		}

		max_dist = std::max(max_dist, max_distance_from_source(tree, v));
	}
	return max_dist;
}

template <class tree_t>
err_type exe_commands_utils_diameter(std::ifstream& fin) noexcept
{
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
		else if (option == "find_diameter") {
			lal::node s;
			fin >> s;
			std::cout << lal::detail::tree_diameter(t, s) << '\n';
		}
		else if (option == "diameter_is") {
			lal::node s;
			fin >> s;
			const auto library_diameter = lal::detail::tree_diameter(t, s);

			uint64_t user_diameter;
			fin >> user_diameter;

			if (user_diameter != library_diameter) {
				std::cerr << ERROR << '\n';
				std::cerr
					<< "    Calculations of diameters do not agree at vertex '"
					<< s << "'.\n";
				std::cerr << "        Library diameter= " << library_diameter
						  << ".\n";
				std::cerr << "        User diameter=    " << user_diameter
						  << ".\n";
				return err_type::test_format;
			}
		}
		else if (option == "output_graph") {
			std::cout << t << '\n';
		}
		else if (option == "remove_edge") {
			lal::node u, v;
			fin >> u >> v;
			t.remove_edge(u, v);
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Invalid command '" << option << "'.\n";
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

err_type exe_full_utils_diameter(
	const std::string& graph_type, std::ifstream& fin
) noexcept
{
	std::string how;
	fin >> how;
	if (how != "exhaustive" and how != "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Method of enumeration '" << how << "' is invalid.\n";
		std::cerr << "    It must be either 'exhaustive' or 'random'.\n";
		return err_type::test_format;
	}

#define test_correctness(T)                                                    \
	{                                                                          \
		const auto lib_diam = lal::detail::tree_diameter(T, 0);                \
		const auto easy_diam = straightforward_diameter(T, 0);                 \
		if (lib_diam != easy_diam) {                                           \
			std::cerr << ERROR << '\n';                                        \
			std::cerr << "    Diameters differ.\n";                            \
			std::cerr << "    Library: " << lib_diam << '\n';                  \
			std::cerr << "    Straightforward: " << easy_diam << '\n';         \
			std::cerr << "    For tree:\n";                                    \
			std::cerr << T << '\n';                                            \
			return err_type::test_execution;                                   \
		}                                                                      \
	}

#define exe_exhaustive(G, n)                                                   \
	{                                                                          \
		for (G Gen(n); not Gen.end(); Gen.next()) {                            \
			const auto T = Gen.get_tree();                                     \
			test_correctness(T)                                                \
		}                                                                      \
	}

#define exe_random(G, n)                                                       \
	{                                                                          \
		uint64_t N;                                                            \
		fin >> N;                                                              \
		G Gen(n);                                                              \
		for (uint64_t i = 0; i < N; ++i) {                                     \
			const auto T = Gen.get_tree();                                     \
			test_correctness(T)                                                \
		}                                                                      \
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

err_type exe_properties_diameter(std::ifstream& fin) noexcept
{

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
		(mode == "manual"
			 ? (graph_type == "ftree"
					? exe_commands_utils_diameter<lal::graphs::free_tree>(fin)
					: exe_commands_utils_diameter<lal::graphs::rooted_tree>(fin)
			   )
			 : exe_full_utils_diameter(graph_type, fin));

	if (err != err_type::no_error) {
		// avoid TEST_GOODBYE;
		return err;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace properties
} // namespace tests
