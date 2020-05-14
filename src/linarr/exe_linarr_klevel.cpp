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

// C includes

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/numeric/rational.hpp>
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

err_type exe_linarr_klevel(const string& level, const string& what, ifstream& fin) {

	/* FUNCTIONS */

	auto MDD_F = [level](const vector<ugraph>& Gs, const vector<linearrgmnt>& pis) {
		if (level == "1") { return linarr::MDD_1level_rational(Gs, pis); }
		if (level == "2") { return linarr::MDD_2level_rational(Gs, pis); }

		// return invalid value
		cerr << ERROR << endl;
		return rational(-1);
	};

	/* TEST's CODE */

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
	if (n_inputs == 0) {
		cerr << ERROR << endl;
		cerr << "    Expected at least one input." << endl;
		cerr << "    Instead, none were given." << endl;
		return err_type::test_format_error;
	}

	vector<ugraph> Gs(n_inputs);
	for (size_t i = 0; i < n_inputs; ++i) {
		string graph_name;
		string graph_format;
		fin >> graph_name >> graph_format;

		err_type r = io_wrapper::read_graph(graph_name, graph_format, Gs[i]);
		if (r != err_type::no_error) {
			return r;
		}
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
	vector<vector<node> > Ts;
	vector<linearrgmnt> pis;

	// amount of linear arrangements
	size_t n_linarrs;
	fin >> n_linarrs;

	if (not (n_linarrs == 0 or n_linarrs == Gs.size())) {
		cerr << ERROR << endl;
		cerr << "    The number of linear arrangements does not equal 0" << endl;
		cerr << "    and it does not equal the amount of graphs given as input." << endl;
		cerr << "    Amount of graphs given as input: " << Gs.size() << endl;
		cerr << "    Number of linear arrangements: " << n_linarrs << endl;
		return err_type::test_format_error;
	}

	if (n_linarrs != 0) {
		Ts = vector<vector<node> >(n_linarrs);
		pis = vector<linearrgmnt>(n_linarrs);

		for (size_t i = 0; i < n_linarrs; ++i) {
			const uint32_t Ni = Gs[i].n_nodes();

			Ts[i] = vector<node>(Ni);
			pis[i] = linearrgmnt(Ni);

			// read all linear arrangement
			for (node u = 0; u < Ni; ++u) {
				fin >> Ts[i][u];
				pis[i][ Ts[i][u] ] = u;
			}
		}
	}

	if (what == "MDD") {
		rational MDD = MDD_F(Gs, pis);
		cout << level << "-level MDD= " << MDD << endl;
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
