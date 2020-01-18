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
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/generation/all_ulab_free_trees.hpp>
#include <lal/properties/mhd.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace generate;
using namespace graphs;
using namespace numeric;
using namespace properties;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"

namespace exe_tests {

err_type exe_properties_MHD_All_trees(ifstream& fin) {
	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	uint32_t n;
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    Expected no inputs at all." << endl;
		cerr << "    Instead, '" << n << "' were found." << endl;
		return err_type::test_format_error;
	}

	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	all_ulab_free_trees TreeGen;
	while (fin >> n) {
		cout << "----------------------------------------" << endl;
		cout << "n= " << n << endl;

		TreeGen.init(n);
		int i = 0;
		while (TreeGen.has_next()) {
			TreeGen.next();
			utree t = TreeGen.get_tree();

			cout << i << ")" << endl;

			// for each node, make a rooted tree at that node
			for (node r = 0; r < t.n_nodes(); ++r) {
				urtree R(t, r);
				rational mhd = properties::MHD_rational(R);
				cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << endl;
			}

			++i;
		}
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
