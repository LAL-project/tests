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
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/rand_planar_arrangements.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/iterators/E_iterator.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace iterators;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace generate {

err_type exe_gen_arr_rand_planar(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	uint64_t n, ntrees, nit;
	while (fin >> n >> ntrees >> nit) {
		// do 'ntrees' trees of 'n' vertices
		rand_ulab_free_trees TreeGen(n);

		for (uint64_t nt = 0; nt < ntrees; ++nt) {
			const free_tree T = TreeGen.get_tree();

			rand_planar_arrangements RandArr(T, 100);

			for (uint64_t it = 0; it < nit; ++it) {
				const linear_arrangement arr = RandArr.get_arrangement();

				// Do some sanity checks.
				if (not lal::linarr::is_planar(T, arr)) {
					cerr << ERROR << endl;
					cerr << "    Generation of random arrangement failed with error:" << endl;
					cerr << "    Arrangement:     " << arr << endl;
					cerr << "    Inv Arrangement: " << invlinarr(arr) << endl;
					cerr << "    For tree:" << endl;
					cerr << T << endl;
					cerr << T.get_head_vector() << endl;
					return err_type::test_execution;
				}
			}

			for (uint64_t it = 0; it < nit; ++it) {
				const linear_arrangement arr = RandArr.yield_arrangement();

				// Do some sanity checks.
				if (not lal::linarr::is_planar(T, arr)) {
					cerr << ERROR << endl;
					cerr << "    Generation of random arrangement failed with error:" << endl;
					cerr << "    Arrangement:     " << arr << endl;
					cerr << "    Inv Arrangement: " << invlinarr(arr) << endl;
					cerr << "    For tree:" << endl;
					cerr << T.get_head_vector() << endl;
					return err_type::test_execution;
				}
			}
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
