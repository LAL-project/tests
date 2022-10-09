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
#include <set>

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/linarr/D.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/std_utils.hpp"
#include "common/parse_header.hpp"

namespace tests {
namespace linarr {

err_type exe_linarr_D(std::ifstream& fin) noexcept {
	const std::set<std::string> allowed_procs(
	{"D", "MDD"}
	);

	const input_list inputs = read_input_list(fin);

	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Only one input file si allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	lal::graphs::undirected_graph G;
	{
	const std::string& graph_name = inputs[0].first;
	const std::string& graph_format = inputs[0].second;
	const err_type r = io_wrapper::read_graph(graph_name, graph_format, G);
	if (r != err_type::no_error) {
		return r;
	}
	}

	std::string proc;
	fin >> proc;
	if (allowed_procs.find(proc) == allowed_procs.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Procedure '" << proc << "' was found.\n";
		for (const std::string& p : allowed_procs) {
		std::cerr << "    - " << p << '\n';
		}
		return err_type::test_format;
	}

	// linear arrangement
	const uint64_t n = G.get_num_nodes();
	lal::linear_arrangement pi(n);

	// amount of linear arrangements
	std::size_t n_linarrs;
	fin >> n_linarrs;

	for (std::size_t i = 0; i < n_linarrs; ++i) {
		// read ¡INVERSE! linear arrangement
		lal::node u;
		for (lal::position pu = 0; pu < n; ++pu) {
			fin >> u;
			pi.assign(u, pu);
		}

		// output
		std::cout << "[" << pi.inverse_as_vector() << "]: ";

		if (proc == "D") {
			std::cout << lal::linarr::sum_edge_lengths(G, pi) << '\n';
		}
		else if (proc == "MDD") {
			std::cout << lal::linarr::mean_dependency_distance_rational(G, pi) << '\n';
		}
	}

	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
