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
#include <lal/generation/free_lab_trees.hpp>
#include <lal/generation/free_ulab_trees.hpp>
#include <lal/generation/rand_free_lab_trees.hpp>
#include <lal/generation/rand_free_ulab_trees.hpp>
#include <lal/generation/rand_rooted_lab_dir_trees.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

/* DATA FOR TESTING */

// expected second moment of degree over all labelled trees
inline rational exp_mmt_deg_2_lab_trees(uint64_t n) {
	rational p1 = 1;
	p1 -= rational(1,n);
	rational p2 = 5;
	p2 -= rational(6,n);
	return p1*p2;
}

// number of caterpillar trees of a given size
inline uint64_t num_caterpillar_trees(uint64_t n) {
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

// size of the vector with the number of unlabelled free trees
#define SIZE_UUF 37

inline bool is_caterpillar(const utree& t) {
	// number of vertices
	const uint64_t N = t.n_nodes();

	// number of internal vertices
	uint64_t n_internal = 0;
	// degree of the internal vertices
	vector<uint64_t> deg_internal(N, 0);
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
	uint64_t n1 = 0;
	for (node u = 0; u < N; ++u) {
		n1 += deg_internal[u] == 1;
	}
	return n1 == 2 or n1 == 0;
}

namespace exe_tests {

err_type exe_gen_trees(std::ifstream& fin) {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unique unlabelled free trees
	vector<integer> UUF(SIZE_UUF, 0);
	UUF[0] = 1;
	UUF[1] = 1;
	UUF[2] = 1;
	UUF[3] = 1;
	UUF[4] = 2;
	UUF[5] = 3;
	UUF[6] = 6;
	UUF[7] = 11;
	UUF[8] = 23;
	UUF[9] = 47;
	UUF[10] = 106;
	UUF[11] = 235;
	UUF[12] = 551;
	UUF[13] = 1301;
	UUF[14] = 3159;
	UUF[15] = 7741;
	UUF[16] = 19320;
	UUF[17] = 48629;
	UUF[18] = 123867;
	UUF[19] = 317955;
	UUF[20] = 823065;
	UUF[21] = 2144505;
	UUF[22] = 5623756;
	UUF[23] = 14828074;
	UUF[24] = 39299897;
	UUF[25] = integer("104636890");
	UUF[26] = integer("279793450");
	UUF[27] = integer("751065460");
	UUF[28] = integer("2023443032");
	UUF[29] = integer("5469566585");
	UUF[30] = integer("14830871802");
	UUF[31] = integer("40330829030");
	UUF[32] = integer("109972410221");
	UUF[33] = integer("300628862480");
	UUF[34] = integer("823779631721");
	UUF[35] = integer("2262366343746");
	UUF[36] = integer("6226306037178");

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

	utree T;
	string gen_type;

	uint64_t num_vertices;
	integer gen;

	free_lab_trees				AllFreeLabTreeGen;
	free_ulab_trees				AllFreeUlabTreeGen;
	rand_free_lab_trees			RandFreeLabTreeGen;
	rand_free_ulab_trees		RandFreeULabTreeGen;
	rand_rooted_lab_dir_trees	RandRootedLabTreeGen;

	while (fin >> gen_type >> num_vertices) {
		const integer n = integer_from_ui(num_vertices);

		if (gen_type == "exhaustive-labelled") {
			// expected second moment of degree
			const rational exp_mmtdeg2 = exp_mmt_deg_2_lab_trees(num_vertices);
			rational mmtdeg2 = 0;
			// number of generated trees
			gen = 0;

			// generate all trees
			AllFreeLabTreeGen.init(num_vertices);
			while (AllFreeLabTreeGen.has_next()) {
				AllFreeLabTreeGen.next();
				T = AllFreeLabTreeGen.get_tree();

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
			const integer total = (n^(n - 2));
			if (gen != total) {
				cerr << ERROR << endl;
				cerr << "    Exhaustive generation of free labelled trees" << endl;
				cerr << "    Amount of trees should be: " << total << endl;
				cerr << "    But generated: " << gen << endl;
				cerr << "    For a size of " << num_vertices << " vertices" << endl;
				return err_type::test_exe_error;
			}
		}
		else if (gen_type == "exhaustive-unlabelled") {
			// number of caterpillar trees
			uint64_t n_caterpillar = 0;
			// number of generated trees
			gen = 0;

			// generate all trees
			AllFreeUlabTreeGen.init(num_vertices);
			while (AllFreeUlabTreeGen.has_next()) {
				AllFreeUlabTreeGen.next();
				T = AllFreeUlabTreeGen.get_tree();

				// compute 'statistics'
				bool is_cat = is_caterpillar(T);
				n_caterpillar += is_cat;
				gen += 1;
			}

			// check the number of caterpillar trees is correct
			const uint64_t n_cat = num_caterpillar_trees(num_vertices);
			if (n_cat != n_caterpillar) {
				cerr << ERROR << endl;
				cerr << "    Number of caterpillar trees detected does not agree with the formula." << endl;
				cerr << "    Number of vertices: " << num_vertices << endl;
				cerr << "    Formula:  " << n_cat << endl;
				cerr << "    Detected: " << n_caterpillar << endl;
				return err_type::test_exe_error;
			}

			// make sure that the amount of trees generate coincides
			// with the series from the OEIS
			if (num_vertices < SIZE_UUF and gen != UUF[num_vertices]) {
				cerr << ERROR << endl;
				cerr << "    Exhaustive generation of free unlabelled trees" << endl;
				cerr << "    Amount of trees should be: " << UUF[num_vertices] << endl;
				cerr << "    But generated: " << gen << endl;
				cerr << "    For a size of " << num_vertices << " vertices" << endl;
				return err_type::test_exe_error;
			}
		}
		else if (gen_type == "random-labelled") {
			RandFreeLabTreeGen.init(num_vertices, 100);
			for (int i = 0; i < 10000; ++i) {
				T = RandFreeLabTreeGen.make_rand_tree();
			}
		}
		else if (gen_type == "random-unlabelled") {
			RandFreeULabTreeGen.init(num_vertices, 100);
			for (int i = 0; i < 10000; ++i) {
				T = RandFreeULabTreeGen.make_rand_tree();
			}
		}
		else if (gen_type == "random-rooted-labelled-trees") {
			RandRootedLabTreeGen.init(num_vertices, 100);
			for (int i = 0; i < 10000; ++i) {
				T = RandFreeULabTreeGen.make_rand_tree();
			}
		}
	}

	for (size_t i = 0; i < SIZE_UUF; ++i) {
		UUF[i].clear();
	}
	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
