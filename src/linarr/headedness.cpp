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
#include <set>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/linarr/head_initial.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/time.hpp"

namespace tests {
namespace linarr {

err_type exe_linarr_headedness(const input_list& inputs, std::ifstream& fin) {
	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Only one input file si allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	lal::graphs::directed_graph G;
	{
	const std::string graph_name = inputs[0].first;
	const std::string graph_format = inputs[0].second;
	err_type r = io_wrapper::read_graph(graph_name, graph_format, G);
	if (r != err_type::no_error) {
		return r;
	}
	}

	// linear arrangement
	const uint64_t n = G.get_num_nodes();
	std::vector<lal::node> T(n);
	lal::linear_arrangement pi(n);

	// amount of linear arrangements
	std::size_t n_linarrs;
	fin >> n_linarrs;

	for (std::size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (lal::node u = 0; u < G.get_num_nodes(); ++u) {
			fin >> T[u];
			pi[ T[u] ] = u;
		}

		// output
		std::cout << "[" << T[0];
		for (lal::node j = 1; j < G.get_num_nodes(); ++j) {
			std::cout << ", " << T[j];
		}
		std::cout << "]: ";

		const lal::numeric::rational h = lal::linarr::head_initial_rational(G, pi);
		std::cout << h << '\n';
	}

	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
