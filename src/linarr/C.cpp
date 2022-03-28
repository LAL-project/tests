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
#include <set>

// lal includes
#include <lal/linarr/C.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/properties/Q.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/linarr/algorithms_C.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

// linarr includes
#include "linarr/n_crossings_m2.hpp"

namespace tests {
namespace linarr {

#define output_arr_graph												\
	std::cerr << "    For linear arrangement function " << i << ":\n";	\
	std::cerr << "        " << pi.inverse_as_vector() << '\n';			\
	std::cerr << "    For (directed) graph\n";							\
	std::cerr << dG << '\n';											\
	std::cerr << "    For (undirected) graph\n";						\
	std::cerr << uG << '\n';											\

err_type exe_linarr_C(const input_list& inputs, std::ifstream& fin, bool has_upper_bound) {
	std::set<std::string> allowed_procs({
		"bruteforce", "dyn_prog", "ladder", "stack_based"
	});

	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Only one input file si allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	lal::graphs::undirected_graph uG;
	lal::graphs::directed_graph dG;
	{
	const std::string graph_name = inputs[0].first;
	const std::string graph_format = inputs[0].second;
	err_type r;
	r = io_wrapper::read_graph(graph_name, graph_format, uG);
	if (r != err_type::no_error) { return r; }
	r = io_wrapper::read_graph(graph_name, graph_format, dG);
	if (r != err_type::no_error) { return r; }
	}

	std::string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Procedure '" << proc << "' was found.\n";
		return err_type::test_format;
	}

	// linear arrangement
	const uint64_t n = uG.get_num_nodes();
	lal::linear_arrangement pi(n);

	// amount of linear arrangements
	std::size_t n_linarrs;
	fin >> n_linarrs;

	for (std::size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		lal::position p;
		for (lal::node u = 0; u < n; ++u) {
			fin >> p;
			pi.assign(u, p);
		}

		const uint64_t uCbf = num_crossings_brute_force(uG, pi);
		const uint64_t dCbf = num_crossings_brute_force(dG, pi);
		if (uCbf != dCbf) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Number of crossings calculated by bruteforce do not coincide.\n";
			std::cerr << "    Comparing results for directed and undirected graphs.\n";
			std::cerr << "    uCbf= " << uCbf << '\n';
			std::cerr << "    dCbf= " << dCbf << '\n';
			output_arr_graph;
			return err_type::test_execution;
		}

		uint64_t uC = 0, dC = 0;
		const auto choose_algo =
		[](const std::string& name) {
			if (name == "dyn_prog") { return lal::linarr::algorithms_C::dynamic_programming; }
			if (name == "ladder") { return lal::linarr::algorithms_C::ladder; }
			if (name == "stack_based") { return lal::linarr::algorithms_C::stack_based; }
			return lal::linarr::algorithms_C::brute_force;
		}(proc);

		uint64_t upper_bound;
		if (has_upper_bound) {
			fin >> upper_bound;
			uC = is_num_crossings_lesseq_than(uG, pi, upper_bound, choose_algo);
			dC = is_num_crossings_lesseq_than(dG, pi, upper_bound, choose_algo);
		}
		else {
			uC = num_crossings(uG, pi, choose_algo);
			dC = num_crossings(dG, pi, choose_algo);
		}

		if (uC != dC) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Number of crossings for the directed graph does not\n";
			std::cerr << "    coincide with the undirected graph.\n";
			std::cerr << "    uC= " << uC << '\n';
			std::cerr << "    dC= " << dC << '\n';
			output_arr_graph;
			return err_type::test_execution;
		}

		if (has_upper_bound) {
			if (uCbf > upper_bound) {
				if (uC <= upper_bound) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Expected number of crossings to be > upper_bound.\n";
					std::cerr << "    Instead, received: " << uC << '\n';
					std::cerr << "    Actual number of crossings: " << uCbf << '\n';
					std::cerr << "    Upper bound: " << upper_bound << '\n';
					output_arr_graph;
					return err_type::test_execution;
				}
			}
			else if (uC != uCbf) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Number of crossings obtained with the algorithm does not\n";
				std::cerr << "    coincide with the number of crossings obtained by brute force.\n";
				std::cerr << "        brute force: " << uCbf << '\n';
				std::cerr << "        " << proc << ": " << uC << '\n';
				output_arr_graph;
				return err_type::test_execution;
			}
		}
		else {
			if (uC != uCbf) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Number of crossings obtained with the algorithm does not\n";
				std::cerr << "    coincide with the number of crossings obtained by brute force.\n";
				std::cerr << "        brute force: " << uCbf << '\n';
				std::cerr << "        " << proc << ": " << uC << '\n';
				output_arr_graph;
				return err_type::test_execution;
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
