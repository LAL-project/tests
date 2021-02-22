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
#include <lal/iterators/Q_iterator.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/properties/Q.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/linarr/algorithms_C.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"
#include "time.hpp"

namespace exe_tests {

err_type exe_linarr_C(const input_list& inputs, ifstream& fin) {
	set<string> allowed_procs({"Q", "brute_force", "dyn_prog", "ladder", "stack_based"});

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
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		return err_type::test_format;
	}

	double total_elapsed = 0.0;

	// linear arrangement
	const uint32_t n = uG.n_nodes();
	vector<node> T(n);
	linear_arrangement pi(n);

	// amount of linear arrangements
	size_t n_linarrs;
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (uint32_t u = 0; u < uG.n_nodes(); ++u) {
			fin >> T[u];
			pi[ T[u] ] = u;
		}

		const uint32_t uCbf = n_crossings(uG, pi, algorithms_C::brute_force);
		const uint32_t dCbf = n_crossings(dG, pi, algorithms_C::brute_force);
		if (uCbf != dCbf) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings calculated by bruteforce do not coincide." << endl;
			cerr << "    Comparing results for directed and undirected graphs." << endl;
			cerr << "    uCbf= " << uCbf << endl;
			cerr << "    dCbf= " << dCbf << endl;
			cerr << "    For inverse linear arrangement function " << i << ":" << endl;
			cerr << "    [" << T[0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[j];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}

		uint32_t uC = 0, dC = 0;
		const auto choose_algo =
		[](const string& name) {
			if (name == "dyn_prog") { return algorithms_C::dynamic_programming; }
			if (name == "ladder") { return algorithms_C::ladder; }
			if (name == "stack_based") { return algorithms_C::stack_based; }
			return algorithms_C::brute_force;
		}(proc);
		if (choose_algo == algorithms_C::brute_force) {
			cerr << ERROR << endl;
			cerr << "    Unhandled proc '" << proc << "'." << endl;
			return err_type::not_implemented;
		}

		const auto begin = timing::now();
		uC = n_crossings(uG, pi, choose_algo);
		dC = n_crossings(dG, pi, choose_algo);
		const auto end = timing::now();
		total_elapsed += timing::elapsed_milliseconds(begin, end);

		if (uC != dC) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings for the directed graph does not" << endl;
			cerr << "    coincide with the undirected graph." << endl;
			cerr << "    uC= " << uC << endl;
			cerr << "    dC= " << dC << endl;
			cerr << "    For inverse linear arrangement function " << i << ":" << endl;
			cerr << "    [" << T[0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[j];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}
		if (uC != uCbf) {
			cerr << ERROR << endl;
			cerr << "    Number of crossings obtained with the algorithm does not" << endl;
			cerr << "    coincide with the number of crossings obtained by brute force." << endl;
			cerr << "        brute force: " << uCbf << endl;
			cerr << "        " << proc << ": " << uC << endl;
			cerr << "    For inverse linear arrangement function " << i << ":" << endl;
			cerr << "    [" << T[0];
			for (size_t j = 1; j < n; ++j) {
				cerr << "," << T[j];
			}
			cerr << "]" << endl;
			cerr << "    Undirected graph:" << endl;
			cerr << uG << endl;
			cerr << "    Directed graph:" << endl;
			cerr << dG << endl;
			return err_type::test_execution;
		}
	}

	string time_filename;
	if (fin >> time_filename) {
		ofstream fout;
		fout.open(time_filename.c_str());
		fout << "Total time: " << total_elapsed << " ms" << endl;
		fout.close();
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
