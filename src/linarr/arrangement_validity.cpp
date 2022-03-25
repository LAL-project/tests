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
#include <fstream>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/linarr/C.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace linarr {

template<class G>
err_type test_arrangement(std::ifstream& fin) noexcept {
	G g;
	{
	uint64_t num_nodes = 0;
	std::vector<lal::edge> edges;
	int u;
	while (fin >> u and u != -1) {
		lal::node v;
		fin >> v;
		edges.push_back({u,v});
		num_nodes = std::max(lal::node(u), num_nodes);
		num_nodes = std::max(v, num_nodes);
	}
	num_nodes += 1;

	g.init(num_nodes);
	g.set_edges(edges);
	}

	lal::linear_arrangement arr;
	{
	std::vector<lal::node> inv_arr;
	int p;
	while (fin >> p and p != -1) {
		inv_arr.push_back(p);
	}
	arr = lal::linear_arrangement::from_inverse(inv_arr);
	}

	std::string check;
	while (fin >> check and check != "end") {
		if (check == "permutation") {
			const bool res = lal::linarr::is_permutation(arr);
			if (not res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is not a permutation\n";
				std::cerr << "    ** but it should be!\n";
				std::cerr << "    Input: " << arr.direct_as_vector() << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "not_permutation") {
			const bool res = lal::linarr::is_permutation(arr);
			if (res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is a permutation.\n";
				std::cerr << "    ** but it should not be!\n";
				std::cerr << "    Input: " << arr.direct_as_vector() << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "arrangement") {
			const bool res = lal::linarr::is_arrangement(g, arr);
			if (not res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is not an arrangement of the input graph\n";
				std::cerr << "    ** but it should be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "not_arrangement") {
			const bool res = lal::linarr::is_arrangement(g, arr);
			if (res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is an arrangement of the input graph\n";
				std::cerr << "    ** but it should not be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "planar") {
			const bool res = lal::linarr::is_planar(g, arr);
			if (not res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is not a planar arrangement of the input graph\n";
				std::cerr << "    ** but it should be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Number of crossings: "
					 << lal::linarr::num_crossings(g, arr)
					 << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "not_planar") {
			const bool res = lal::linarr::is_planar(g, arr);
			if (res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is a planar arrangement of the input graph\n";
				std::cerr << "    ** but it should not be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Number of crossings: "
					 << lal::linarr::num_crossings(g, arr)
					 << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
		}
		else if (check == "projective") {
			if constexpr (not std::is_same_v<G, lal::graphs::rooted_tree>) {
				std::cerr << ERROR << '\n';
				std::cerr << "    This check is only allowed for rooted trees.\n";
				return err_type::test_format;
			}

			if constexpr (std::is_same_v<G, lal::graphs::rooted_tree>) {
			const bool res = lal::linarr::is_projective(g, arr);
			if (not res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is not a projective arrangement of the input graph\n";
				std::cerr << "    ** but it should be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Number of crossings: "
					 << lal::linarr::num_crossings(g, arr)
					 << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
			}
		}
		else if (check == "not_projective") {
			if constexpr (not std::is_same_v<G, lal::graphs::rooted_tree>) {
				std::cerr << ERROR << '\n';
				std::cerr << "    This check is only allowed for rooted trees.\n";
				return err_type::test_format;
			}

			if constexpr (std::is_same_v<G, lal::graphs::rooted_tree>) {
			const bool res = lal::linarr::is_projective(g, arr);
			if (res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Input arrangement is a projective arrangement of the input graph\n";
				std::cerr << "    ** but it should not be!\n";
				std::cerr << "    Arrangement: " << arr.direct_as_vector() << '\n';
				std::cerr << "    Number of crossings: "
					 << lal::linarr::num_crossings(g, arr)
					 << '\n';
				std::cerr << "    Graph:\n";
				std::cerr << g << '\n';
				return err_type::test_execution;
			}
			}
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Unknown assertion '" << check << "'.\n";
			return err_type::test_format;
		}
	}
	return err_type::no_error;
}

err_type exe_linarr_arrangement_validity
(const input_list& inputs, std::ifstream& fin)
noexcept
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::string graph_type;
	while (fin >> graph_type) {
		err_type err;
		if (graph_type == "directed_graph") {
			err = test_arrangement<lal::graphs::directed_graph>(fin);
		}
		else if (graph_type == "undirected_graph") {
			err = test_arrangement<lal::graphs::undirected_graph>(fin);
		}
		else if (graph_type == "free_tree") {
			err = test_arrangement<lal::graphs::free_tree>(fin);
		}
		else if (graph_type == "rooted_tree") {
			err = test_arrangement<lal::graphs::rooted_tree>(fin);
		}
		else {
			err = err_type::test_format;
			std::cerr << ERROR << '\n';
			std::cerr << "    Unknown graph type '" << graph_type << "'.\n";
		}

		if (err != err_type::no_error) {
			return err;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests

