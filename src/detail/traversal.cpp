/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <fstream>
#include <random>
#include <set>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/detail/graphs/cycles.hpp>
#include <lal/detail/graphs/reachability.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

namespace tests {
namespace detail {

template <class graph_t>
err_type process_common_assertions
(const graph_t& g, const std::string& assert_what, std::ifstream& fin)
noexcept
{
	lal::node s, t;
	if (assert_what == "is_reachable") {
		fin >> s >> t;
		const bool is = lal::detail::is_node_reachable_from(g, s, t);
		if (not is) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Vertex " << t << " is not reachable from " << s << '\n';
			return err_type::test_execution;
		}
	}
	else if (assert_what == "is_not_reachable") {
		fin >> s >> t;
		const bool is = lal::detail::is_node_reachable_from(g, s, t);
		if (is) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Vertex " << t << " is reachable from " << s << '\n';
			return err_type::test_execution;
		}
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unknow assertion '" << assert_what << "'.\n";
		return err_type::test_format;
	}
	return err_type::no_error;
}

err_type process_assert(const lal::graphs::directed_graph& g, std::ifstream& fin)
noexcept
{
	std::string assert_what;
	fin >> assert_what;
	if (assert_what == "has_dir_cycle") {
		const bool has = lal::detail::has_directed_cycles(g);
		if (not has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does not have directed cycles\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == "hasnt_dir_cycle") {
		const bool has = lal::detail::has_directed_cycles(g);
		if (has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does have directed cycles\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == "has_undir_cycle") {
		const bool has = lal::detail::has_undirected_cycles(g);
		if (not has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does not have undirected cycles\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == "hasnt_undir_cycle") {
		const bool has = lal::detail::has_undirected_cycles(g);
		if (has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does have undirected cycles\n";
			return err_type::test_execution;
		}
	}
	else {
		return process_common_assertions(g, assert_what, fin);
	}

	return err_type::no_error;
}

err_type process_assert(const lal::graphs::undirected_graph& g, std::ifstream& fin)
noexcept
{
	std::string assert_what;
	fin >> assert_what;
	if (assert_what == "has_undir_cycle") {
		const bool has = lal::detail::has_undirected_cycles(g);
		if (not has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does not have undirected cycles\n";
			return err_type::test_execution;
		}
	}
	else if (assert_what == "hasnt_undir_cycle") {
		const bool has = lal::detail::has_undirected_cycles(g);
		if (has) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Graph does have undirected cycles\n";
			return err_type::test_execution;
		}
	}
	else {
		return process_common_assertions(g, assert_what, fin);
	}

	return err_type::no_error;
}

template <class graph_t>
err_type process_instruction(const graph_t& g, const std::string& command, std::ifstream& fin)
noexcept
{
	if (command == "output_graph") {
		std::cout << g << '\n';
	}
	else if (command == "is_reachable") {

	}
	else if (command == "assert") {
		const auto e = process_assert(g, fin);
		if (e != err_type::no_error) { return e; }
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unrecognised command '" << command << "'\n";
		return err_type::test_format;
	}

	return err_type::no_error;
}

template <class graph_t>
err_type test_without_graph(std::ifstream& fin) noexcept {
	graph_t g;
	std::string command;
	while (fin >> command) {
		if (command == "add_edge") {
			lal::node u,v;
			fin >> u >> v;
			g.add_edge(u,v);
		}
		else if (command == "init") {
			uint64_t n;
			fin >> n;
			g.init(n);
		}
		else {
			err_type r = process_instruction(g, command, fin);
			if (r != err_type::no_error) {
				return r;
			}
		}
	}
	return err_type::no_error;
}

template <class graph_t>
err_type execute_utils_bfs_test(
	const std::string& graph_name,
	const std::string& graph_format,
	std::ifstream& fin
)
noexcept
{
	graph_t g;
	err_type r1 = io_wrapper::read_graph(graph_name, graph_format, g);
	if (r1 != err_type::no_error) { return r1; }

	std::string command;
	while (fin >> command) {
		r1 = process_instruction(g, command, fin);
		if (r1 != err_type::no_error) { return r1; }
	}
	return err_type::no_error;
}

err_type exe_detail_bfs(std::ifstream& fin) noexcept {
	const std::set<std::string> allowed_options({
		"sort_insertion", "sort_boolean"
	});

	const input_list inputs = read_input_list(fin);

	if (inputs.size() > 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    At most one input file is allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::string graph_type;
	fin >> graph_type;
	if (graph_type != "ugraph" and graph_type != "dgraph") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Incorrect type of graph specified.\n";
		std::cerr << "    Recevied: '" << graph_type << "'.\n";
		return err_type::test_format;
	}

	err_type r = err_type::no_error;
	std::cout << "graph_type: " << graph_type << '\n';
	if (inputs.size() == 1) {
		const std::string& file_name = inputs[0].first;
		const std::string& file_format = inputs[0].second;
		r = (graph_type == "ugraph" ?
			execute_utils_bfs_test<lal::graphs::undirected_graph>(file_name, file_format, fin) :
			execute_utils_bfs_test<lal::graphs::directed_graph>(file_name, file_format, fin)
		);
	}
	else {
		r = (graph_type == "ugraph" ?
			test_without_graph<lal::graphs::undirected_graph>(fin) :
			test_without_graph<lal::graphs::directed_graph>(fin)
		);
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace detail
} // -- namespace tests
