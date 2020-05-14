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
#include <lal/graphs/output.hpp>
#include <lal/generate/rand_projective_arrangements.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/linarr/C.hpp>
#include <lal/iterators/E_iterator.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace iterators;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"

namespace exe_tests {

/* ---- RANDOM PROJECTIVE ARRANGEMENTS ---- */

string is_rand_proj_arr_correct(
	const rtree& rT, const ftree& fT, const linearrgmnt& arr
)
{
	set<position> setpos;
	for (node u = 0; u < rT.n_nodes(); ++u) {
		setpos.insert(arr[u]);
	}

	if (setpos.size() != rT.n_nodes()) {
		return "There are collisions in vertices positions";
	}

	if (not is_linarr_projective(rT, fT, arr)) {
		// the arrangement is not projective;
		return "The arrangement is not projective";
	}

	// no error
	return "No error";
}

err_type test_rand_projective_arrangements(ifstream& fin) {
	uint32_t n, ntrees, nit;
	while (fin >> n >> ntrees >> nit) {
	// do 'ntrees' trees of 'n' vertices
	rand_ulab_rooted_trees TreeGen(n);

	uint32_t nt = 0;
	while (nt < ntrees) {
		const rtree rT = TreeGen.make_rand_tree();
		const ftree fT = rT.to_undirected();

		for (uint32_t it = 0; it < nit; ++it) {
			const linearrgmnt arr = rand_projective_arrgmnt(rT, false);

			// Do some sanity checks.
			const string err = is_rand_proj_arr_correct(rT, fT, arr);
			if (err != "No error") {
				cerr << ERROR << endl;
				cerr << "    Generation of random arrangement for rtree:" << endl;
				cerr << rT << endl;
				cerr << "    Failed with error: '" << err << "'" << endl;
				cerr << "    Arrangement:" << endl;
				cerr << "    " << arr << endl;
				cerr << "    Inverse linear arrangement:" << endl;
				cerr << "    " << invlinarr(arr) << endl;
				return err_type::test_exe_error;
			}
		}

		++nt;
	}
	}
	return err_type::no_error;
}

err_type exe_gen_rand_arrangements(ifstream& fin) {
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

	string arr_type;
	fin >> arr_type;

	const set<string> allowed_arr_types({"projective"});
	if (allowed_arr_types.find(arr_type) == allowed_arr_types.end()) {
		cerr << ERROR << endl;
		cerr << "    Unhandled arrangement type '" << arr_type << "'." << endl;
		cerr << "    Allowed values: " << endl;
		for (const string& type : allowed_arr_types) {
		cerr << "    - " << type << endl;
		}
		return err_type::test_format_error;
	}

	const err_type err =
	[&]() -> err_type {
		if (arr_type == "projective") {
			return test_rand_projective_arrangements(fin);
		}
		return err_type::test_format_error;
	}();

	if (err != err_type::no_error) { return err; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
