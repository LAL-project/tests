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
using namespace std;

// lal includes
#include <lal/linarr/C.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"
#include "time.hpp"
#include "linarr/n_crossings_m2.hpp"

namespace exe_tests {

err_type exe_linarr_C_list(const input_list& inputs, ifstream& fin, char upper_bound_type) {
	const set<string> allowed_procs(
	{"bruteforce", "dyn_prog", "ladder", "stack_based"}
	);

	if (inputs.size() != 1) {
		cerr << ERROR << endl;
		cerr << "    Only one input file si allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	undirected_graph uG;
	directed_graph dG;
	{
	const string graph_name = inputs[0].first;
	const string graph_format = inputs[0].second;
	err_type r;
	r = io_wrapper::read_graph(graph_name, graph_format, uG);
	if (r != err_type::no_error) { return r; }
	r = io_wrapper::read_graph(graph_name, graph_format, dG);
	if (r != err_type::no_error) { return r; }
	}

	string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		cerr <<ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		cerr << "    Valid values:" << endl;
		for (const string& p : allowed_procs) {
		cerr << "    - " << p << endl;
		}
		return err_type::test_format;
	}

	timing::time_point begin, end;
	double total_elapsed = 0.0;

	const uint32_t n = uG.num_nodes();

	// amount of linear arrangements
	uint32_t n_linarrs;
	fin >> n_linarrs;

	// linear arrangements
	vector<vector<node>> T(n_linarrs, vector<node>(uG.num_nodes()));
	vector<linear_arrangement> pis(n_linarrs, linear_arrangement(uG.num_nodes()));
	uint32_t single_upper_bound;
	vector<uint32_t> list_upper_bounds(n_linarrs, 0);

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (uint32_t u = 0; u < uG.num_nodes(); ++u) {
			fin >> T[i][u];
			pis[i][ T[i][u] ] = u;
		}

		if (upper_bound_type == 2) {
			fin >> list_upper_bounds[i];
		}
	}
	if (upper_bound_type == 1) {
		fin >> single_upper_bound;
	}

	const vector<uint32_t> uCbfs = number_of_crossings_brute_force(uG, pis);
	const vector<uint32_t> dCbfs = number_of_crossings_brute_force(dG, pis);
	for (uint32_t i = 0; i < n_linarrs; ++i) {
		if (uCbfs[i] != dCbfs[i]) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings do not coincide" << endl;
			cerr << "        uCbfs: " << uCbfs[i] << endl;
			cerr << "        dCbfs: " << dCbfs[i] << endl;
			cerr << "    For linear arrangement " << i << ":" << endl;
			cerr << "    [" << T[i][0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[i][j];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}
	}
	// uCbfs == dCbfs

	const auto choose_algo =
	[](const string& name) {
		if (name == "dyn_prog") { return algorithms_C::dynamic_programming; }
		if (name == "ladder") { return algorithms_C::ladder; }
		if (name == "stack_based") { return algorithms_C::stack_based; }
		return algorithms_C::brute_force;
	}(proc);

	vector<uint32_t> uCs, dCs;

	// compute all C
	begin = timing::now();
	if (upper_bound_type == 0) {
		uCs = number_of_crossings(uG, pis, choose_algo);
		dCs = number_of_crossings(dG, pis, choose_algo);
	}
	else if (upper_bound_type == 1) {
		uCs = is_number_of_crossings_lesseq_than(uG, pis, single_upper_bound, choose_algo);
		dCs = is_number_of_crossings_lesseq_than(dG, pis, single_upper_bound, choose_algo);
	}
	else if (upper_bound_type == 2) {
		uCs = is_number_of_crossings_lesseq_than(uG, pis, list_upper_bounds, choose_algo);
		dCs = is_number_of_crossings_lesseq_than(dG, pis, list_upper_bounds, choose_algo);
	}
	end = timing::now();
	total_elapsed += timing::elapsed_milliseconds(begin, end);

	for (uint32_t i = 0; i < n_linarrs; ++i) {
		if (dCs[i] != uCs[i]) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings do not coincide" << endl;
			cerr << "        " << proc << " (u): " << uCs[i] << endl;
			cerr << "        " << proc << " (d): " << dCs[i] << endl;
			cerr << "    For linear arrangement " << i << ":" << endl;
			cerr << "    [" << T[i][0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[i][j];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}
	}
	// uCs == dCs

	if (upper_bound_type == 0) {
		for (uint32_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] != uCs[i]) {
				cerr << ERROR << endl;
				cerr << "    Number of crossings do not coincide" << endl;
				cerr << "        brute force: " << uCbfs[i] << endl;
				cerr << "        " << proc << ": " << uCs[i] << endl;
				cerr << "    For linear arrangement " << i << ":" << endl;
				cerr << "    [" << T[i][0];
				for (size_t j = 1; j < n; ++j) {
					cerr << "," << T[i][j];
				}
				cerr << "]" << endl;
				return err_type::test_execution;
			}
		}
	}
	else if (upper_bound_type == 1) {
		for (uint32_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] > single_upper_bound) {
				if (uCs[i] != uG.num_edges()*uG.num_edges()) {
					cerr << ERROR << endl;
					cerr << "    Expected number of crossings to be m^2." << endl;
					cerr << "    Instead, received: " << uCs[i] << endl;
					cerr << "    Actual number of crossings: " << uCbfs[i] << endl;
					cerr << "    Upper bound: " << single_upper_bound << endl;
					return err_type::test_execution;
				}
			}
			else if (uCs[i] != uCbfs[i]) {
				cerr << ERROR << endl;
				cerr << "    Number of crossings obtained with the algorithm does not" << endl;
				cerr << "    coincide with the number of crossings obtained by brute force." << endl;
				cerr << "        brute force: " << uCbfs[i] << endl;
				cerr << "        " << proc << ": " << uCs[i] << endl;
				cerr << "    For inverse linear arrangement function " << i << ":" << endl;
				cerr << "    [" << T[i][0];
				for (size_t j = 1; j < n; ++j) {
					cerr << "," << T[i][j];
				}
				cerr << "]" << endl;
				cerr << "    Undirected graph:" << endl;
				cerr << uG << endl;
				cerr << "    Directed graph:" << endl;
				cerr << dG << endl;
				return err_type::test_execution;
			}
		}
	}
	else if (upper_bound_type == 2) {
		for (uint32_t i = 0; i < n_linarrs; ++i) {
			if (uCbfs[i] > list_upper_bounds[i]) {
				if (uCs[i] != uG.num_edges()*uG.num_edges()) {
					cerr << ERROR << endl;
					cerr << "    Expected number of crossings to be m^2." << endl;
					cerr << "    Instead, received: " << uCs[i] << endl;
					cerr << "    Actual number of crossings: " << uCbfs[i] << endl;
					cerr << "    Upper bound: " << list_upper_bounds[i] << endl;
					return err_type::test_execution;
				}
			}
			else if (uCs[i] != uCbfs[i]) {
				cerr << ERROR << endl;
				cerr << "    Number of crossings obtained with the algorithm does not" << endl;
				cerr << "    coincide with the number of crossings obtained by brute force." << endl;
				cerr << "        brute force: " << uCbfs[i] << endl;
				cerr << "        " << proc << ": " << uCs[i] << endl;
				cerr << "    For inverse linear arrangement function " << i << ":" << endl;
				cerr << "    [" << T[i][0];
				for (size_t j = 1; j < n; ++j) {
					cerr << "," << T[i][j];
				}
				cerr << "]" << endl;
				cerr << "    Undirected graph:" << endl;
				cerr << uG << endl;
				cerr << "    Directed graph:" << endl;
				cerr << dG << endl;
				return err_type::test_execution;
			}
		}
	}

	string time_filename;
	if (fin >> time_filename) {
		ofstream fout;
		fout.open(time_filename.c_str());
		fout << "Total time: " << total_elapsed << " ms" << endl;
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
