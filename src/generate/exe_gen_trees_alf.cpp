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
#include <lal/generation/all_lab_free_trees.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
#include <lal/properties/degrees.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

/*
 * ALL LABELLED FREE TREES
 *
 */

// expected second moment of degree over all labelled trees
inline rational exp_mmt_deg_2_lab_trees(uint32_t n) {
	rational p1 = 1;
	p1 -= rational(1,n);
	rational p2 = 5;
	p2 -= rational(6,n);
	return p1*p2;
}

// size of the vector with the number of unlabelled free trees
#define SIZE_LFT 31

namespace exe_tests {

err_type exe_gen_trees_alf(std::ifstream& fin) {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unlabelled free trees
	vector<integer> LFT(SIZE_LFT, 0);
	LFT[0] = 0;
	LFT[1] = 1;
	LFT[2] = 1;
	LFT[3] = 2;
	LFT[4] = 4;
	LFT[5] = 9;
	LFT[6] = 20;
	LFT[7] = 48;
	LFT[8] = 115;
	LFT[9] = 286;
	LFT[10] = 719;
	LFT[11] = 1842;
	LFT[12] = 4766;
	LFT[13] = 12486;
	LFT[14] = 32973;
	LFT[15] = 87811;
	LFT[16] = 235381;
	LFT[17] = 634847;
	LFT[18] = 1721159;
	LFT[19] = 4688676;
	LFT[20] = 12826228;
	LFT[21] = 35221832;
	LFT[22] = 97055181;
	LFT[23] = 268282855;
	LFT[24] = 743724984;
	LFT[25] = integer("2067174645");
	LFT[26] = integer("5759636510");
	LFT[27] = integer("16083734329");
	LFT[28] = integer("45007066269");
	LFT[29] = integer("126186554308");
	LFT[30] = integer("354426847597");

	// -------------------------------------------------------------------------

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
	if (n_inputs != 0) {
		cerr << ERROR << endl;
		cerr << "    Expected no inputs at all." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
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

	/* do the tests */

	uint32_t n;
	integer gen;

	all_lab_free_trees TreeGen;
	utree T;

	while (fin >> n) {
		const integer nn = integer_from_ui(n);

		// expected second moment of degree
		const rational exp_mmtdeg2 = exp_mmt_deg_2_lab_trees(n);
		rational mmtdeg2 = 0;
		// number of generated trees
		gen = 0;

		// generate all trees
		TreeGen.init(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			T = TreeGen.get_tree();

			// compute 'statistics'
			mmtdeg2 += properties::mmt_degree_rational(T, 2);
			gen += 1;
		}

		// check that the expected second moment of degree is correct
		mmtdeg2 /= gen;
		if (mmtdeg2 != exp_mmtdeg2) {
			cerr << ERROR << endl;
			cerr << "    Calculated 2nd moment of degree: " << mmtdeg2 << endl;
			cerr << "    Does not agree with the formula: " << exp_mmtdeg2 << endl;
			return err_type::test_exe_error;
		}

		// Prüfer's formula: make sure that the generator made
		// as many trees as n^(n - 2)
		const integer total = (nn^(nn - 2));
		if (gen != total or gen != LFT[n]) {
			cerr << ERROR << endl;
			cerr << "    Exhaustive generation of free labelled trees" << endl;
			cerr << "    Amount of trees should be: " << total << endl;
			cerr << "    But generated: " << gen << endl;
			cerr << "    For a size of " << n << " vertices" << endl;
			return err_type::test_exe_error;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
