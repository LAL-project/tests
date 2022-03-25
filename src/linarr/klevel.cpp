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

// C includes

// C++ includes
#include <iostream>
#include <fstream>
#include <set>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/1level.hpp>
#include <lal/linarr/2level.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace linarr {

err_type exe_linarr_klevel(const input_list& inputs, std::ifstream& fin)
noexcept
{
	const std::set<std::string> allowed_levels(
	{"1", "2"}
	);

	const std::set<std::string> allowed_procs(
	{"MDD"}
	);

	/* FUNCTIONS */

	std::string level, proc;
	fin >> level >> proc;
	if (allowed_levels.find(level) == allowed_levels.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for level.\n";
		std::cerr << "    Level '" << level << "' was found.\n";
		std::cerr << "    Valid values:\n";
		for (const std::string& l : allowed_levels) {
		std::cerr << "    - " << l << '\n';
		}
		return err_type::test_format;
	}
	if (allowed_procs.find(level) == allowed_procs.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure.\n";
		std::cerr << "    Procedure '" << proc << "' was found.\n";
		std::cerr << "    Valid values:\n";
		for (const std::string& p : allowed_procs) {
		std::cerr << "    - " << p << '\n';
		}
		return err_type::test_format;
	}

	auto MDD_F = [level](const std::vector<lal::graphs::undirected_graph>& Gs, const std::vector<lal::linear_arrangement>& pis) {
		if (level == "1") { return lal::linarr::mean_dependency_distance_1level_rational(Gs, pis); }
		if (level == "2") { return lal::linarr::mean_dependency_distance_2level_rational(Gs, pis); }

		// return invalid value
		std::cerr << ERROR << '\n';
		return lal::numeric::rational(-1);
	};

	/* TEST's CODE */

	if (inputs.size() == 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Expected at least one input.\n";
		std::cerr << "    Instead, none were given.\n";
		return err_type::test_format;
	}

	const std::size_t n_inputs = inputs.size();
	std::vector<lal::graphs::undirected_graph> Gs(n_inputs);
	for (std::size_t i = 0; i < n_inputs; ++i) {
		const std::string graph_name = inputs[i].first;
		const std::string graph_format = inputs[i].second;

		err_type r = io_wrapper::read_graph(graph_name, graph_format, Gs[i]);
		if (r != err_type::no_error) {
			return r;
		}
	}

	// amount of linear arrangements
	std::size_t n_linarrs;
	fin >> n_linarrs;

	// only valid values: 0, Gs.size()
	if (not (n_linarrs == 0 or n_linarrs == Gs.size())) {
		std::cerr << ERROR << '\n';
		std::cerr << "    The number of linear arrangements does not equal 0\n";
		std::cerr << "    and it does not equal the amount of graphs given as input.\n";
		std::cerr << "    Amount of graphs given as input: " << Gs.size() << '\n';
		std::cerr << "    Number of linear arrangements: " << n_linarrs << '\n';
		return err_type::test_format;
	}

	// linear arrangements
	std::vector<lal::linear_arrangement> pis;

	if (n_linarrs != 0) {
		// read arrangements from input
		pis = std::vector<lal::linear_arrangement>(n_linarrs);

		for (std::size_t i = 0; i < n_linarrs; ++i) {
			const uint64_t Ni = Gs[i].get_num_nodes();

			pis[i] = lal::linear_arrangement(Ni);

			// read all ¡INVERSE! linear arrangement
			lal::node u;
			for (lal::position pu = 0; pu < Ni; ++pu) {
				fin >> u;
				pis[i].assign(u, pu);
			}
		}
	}

	if (proc == "MDD") {
		const lal::numeric::rational MDD = MDD_F(Gs, pis);
		std::cout << level << "-level MDD= " << MDD << '\n';
	}

	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
