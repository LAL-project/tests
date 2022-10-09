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
#include <lal/graphs/undirected_graph.hpp>
#include <lal/io/basic_output.hpp>

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
	std::cerr << "        " << arrangements[i].direct_as_vector() << '\n';	\
	std::cerr << "    For (directed) graph\n";							\
	std::cerr << dG << '\n';											\
	std::cerr << "    For (undirected) graph\n";						\
	std::cerr << uG << '\n';											\

err_type exe_linarr_C_list
(const input_list& inputs, std::ifstream& fin, char upper_bound_type)
noexcept
{
	const std::set<std::string> allowed_procs(
	{"bruteforce", "dyn_prog", "ladder", "stack_based"}
	);

	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Only one input file si allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	lal::graphs::undirected_graph uG;
	lal::graphs::directed_graph dG;
	{
	const std::string& graph_name = inputs[0].first;
	const std::string& graph_format = inputs[0].second;
	err_type r;
	r = io_wrapper::read_graph(graph_name, graph_format, uG);
	if (r != err_type::no_error) { return r; }

	r = io_wrapper::read_graph(graph_name, graph_format, dG);
	if (r != err_type::no_error) { return r; }
	}

	std::string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		std::cerr <<ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Procedure '" << proc << "' was found.\n";
		std::cerr << "    Valid values:\n";
		for (const std::string& p : allowed_procs) {
		std::cerr << "    - " << p << '\n';
		}
		return err_type::test_format;
	}

	const uint64_t n = uG.get_num_nodes();

	// amount of linear arrangements
	uint64_t n_linarrs;
	fin >> n_linarrs;

	// linear arrangements
	std::vector<lal::linear_arrangement> arrangements
			(n_linarrs, lal::linear_arrangement(n));

	uint64_t single_upper_bound;
	std::vector<uint64_t> list_upper_bounds(n_linarrs, 0);

	for (std::size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (lal::node u = 0; u < n; ++u) {
			lal::position p;
			fin >> p;
			arrangements[i].assign(u, p);
		}

		if (upper_bound_type == 2) {
			fin >> list_upper_bounds[i];
		}
	}
	if (upper_bound_type == 1) {
		fin >> single_upper_bound;
	}

	const std::vector<uint64_t> uCbfs = num_crossings_brute_force(uG, arrangements);
	const std::vector<uint64_t> dCbfs = num_crossings_brute_force(dG, arrangements);
	for (uint64_t i = 0; i < n_linarrs; ++i) {
		if (uCbfs[i] != dCbfs[i]) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Number of crossings do not coincide\n";
			std::cerr << "        uCbfs: " << uCbfs[i] << '\n';
			std::cerr << "        dCbfs: " << dCbfs[i] << '\n';
			std::cerr << "    For linear arrangement " << i << ":\n";
			std::cerr << "        " << arrangements[i].direct_as_vector() << '\n';
			return err_type::test_execution;
		}
	}
	// uCbfs == dCbfs

	const auto choose_algo =
	[](const std::string& name) {
		if (name == "dyn_prog") { return lal::linarr::algorithms_C::dynamic_programming; }
		if (name == "ladder") { return lal::linarr::algorithms_C::ladder; }
		if (name == "stack_based") { return lal::linarr::algorithms_C::stack_based; }
		return lal::linarr::algorithms_C::brute_force;
	}(proc);

	std::vector<uint64_t> uCs, dCs;

	// compute all C
	if (upper_bound_type == 0) {
		uCs = num_crossings_list(uG, arrangements, choose_algo);
		dCs = num_crossings_list(dG, arrangements, choose_algo);
	}
	else if (upper_bound_type == 1) {
		uCs = is_num_crossings_lesseq_than_list(uG, arrangements, single_upper_bound, choose_algo);
		dCs = is_num_crossings_lesseq_than_list(dG, arrangements, single_upper_bound, choose_algo);
	}
	else if (upper_bound_type == 2) {
		uCs = is_num_crossings_lesseq_than_list(uG, arrangements, list_upper_bounds, choose_algo);
		dCs = is_num_crossings_lesseq_than_list(dG, arrangements, list_upper_bounds, choose_algo);
	}

	for (uint64_t i = 0; i < n_linarrs; ++i) {
		if (dCs[i] != uCs[i]) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Number of crossings do not coincide\n";
			std::cerr << "        " << proc << " (undirected): " << uCs[i] << '\n';
			std::cerr << "        " << proc << " (directed): " << dCs[i] << '\n';
			output_arr_graph;
			return err_type::test_execution;
		}
	}
	// uCs == dCs

	if (upper_bound_type == 0) {
		for (uint64_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] != uCs[i]) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Number of crossings do not coincide\n";
				std::cerr << "        brute force: " << uCbfs[i] << '\n';
				std::cerr << "        " << proc << ": " << uCs[i] << '\n';
				output_arr_graph;
				return err_type::test_execution;
			}
		}
	}
	else if (upper_bound_type == 1) {
		for (uint64_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] > single_upper_bound) {
				if (uCs[i] <= list_upper_bounds[i]) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Expected number of crossings to be > upper_bound.\n";
					std::cerr << "    Instead, received: " << uCs[i] << '\n';
					std::cerr << "    Actual number of crossings: " << uCbfs[i] << '\n';
					std::cerr << "    Upper bound: " << single_upper_bound << '\n';
					output_arr_graph;
					return err_type::test_execution;
				}
			}
			else if (uCs[i] != uCbfs[i]) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Number of crossings obtained with the algorithm does not\n";
				std::cerr << "    coincide with the number of crossings obtained by brute force.\n";
				std::cerr << "        brute force: " << uCbfs[i] << '\n';
				std::cerr << "        " << proc << ": " << uCs[i] << '\n';
				output_arr_graph;
				return err_type::test_execution;
			}
		}
	}
	else if (upper_bound_type == 2) {
		for (uint64_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] > list_upper_bounds[i]) {
				if (uCs[i] <= list_upper_bounds[i]) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Expected number of crossings to be > upper_bound.\n";
					std::cerr << "    Instead, received: " << uCs[i] << '\n';
					std::cerr << "    Actual number of crossings: " << uCbfs[i] << '\n';
					std::cerr << "    Upper bound: " << list_upper_bounds[i] << '\n';
					output_arr_graph;
					return err_type::test_execution;
				}
			}
			else if (uCs[i] != uCbfs[i]) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Number of crossings obtained with the algorithm does not\n";
				std::cerr << "    coincide with the number of crossings obtained by brute force.\n";
				std::cerr << "        brute force: " << uCbfs[i] << '\n';
				std::cerr << "        " << proc << ": " << uCs[i] << '\n';
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
