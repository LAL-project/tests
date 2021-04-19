/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/arrgmnt_validity_check.hpp"
#include "common/std_utils.hpp"
#include "common/tree_validity_check.hpp"

// generate includes
#include "generate/test_exhaustive_enumeration.hpp"

// number of caterpillar trees of a given size
inline integer num_caterpillar_trees(uint32_t n) {
	if (n == 1) { return 1; }
	if (n == 2) { return 1; }
	if (n == 3) { return 1; }
	if (n == 4) { return 2; }
	integer n1 = 2;
	n1 ^= n - 4;
	integer n2 = 2;
	n2 ^= ((n - 4)/2);
	n1 += n2;
	return n1;
}

namespace tests {
namespace generate {

namespace auf {
struct extra_params {
	vector<integer> UFT;
	uint32_t SIZE_UFT;
};

err_type test_for_n(
	uint32_t n, all_ulab_free_trees& TreeGen, const extra_params& params
)
{
	const auto& UFT = params.UFT;
	const auto& SIZE_UFT = params.SIZE_UFT;

	// number of caterpillar trees
	integer n_caterpillar = 0;
	// number of generated trees
	integer gen = 0;

	while (TreeGen.has_next()) {
		TreeGen.next();
		const free_tree T = TreeGen.get_tree();

		const ftree_check err = test_validity_tree(n, T);
		if (err != ftree_check::correct) {
			cerr << ERROR << endl;
			cerr << "    Tree of index " << gen << " is not correct." << endl;
			cerr << "    Error: " << ftree_check_to_string(err) << endl;
			cerr << T << endl;
			return err_type::test_execution;
		}

		// compute 'statistics'
		n_caterpillar += T.is_of_tree_type(tree_type::caterpillar);
		gen += 1;
	}

	// check the number of caterpillar trees is correct
	const integer n_cat = num_caterpillar_trees(n);
	if (n_cat != n_caterpillar) {
		cerr << ERROR << endl;
		cerr << "    Number of caterpillar trees detected does not agree with the formula." << endl;
		cerr << "    Number of vertices: " << n << endl;
		cerr << "    Formula:  " << n_cat << endl;
		cerr << "    Detected: " << n_caterpillar << endl;
		return err_type::test_execution;
	}

	// make sure that the amount of trees generate coincides
	// with the series from the OEIS
	if (n < SIZE_UFT and gen != UFT[n]) {
		cerr << ERROR << endl;
		cerr << "    Exhaustive generation of unlabelled free trees" << endl;
		cerr << "    Amount of trees should be: " << UFT[n] << endl;
		cerr << "    But generated: " << gen << endl;
		cerr << "    For a size of " << n << " vertices" << endl;
		return err_type::test_execution;
	}
	return err_type::no_error;
}
} // -- namespace auf

err_type exe_gen_trees_auf(const input_list& inputs, ifstream& fin) {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unlabelled free trees
	auf::extra_params params;
	// size of the vector with the number of unlabelled free trees
	params.SIZE_UFT = 37;

	auto& UFT = params.UFT;
	UFT = vector<integer>(params.SIZE_UFT, 0);
	UFT[0] = 1;
	UFT[1] = 1;
	UFT[2] = 1;
	UFT[3] = 1;
	UFT[4] = 2;
	UFT[5] = 3;
	UFT[6] = 6;
	UFT[7] = 11;
	UFT[8] = 23;
	UFT[9] = 47;
	UFT[10] = 106;
	UFT[11] = 235;
	UFT[12] = 551;
	UFT[13] = 1301;
	UFT[14] = 3159;
	UFT[15] = 7741;
	UFT[16] = 19320;
	UFT[17] = 48629;
	UFT[18] = 123867;
	UFT[19] = 317955;
	UFT[20] = 823065;
	UFT[21] = 2144505;
	UFT[22] = 5623756;
	UFT[23] = 14828074;
	UFT[24] = 39299897;
	UFT[25] = integer("104636890");
	UFT[26] = integer("279793450");
	UFT[27] = integer("751065460");
	UFT[28] = integer("2023443032");
	UFT[29] = integer("5469566585");
	UFT[30] = integer("14830871802");
	UFT[31] = integer("40330829030");
	UFT[32] = integer("109972410221");
	UFT[33] = integer("300628862480");
	UFT[34] = integer("823779631721");
	UFT[35] = integer("2262366343746");
	UFT[36] = integer("6226306037178");

	// -------------------------------------------------------------------------

	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	// --- do the tests

	uint32_t n;
	while (fin >> n) {
		const auto err =
			test_exhaustive_enumeration_of_trees<all_ulab_free_trees>
			(n, auf::test_for_n, params);

		if (err != err_type::no_error) { return err; }
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
