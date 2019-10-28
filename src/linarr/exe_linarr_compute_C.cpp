/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// lal includes
#include <lal/linarr/C.hpp>
#include <lal/ugraph.hpp>
#include <lal/properties/Q.hpp>
using namespace lal;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"
#include "../time.hpp"

namespace exe_tests {

err_type exe_linarr_compute_C(ifstream& fin) {
	set<string> allowed_procs({"Q", "brute_force", "dyn_prog", "ladder", "stack_based"});

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_linarrs;
	fin >> n_linarrs;
	if (n_linarrs != 1) {
		cerr << ERROR << endl;
		cerr << "    Expected only one input." << endl;
		cerr << "    Instead, '" << n_linarrs << "' were found." << endl;
		return err_type::test_format_error;
	}

	string graph_name;
	string graph_format;
	fin >> graph_name >> graph_format;

	ugraph G;
	err_type r = io_wrapper::read_graph(graph_name, graph_format, G);
	if (r != err_type::no_error) {
		return r;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		return err_type::test_format_error;
	}

	timing::time_point begin, end;
	double total_elapsed = 0.0;

	// linear arrangement
	uint32_t n = G.n_nodes();
	vector<node> arr(n);

	vector<edge_pair> Q;
	if (proc == "Q") {
		properties::enumerate_Q(G, Q);
	}

	// amount of linear arrangements
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (uint32_t u = 0; u < G.n_nodes(); ++u) {
			fin >> arr[u];
		}

		uint32_t Cbf = linarr::n_crossings_brute_force(G, arr);

		uint32_t C = 0;
		if (proc == "Q") {
			begin = timing::now();
			C = linarr::n_crossings_Q(Q, arr);
			end = timing::now();
			total_elapsed += timing::elapsed_milliseconds(begin, end);
		}
		else if (proc == "dyn_prog") {
			begin = timing::now();
			C = linarr::n_crossings_dyn_prog(G, arr);
			end = timing::now();
			total_elapsed += timing::elapsed_milliseconds(begin, end);
		}
		else if (proc == "ladder") {
			begin = timing::now();
			C = linarr::n_crossings_ladder(G, arr);
			end = timing::now();
			total_elapsed += timing::elapsed_milliseconds(begin, end);
		}
		else if (proc == "stack_based") {
			begin = timing::now();
			C = linarr::n_crossings_stack_based(G, arr);
			end = timing::now();
			total_elapsed += timing::elapsed_milliseconds(begin, end);
		}

		if (Cbf != C) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings do not coincide" << endl;
			cerr << "        brute force: " << Cbf << endl;
			cerr << "        " << proc << ": " << C << endl;
			cerr << "    For linear arrangement " << i << ":" << endl;
			cerr << "    [" << arr[0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << arr[j];
			}
			cerr << "]" << endl;
			return err_type::test_exe_error;
		}
	}

	string time_filename;
	if (fin >> time_filename) {
		ofstream fout;
		fout.open(time_filename.c_str());
		fout << "Total time: " << total_elapsed << " ms" << endl;
	}

	cout << "Test finished without apparent errors." << endl;
	return err_type::no_error;
}

} // -- namespace exe_tests
