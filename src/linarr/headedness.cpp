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
#include <lal/graphs/dgraph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/linarr/headedness.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"
#include "time.hpp"

namespace exe_tests {

err_type exe_linarr_headedness(ifstream& fin) {
	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_inputs;
	fin >> n_inputs;
	if (n_inputs != 1) {
		cerr << ERROR << endl;
		cerr << "    Expected only one input." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
		return err_type::test_format_error;
	}

	string graph_name;
	string graph_format;
	fin >> graph_name >> graph_format;

	dgraph G;
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

	// linear arrangement
	const uint32_t n = G.n_nodes();
	vector<node> T(n);
	linearrgmnt pi(n);

	// amount of linear arrangements
	size_t n_linarrs;
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (node u = 0; u < G.n_nodes(); ++u) {
			fin >> T[u];
			pi[ T[u] ] = u;
		}

		// output
		cout << "[" << T[0];
		for (node j = 1; j < G.n_nodes(); ++j) {
			cout << ", " << T[j];
		}
		cout << "]: ";

		rational h = linarr::headedness_rational(G, pi);
		cout << h << endl;
	}

	return err_type::no_error;
}

} // -- namespace exe_tests