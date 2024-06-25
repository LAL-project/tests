/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
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
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/properties/bipartite_graph_colorability.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/parse_header.hpp"

namespace tests {
namespace properties {

#define output_error_graph(g, c)														\
	{																					\
	std::cerr << g << '\n';																\
	std::cerr << "    Coloring:\n";														\
	for (lal::node __u = 0; __u < n; ++__u) {											\
		std::cerr << "        " << __u << " -> " << int(c.get_color_of(__u)) << '\n';	\
	}																					\
	}

template <class graph_t>
bool is_coloring_correct
(const graph_t& g, const lal::properties::bipartite_graph_coloring& c)
noexcept
{
	for (lal::node u = 0; u < g.get_num_nodes(); ++u) {
		if (not c.is_color_valid(u)) {
			return false;
		}
	}
	return true;
}

#define output_error_graph_header(g, c, i, name)										\
	{																					\
	std::cerr << "    Graph (" << i << " -- " << inputs[i].first << ")\n";				\
	output_error_graph(g, c);															\
	}

template <class graph_t>
err_type perform_manual_test(std::ifstream& fin) noexcept {
	graph_t g;

	lal::properties::bipartite_graph_coloring c;
	std::string option;
	while (fin >> option) {
		if (option == "init") {
			uint64_t n;
			fin >> n;
			g.init(n);
		}
		else if (option == "add_edge") {
			lal::node u, v;
			fin >> u >> v;
			g.add_edge(u,v);
		}
		else if (option == "calculate_coloring") {
			c = lal::properties::bipartite_coloring(g);
		}
		else if (option == "check_coloring_correct") {
			if (not is_coloring_correct(g, c)) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Coloring is not correct\n";
				return err_type::test_execution;
			}
		}
		else if (option == "output_coloring") {
			std::cout << "Coloring:\n";
			for (lal::node u = 0; u < g.get_num_nodes(); ++u) {
				std::cout << "    color[" << u << "]= " << int(c.get_color_of(u)) << '\n';
			}
		}
		else if (option == "is_bipartite") {
			if (lal::properties::is_graph_bipartite(g, c)) {
				std::cout << "The graph is bipartite\n";
			}
			else {
				std::cout << "The graph is not bipartite\n";
			}
		}
	}
	return err_type::no_error;
}

template <class gen_t>
bool are_all_trees_bipartite(uint64_t n) noexcept {
	gen_t gen(n);
	gen.deactivate_all_postprocessing_actions();
	while (not gen.end()) {
		const auto t = gen.yield_tree();
		const auto c = lal::properties::bipartite_coloring(t);
		// make sure all colors are valid
		if (not is_coloring_correct(t, c)) {
			std::cerr << ERROR << '\n';
			std::cerr << "    There is an invalid color.\n";
			output_error_graph(t, c);
			return false;
		}
		if (not lal::properties::is_graph_bipartite(t, c)) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph is not bipartite but it should be.\n";
			output_error_graph(t, c);
			return false;
		}
	}
	return true;
}

err_type exe_properties_bipartite_coloring(std::ifstream& fin) noexcept {
	const input_list inputs = read_input_list(fin);
	std::string command;
	fin >> command;

	if (inputs.size() != 0) {
		lal::graphs::undirected_graph G;
		for (std::size_t i = 0; i < inputs.size(); ++i) {
			G.clear();
			const err_type r =
				io_wrapper::read_graph(inputs[i].first, inputs[i].second, G, false);
			if (r != err_type::no_error) { return r; }

			const auto n = G.get_num_nodes();

			const auto c = lal::properties::bipartite_coloring(G);
			// make sure all colors are valid
			if (not is_coloring_correct(G, c)) {
				std::cerr << ERROR << '\n';
				std::cerr << "    There is an invalid color.\n";
				output_error_graph_header(G, c, i, inputs[i].first);
				return err_type::test_execution;
			}

			if (command == "assert_is_bipartite") {
				if (not lal::properties::is_graph_bipartite(G, c)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Graph is not bipartite but it should be.\n";
					output_error_graph_header(G, c, i, inputs[i].first);
					return err_type::test_execution;
				}
			}
			else if (command == "assert_is_not_bipartite") {
				if (lal::properties::is_graph_bipartite(G, c)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Graph is not bipartite but it should be.\n";
					output_error_graph_header(G, c, i, inputs[i].first);
					return err_type::test_execution;
				}
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr << "    Wrong command '" << command << "'\n";
				return err_type::test_format;
			}
		}
	}
	else {
		if (command == "check_all_free_trees") {
			std::size_t n;
			fin >> n;
			if (not are_all_trees_bipartite<lal::generate::all_ulab_free_trees>(n)) {
				return err_type::test_execution;
			}
		}
		else if (command == "check_all_rooted_trees") {
			std::size_t n;
			fin >> n;
			if (not are_all_trees_bipartite<lal::generate::all_ulab_rooted_trees>(n)) {
				return err_type::test_execution;
			}
		}
		else if (command == "manual") {
			std::string graph_type;
			fin >> graph_type;
			if (graph_type == "undirected") {
				const auto r = perform_manual_test<lal::graphs::undirected_graph>(fin);
				if (r != err_type::no_error) {
					return r;
				}
			}
			else if (graph_type == "directed") {
				const auto r = perform_manual_test<lal::graphs::directed_graph>(fin);
				if (r != err_type::no_error) {
					return r;
				}
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr << "    Wrong graph type '" << graph_type << "'\n";
				return err_type::test_format;
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
