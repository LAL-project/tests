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
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// custom includes
#include "../definitions.hpp"

namespace exe_tests {

err_type exe_gen_trees(std::ifstream& fin) {
	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n;
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    Expected no inputs at all." << endl;
		cerr << "    Instead, '" << n << "' were found." << endl;
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

#define SIZE_UUF 37

	// from: http://oeis.org/A000055/list
	// amount of unique unlabelled free trees
	integer *UUF = static_cast<integer *>(malloc(SIZE_UUF*sizeof(integer)));
	for (int i = 0; i < SIZE_UUF; ++i) {
		new (&(UUF[i])) integer();
		UUF[i].init();
	}
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

	/* do the tests */

	ugraph T;
	string gen_type;

	uint32_t num_nodes;
	integer gen;

	free_lab_trees				AllFreeLabTreeGen;
	free_ulab_trees				AllFreeUlabTreeGen;
	rand_free_lab_trees			RandFreeLabTreeGen;
	rand_free_ulab_trees		RandFreeULabTreeGen;
	rand_rooted_lab_dir_trees	RandRootedLabTreeGen;

	while (fin >> gen_type >> num_nodes) {
		if (gen_type == "exhaustive-labelled") {
			gen = 0;
			AllFreeLabTreeGen.init(num_nodes);
			while (AllFreeLabTreeGen.has_next()) {
				AllFreeLabTreeGen.next();
				T = AllFreeLabTreeGen.get_tree();
				gen += 1;
			}

			integer n = num_nodes;
			// Prüfer's formula
			integer total = (n^(n - 2));

			if (gen != total) {
				cerr << ERROR << endl;
				cerr << "    Exhaustive generation of free labelled trees" << endl;
				cerr << "    Amount of trees should be: " << total << endl;
				cerr << "    But generated: " << gen << endl;
				cerr << "    For a size of " << num_nodes << " vertices" << endl;
				return err_type::test_exe_error;
			}
		}
		else if (gen_type == "exhaustive-unlabelled") {
			gen = 0;
			AllFreeUlabTreeGen.init(num_nodes);
			while (AllFreeUlabTreeGen.has_next()) {
				AllFreeUlabTreeGen.next();
				T = AllFreeUlabTreeGen.get_tree();
				gen += 1;
			}

			if (num_nodes < SIZE_UUF and gen != UUF[num_nodes]) {
				cerr << ERROR << endl;
				cerr << "    Exhaustive generation of free unlabelled trees" << endl;
				cerr << "    Amount of trees should be: " << UUF[num_nodes] << endl;
				cerr << "    But generated: " << gen << endl;
				cerr << "    For a size of " << num_nodes << " vertices" << endl;
				return err_type::test_exe_error;
			}
		}
		else if (gen_type == "random-labelled") {
			RandFreeLabTreeGen.init(num_nodes, 100);
			for (uint32_t i = 0; i < 10000; ++i) {
				T = RandFreeLabTreeGen.make_rand_tree();
			}
		}
		else if (gen_type == "random-unlabelled") {
			RandFreeULabTreeGen.init(num_nodes, 100);
			for (uint32_t i = 0; i < 10000; ++i) {
				T = RandFreeULabTreeGen.make_rand_tree();
			}
		}
		else if (gen_type == "random-rooted-labelled-trees") {
			RandRootedLabTreeGen.init(num_nodes, 100);
			for (uint32_t i = 0; i < 10000; ++i) {
				T = RandFreeULabTreeGen.make_rand_tree();
			}
		}
	}

	for (int i = 0; i < SIZE_UUF; ++i) {
		UUF[i].clear();
	}
	free(UUF);

	cout << "Test finished without apparent errors." << endl;
	return err_type::no_error;
}

} // -- namespace exe_tests
