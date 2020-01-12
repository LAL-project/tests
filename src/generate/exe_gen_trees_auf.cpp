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
#include <lal/generation/all_ulab_free_trees.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

/*
 * ALL UNLABELLED FREE TREES
 *
 */

// number of caterpillar trees of a given size
inline uint64_t num_caterpillar_trees(uint32_t n) {
	if (n == 1) { return 1; }
	if (n == 2) { return 1; }
	if (n == 3) { return 1; }
	if (n == 4) { return 2; }
	integer n1 = 2;
	n1 ^= n - 4;
	integer n2 = 2;
	n2 ^= ((n - 4)/2);
	n1 += n2;
	return n1.to_uint();
}

inline bool is_caterpillar(const utree& t) {
	// number of vertices
	const uint32_t N = t.n_nodes();

	// number of internal vertices
	uint32_t n_internal = 0;
	// degree of the internal vertices
	vector<uint32_t> deg_internal(N, 0);
	for (node u = 0; u < N; ++u) {
		if (t.degree(u) > 1) {
			deg_internal[u] = t.degree(u);
			++n_internal;
		}
	}

	// reduce the degree of the internal vertices
	// as many times as leaves are connected to them
	for (node u = 0; u < N; ++u) {
		if (t.degree(u) == 1) {
			deg_internal[ t.get_neighbours(u)[0] ] -= 1;
		}
	}

	// If we are left with 2, or 0, vertices with degree 1,
	// it means that after removing the leaves of the tree
	// all vertices are internal (degree 2), i.e., they are
	// part of a linear tree. Needless to say that these
	// two vertices of degree 1 are the endpoints of the
	// linear tree.
	uint32_t n1 = 0;
	for (node u = 0; u < N; ++u) {
		n1 += deg_internal[u] == 1;
	}
	return n1 == 2 or n1 == 0;
}

// size of the vector with the number of unlabelled free trees
#define SIZE_UFT 37

namespace exe_tests {

err_type exe_gen_trees_auf(std::ifstream& fin) {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unlabelled free trees
	vector<integer> UFT(SIZE_UFT, 0);
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

	all_ulab_free_trees TreeGen;
	utree T;

	while (fin >> n) {
		// number of caterpillar trees
		uint64_t n_caterpillar = 0;
		// number of generated trees
		gen = 0;

		// generate all trees
		TreeGen.init(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			T = TreeGen.get_tree();

			// compute 'statistics'
			n_caterpillar += is_caterpillar(T);
			gen += 1;
		}

		// check the number of caterpillar trees is correct
		const uint64_t n_cat = num_caterpillar_trees(n);
		if (n_cat != n_caterpillar) {
			cerr << ERROR << endl;
			cerr << "    Number of caterpillar trees detected does not agree with the formula." << endl;
			cerr << "    Number of vertices: " << n << endl;
			cerr << "    Formula:  " << n_cat << endl;
			cerr << "    Detected: " << n_caterpillar << endl;
			return err_type::test_exe_error;
		}

		// make sure that the amount of trees generate coincides
		// with the series from the OEIS
		if (n < SIZE_UFT and gen != UFT[n]) {
			cerr << ERROR << endl;
			cerr << "    Exhaustive generation of free unlabelled trees" << endl;
			cerr << "    Amount of trees should be: " << UFT[n] << endl;
			cerr << "    But generated: " << gen << endl;
			cerr << "    For a size of " << n << " vertices" << endl;
			return err_type::test_exe_error;
		}
	}

	for (size_t i = 0; i < SIZE_UFT; ++i) {
		UFT[i].clear();
	}
	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
