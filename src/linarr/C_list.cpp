/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
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

namespace exe_tests {

err_type exe_linarr_C_list(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_procs(
	{"brute_force", "dyn_prog", "ladder", "stack_based"}
	);

	if (inputs.size() != 1) {
		cerr << ERROR << endl;
		cerr << "    Only one input file si allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	ugraph g;
	{
	const string graph_name = inputs[0].first;
	const string graph_format = inputs[0].second;
	err_type r = io_wrapper::read_graph(graph_name, graph_format, g);
	if (r != err_type::no_error) {
		return r;
	}
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
		return err_type::test_format_error;
	}

	timing::time_point begin, end;
	double total_elapsed = 0.0;

	const uint32_t n = g.n_nodes();

	// amount of linear arrangements
	uint32_t n_linarrs;
	fin >> n_linarrs;

	// linear arrangements
	vector<vector<node> > T(n_linarrs, vector<node>(g.n_nodes()));
	vector<linear_arrangement > pis(n_linarrs,  linear_arrangement(g.n_nodes()));

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (uint32_t u = 0; u < g.n_nodes(); ++u) {
			fin >> T[i][u];
			pis[i][ T[i][u] ] = u;
		}
	}

	const vector<uint32_t> Cbfs = linarr::n_crossings_list(g, T, algorithms_C::brute_force);

	// compute all C
	vector<uint32_t> Cs;
	if (proc == "dyn_prog") {
		begin = timing::now();
		Cs = n_crossings_list(g, T, algorithms_C::dynamic_programming);
		end = timing::now();
		total_elapsed += timing::elapsed_milliseconds(begin, end);
	}
	else if (proc == "ladder") {
		begin = timing::now();
		Cs = n_crossings_list(g, T, algorithms_C::ladder);
		end = timing::now();
		total_elapsed += timing::elapsed_milliseconds(begin, end);
	}
	else if (proc == "stack_based") {
		begin = timing::now();
		Cs = n_crossings_list(g, T, algorithms_C::stack_based);
		end = timing::now();
		total_elapsed += timing::elapsed_milliseconds(begin, end);
	}

	for (uint32_t i = 0; i < n_linarrs; ++i) {
		if (Cbfs[i] != Cs[i]) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings do not coincide" << endl;
			cerr << "        brute force: " << Cbfs[i] << endl;
			cerr << "        " << proc << ": " << Cs[i] << endl;
			cerr << "    For linear arrangement " << i << ":" << endl;
			cerr << "    [" << T[i][0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[i][j];
			}
			cerr << "]" << endl;
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
