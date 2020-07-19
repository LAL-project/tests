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
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/properties/Q.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"
#include "time.hpp"

namespace exe_tests {

void output_ExpVar_D_formula(const ugraph& g) {
	const rational Vr = variance_D_rational(g);
	const rational E1r = expectation_D_rational(g);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << "\t" << endl;
}

err_type exe_properties_ExpVar_D(const input_list& inputs, ifstream& fin) {
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
	for (size_t i = 0; i < inputs.size(); ++i) {
		err_type r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, G);
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
