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

// C includes

// C++ includes
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/1level.hpp>
#include <lal/linarr/2level.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace linarr;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"

namespace exe_tests {

err_type exe_linarr_klevel(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_levels(
	{"1", "2"}
	);

	const set<string> allowed_procs(
	{"MDD"}
	);

	/* FUNCTIONS */

	string level, proc;
	fin >> level >> proc;
	if (allowed_levels.find(level) == allowed_levels.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong value for level." << endl;
		cerr << "    Level '" << level << "' was found." << endl;
		cerr << "    Valid values:" << endl;
		for (const string& l : allowed_levels) {
		cerr << "    - " << l << endl;
		}
		return err_type::test_format;
	}
	if (allowed_procs.find(level) == allowed_procs.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		cerr << "    Valid values:" << endl;
		for (const string& p : allowed_procs) {
		cerr << "    - " << p << endl;
		}
		return err_type::test_format;
	}

	auto MDD_F = [level](const vector<undirected_graph>& Gs, const vector<linear_arrangement>& pis) {
		if (level == "1") { return linarr::mean_dependency_distance_1level_rational(Gs, pis); }
		if (level == "2") { return linarr::mean_dependency_distance_2level_rational(Gs, pis); }

		// return invalid value
		cerr << ERROR << endl;
		return rational(-1);
	};

	/* TEST's CODE */

	if (inputs.size() == 1) {
		cerr << ERROR << endl;
		cerr << "    Expected at least one input." << endl;
		cerr << "    Instead, none were given." << endl;
		return err_type::test_format;
	}

	const size_t n_inputs = inputs.size();
	vector<undirected_graph> Gs(n_inputs);
	for (size_t i = 0; i < n_inputs; ++i) {
		const string graph_name = inputs[i].first;
		const string graph_format = inputs[i].second;

		err_type r = io_wrapper::read_graph(graph_name, graph_format, Gs[i]);
		if (r != err_type::no_error) {
			return r;
		}
	}

	// linear arrangement
	vector<vector<node>> Ts;
	vector<linear_arrangement> pis;

	// amount of linear arrangements
	size_t n_linarrs;
	fin >> n_linarrs;

	if (not (n_linarrs == 0 or n_linarrs == Gs.size())) {
		cerr << ERROR << endl;
		cerr << "    The number of linear arrangements does not equal 0" << endl;
		cerr << "    and it does not equal the amount of graphs given as input." << endl;
		cerr << "    Amount of graphs given as input: " << Gs.size() << endl;
		cerr << "    Number of linear arrangements: " << n_linarrs << endl;
		return err_type::test_format;
	}

	if (n_linarrs != 0) {
		Ts = vector<vector<position>>(n_linarrs);
		pis = vector<linear_arrangement>(n_linarrs);

		for (size_t i = 0; i < n_linarrs; ++i) {
			const uint32_t Ni = Gs[i].n_nodes();

			Ts[i] = vector<position>(Ni);
			pis[i] = linear_arrangement(Ni);

			// read all linear arrangement
			for (node u = 0; u < Ni; ++u) {
				fin >> Ts[i][u];
				pis[i][ Ts[i][u] ] = u;
			}
		}
	}

	if (proc == "MDD") {
		const rational MDD = MDD_F(Gs, pis);
		cout << level << "-level MDD= " << MDD << endl;
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
