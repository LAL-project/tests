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
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// C++ includes
#include <iostream>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/Q.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"
#include "../test_utils.hpp"
#include "../time.hpp"
using namespace test_utils;

namespace exe_tests {

void output_ExpVar_D_formula(const ugraph& g) {
	rational Vr = variance_D_rational(g);
	rational E1r = expectation_D_first_rational(g);
	rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << "\t" << endl;
}

err_type exe_properties_ExpVar_D(ifstream& fin) {
	cout.setf(ios::fixed);
	cout.precision(4);

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n;
	fin >> n;
	vector<string> graph_name(n), graph_format(n);
	for (size_t i = 0; i < n; ++i) {
		fin >> graph_name[i] >> graph_format[i];
	}

	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string proc;
	fin >> proc;

	if (proc != "formula") {
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Received '" << proc << "'." << endl;
		return err_type::test_format_error;
	}

	// read the graph type so that we can call
	// the appropriate function for this graph
	// type's variance.
	string graph_type;
	fin >> graph_type;

	ugraph G;
	for (size_t i = 0; i < n; ++i) {
		err_type r = io_wrapper::read_graph(graph_name[i], graph_format[i], G);
		if (r != err_type::no_error) {
			return r;
		}
		if (proc == "formula") {
			output_ExpVar_D_formula(G);
		}
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
