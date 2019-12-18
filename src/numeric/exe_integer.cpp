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
#include <fstream>
using namespace std;

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

namespace exe_tests {

err_type exe_numeric_integer(ifstream& fin) {
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
	 *
	 */

	// inputs
	integer L1, R2;
	L1.init();
	R2.init();

	int32_t l = 0;
	uint32_t r = 0;
	string op;
	size_t line = 4;

	// results
	integer r1, r2;
	r1.init();
	r2.init();

	while (fin >> l >> op >> r) {
		L1.set_si(l);

		if (op == "+") {
			R2.set_ui(r);

			r1 = L1 + r;
			r2 = L1 + R2;
		}
		else if (op == "-") {
			R2.set_ui(r);
			r1 = L1 - r;
			r2 = L1 - R2;
		}
		else if (op == "*") {
			R2.set_ui(r);
			r1 = L1 * r;
			r2 = L1 * R2;
		}
		else if (op == "/") {
			R2.set_ui(r);
			r1 = L1 / r;
			r2 = L1 / R2;
		}
		else if (op == "^") {
			R2.set_ui(r);
			r1 = L1 ^ r;
			r2 = L1 ^ R2;
		}
		else if (op == "%") {
			R2.set_ui(r);
			uint64_t resmod = L1 % r;
			r1.set_ui(resmod);
			r2 = L1 % R2;
		}
		else if (op == "++") {
			R2.set_ui(r);
			r1 = L1; r1 += r;
			r2 = L1; r2 += R2;
		}
		else if (op == "--") {
			R2.set_ui(r);
			r1 = L1; r1 -= r;
			r2 = L1; r2 -= R2;
		}
		else if (op == "**") {
			R2.set_ui(r);
			r1 = L1; r1 *= r;
			r2 = L1; r2 *= R2;
		}
		else if (op == "//") {
			R2.set_ui(r);
			r1 = L1; r1 /= r;
			r2 = L1; r2 /= R2;
		}
		else if (op == "^^") {
			R2.set_ui(r);
			r1 = L1; r1 ^= r;
			r2 = L1; r2 ^= R2;
		}

		if (r1 != r2) {
			cerr << ERROR << endl;
			cerr << "    Results do not agree." << endl;
			cerr << "    In line: " << line << endl;
			cerr << "        r1: " << r1.to_string() << " = ";
			cerr << L1.to_string() << " " << op << " " << r << endl;
			cerr << "        r2: " << r2.to_string() << " = ";
			cerr << L1.to_string() << " " << op
				 << " " << R2.to_string() << endl;
			return err_type::test_exe_error;
		}

		cout << r1.to_string() << endl;
		++line;
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
