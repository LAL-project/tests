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
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <set>
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/graphs/ftree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/utils/std_utils.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace generate;

// custom includes
#include "definitions.hpp"
#include "arrgmnt_validity_check.hpp"
#include "test_utils.hpp"

namespace exe_tests {

#define consume_beginning(F)												\
{																			\
	string field;															\
	F >> field;																\
	if (field != "INPUT") {													\
		cerr << ERROR << endl;												\
		cerr << "    Expected field 'INPUT'." << endl;						\
		cerr << "    Instead, '" << field << "' was found." << endl;		\
		return err_type::test_format_error;									\
	}																		\
	size_t n_inputs;														\
	F >> n_inputs;															\
	if (n_inputs != 0) {													\
		cerr << ERROR << endl;												\
		cerr << "    No input files are allowed in this test." << endl;		\
		cerr << "    Instead, " << n_inputs << " were specified." << endl;	\
		return err_type::test_format_error;									\
	}																		\
	F >> field;																\
	if (field != "BODY") {													\
		cerr << ERROR << endl;												\
		cerr << "    Expected field 'BODY'." << endl;						\
		cerr << "    Instead, '" << field << "' was found." << endl;		\
		return err_type::test_format_error;									\
	}																		\
}

#define test_correctness_result(tree, res_library)										\
{																						\
	const linearrgmnt& arr = res_library.second;										\
	/* ensure that the result is an arrangement */										\
	if (not is_arrangement(arr)) {														\
		cerr << ERROR << endl;															\
		cerr << "    The result is not an arrangement." << endl;						\
		cerr << "    Arrangement:     " << arr << endl;									\
		cerr << "    For tree: " << endl;												\
		cerr << tree << endl;															\
		return err_type::test_exe_error;												\
	}																					\
	/* ensure that value of D is correct */												\
	const uint32_t D = sum_length_edges(tree, res_library.second);						\
	if (D != res_library.first) {														\
		cerr << ERROR << endl;															\
		cerr << "    Value of D returned by method is incorrect." << endl;				\
		cerr << "    Arrangement:     " << res_library.second << endl;					\
		cerr << "    Inv Arrangement: " << invlinarr(res_library.second) << endl;		\
		cerr << "    Value of D returned: " << res_library.first << endl;				\
		cerr << "    Actual value of D:   " << D << endl;								\
		cerr << "    For tree: " << endl;												\
		cerr << tree << endl;															\
		return err_type::test_exe_error;												\
	}																					\
	/* compute Dmin by brute force */													\
	const pair<uint32_t, linearrgmnt> res_bf = Dmin_Unconstrained_bruteforce(tree);		\
	/* compare results obtained by the library and by brute force */					\
	if (res_library.first != res_bf.first) {											\
		cerr << ERROR << endl;															\
		cerr << "    Values of projective Dmin do not coincide." << endl;				\
		cerr << "    Library:" << endl;													\
		cerr << "        Value: " << res_library.first << endl;							\
		cerr << "        Arrangement:     " << res_library.second << endl;				\
		cerr << "        Inv Arrangement: " << invlinarr(res_library.second) << endl;	\
		cerr << "    Bruteforce:" << endl;												\
		cerr << "        Value: " << res_bf.first << endl;								\
		cerr << "        Arrangement:     " << res_bf.second << endl;					\
		cerr << "        Inv Arrangement: " << invlinarr(res_bf.second) << endl;		\
		cerr << "    For tree: " << endl;												\
		cerr << tree << endl;															\
		return err_type::test_exe_error;												\
	}																					\
}

pair<uint32_t, linearrgmnt> Dmin_Unconstrained_bruteforce(const ftree& t) {
	const uint32_t n = t.n_nodes();
	if (n == 1) { return make_pair(0, linearrgmnt(0,0)); }

	linearrgmnt arr(n);
	std::iota(arr.begin(), arr.end(), 0);

	linearrgmnt arrMin;
	uint32_t Dmin = numeric_limits<uint32_t>::max();

	do {
		const uint32_t D = sum_length_edges(t, arr);
		if (Dmin > D) {
			Dmin = D;
			arrMin = arr;
		}
	}
	while (std::next_permutation(arr.begin(), arr.end()));

	return make_pair(Dmin, arrMin);
}

// -----------------------------------------------------------------------------

err_type test_Unconstrained(ifstream& fin) {
	return err_type::no_error;
}

err_type test_Unconstrained_YS(ifstream& fin) {
	consume_beginning(fin)

	// read number of vertices
	uint32_t n;
	while (fin >> n) {
		// enumerate all trees of 'n' vertices
		generate::all_ulab_free_trees TreeGen(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			const ftree tree = TreeGen.get_tree();

			// compute Dmin using the library's algorithm
			const pair<uint32_t, linearrgmnt> res_library
				= compute_Dmin(tree, algorithms_Dmin::Unconstrained_YS);

			test_correctness_result(tree, res_library);
		}
	}

	return err_type::no_error;
}

err_type test_Unconstrained_FC(ifstream& fin) {
	consume_beginning(fin)

	// read number of vertices
	uint32_t n;
	while (fin >> n) {
		// enumerate all trees of 'n' vertices
		generate::all_ulab_free_trees TreeGen(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			const ftree tree = TreeGen.get_tree();

			// compute Dmin using the library's algorithm
			const pair<uint32_t, linearrgmnt> res_library
				= compute_Dmin(tree, algorithms_Dmin::Unconstrained_FC);

			test_correctness_result(tree, res_library);
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_Dmin_free(const string& alg, ifstream& fin) {
	err_type r;
	if (alg == "Unconstrained") {
		r = test_Unconstrained(fin);
	}
	else if (alg == "Unconstrained_YS") {
		r = test_Unconstrained_YS(fin);
	}
	else if (alg == "Unconstrained_FC") {
		r = test_Unconstrained_FC(fin);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Test not implemented for algorithm '" << alg << "'." << endl;
		r = err_type::not_implemented;
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
