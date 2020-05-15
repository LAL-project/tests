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
#include <lal/generate/all_lab_free_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
#include <lal/properties/degrees.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "definitions.hpp"
#include "generate/tree_validity_check.hpp"

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

	// --- do the tests

	uint32_t n;
	integer gen;

	all_lab_free_trees TreeGen;

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
			const ftree T = TreeGen.get_tree();
			const ftree_check err = test_validity_tree(n, T);
			if (err != ftree_check::correct) {
				cerr << ERROR << endl;
				cerr << "    Tree of index " << gen << " is not correct." << endl;
				cerr << "    Error: " << ftree_check_to_string(err) << endl;
				cerr << T << endl;
				return err_type::test_exe_error;
			}

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
		const integer total = (n == 1 ? 1 : (nn^(nn - 2)));
		if (gen != total) {
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
