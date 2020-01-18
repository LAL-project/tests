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

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

namespace exe_tests {

err_type exe_numeric_rational_rational(ifstream& fin) {
	string field;
	size_t n;

	fin >> field;
	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}
	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	/* Make the number to the left an integer: L1
	 * -> (1) 'op' to it the number to the right as int32_t
	 * -> (2) 'op' to it the number to the right as integer
	 */

	// inputs
	rational L1, R1;
	L1.init();
	R1.init();

	string l, op, r;
	size_t line = 4;

	// results
	rational r1;

	while (fin >> l >> op >> r) {
		L1.set_str(l);
		R1.set_str(r);

		if (op == "+") {
			r1 = L1 + R1;
		}
		else if (op == "-") {
			r1 = L1 - R1;
		}
		else if (op == "*") {
			r1 = L1 * R1;
		}
		else if (op == "/") {
			r1 = L1 / R1;
		}
		else if (op == "++") {
			r1 = L1; r1 += R1;
		}
		else if (op == "--") {
			r1 = L1; r1 -= R1;
		}
		else if (op == "**") {
			r1 = L1; r1 *= R1;
		}
		else if (op == "//") {
			r1 = L1; r1 /= R1;
		}

		cout << r1 << endl;
		++line;
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
