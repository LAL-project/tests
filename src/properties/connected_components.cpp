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
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/output.hpp>
#include <lal/properties/connected_components_compute.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace properties {

template <class graph_t>
void run_tests(std::ifstream& fin) noexcept {
	uint64_t n;
	while (fin >> n) {
		std::cout << "=====================\n";

		graph_t g(n);

		std::size_t num_edges;
		fin >> num_edges;

		for (std::size_t i = 0; i < num_edges; ++i) {
			lal::node u, v;
			fin >> u >> v;
			g.add_edge_bulk(u, v);
		}
		g.finish_bulk_add();

		const auto ccs = lal::properties::connected_components_compute(g);
		for (lal::node u = 0; u < n; ++u) {
			std::cout
				<< u
				<< " is in connected component "
				<< ccs.get_cc_node(u)
				<< " and is vertex "
				<< ccs.get_label_graph_node_to_cc_node(u)
				<< " within its component\n";
		}

		for (std::size_t i = 0; i < ccs.size(); ++i) {
			std::cout << "---------------------\n";
			std::cout << ccs[i] << '\n';

			const auto ni = ccs[i].get_num_nodes();
			for (lal::node u = 0; u < ni; ++u) {
				std::cout
					<< "    "
					<< u
					<< " in the connected component is "
					<< ccs.get_label_cc_node_to_graph_node(i, u)
					<< " in the original graph\n";
			}
		}
	}
}

err_type exe_properties_connected_components(std::ifstream& fin) noexcept {
	std::string graph_type;
	fin >> graph_type;

	if (graph_type == "undirected") {
		run_tests<lal::graphs::undirected_graph>(fin);
	}
	else if (graph_type == "directed") {
		run_tests<lal::graphs::directed_graph>(fin);
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unhandled graph type '" << graph_type << "'\n";
		return err_type::test_format;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
