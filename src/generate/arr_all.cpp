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
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_arrangements.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/linarr/formal_constraints.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace generate;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

#define check_and_process_arrangement(c)									\
	if (not linarr::is_arrangement(T, arr)) {								\
		cerr << ERROR << endl;												\
		cerr << "    In check: " << c << endl;								\
		cerr << "    Generation of arrangement failed with error:" << endl;	\
		cerr << "    Arrangement:     " << arr << endl;						\
		cerr << "    Inv Arrangement: " << invlinarr(arr) << endl;			\
		cerr << "    For tree:" << endl;									\
		cerr << T << endl;													\
		cerr << T.get_head_vector() << endl;								\
		return err_type::test_execution;									\
	}																		\
	++iterations;															\
	list_arrs.insert(arr);

#define final_check(c)														\
	if (formula != iterations or formula != list_arrs.size()) {				\
		cerr << ERROR << endl;												\
		cerr << "    In check: " << c << endl;								\
		cerr << "    Number of projective arrangements generated" << endl;	\
		cerr << "    does not agree with the formula." << endl;				\
		cerr << "        formula= " << formula << endl;						\
		cerr << "        iterations= " << iterations << endl;				\
		cerr << "        unique amount= " << list_arrs.size() << endl;		\
		cerr << "    List of arrangements:" << endl;						\
		for (const auto& v : list_arrs) {									\
		cerr << "        " << v << endl;									\
		}																	\
		cerr << "    For tree:" << endl;									\
		cerr << T << endl;													\
		cerr << T.get_head_vector() << endl;								\
		return err_type::test_execution;									\
	}

namespace tests {
namespace generate {

inline integer factorial(int64_t f) noexcept {
	if (f == 0) { return 1; }
	const integer f1 = factorial(f - 1);
	return f1*f;
}

inline integer amount_planar(const free_tree& T) noexcept {
	return factorial(T.get_num_nodes());
}

inline err_type test_a_tree(free_tree& T, uint64_t nrelabs) noexcept {
	vector<edge> edges = T.get_edges();

	for (uint64_t i = 0; i < 2*nrelabs; ++i) {
		relabel_tree_vertices(edges, T, (i < nrelabs ? false : true), false);

		uint64_t iterations = 0;
		set<linear_arrangement> list_arrs;
		const integer formula = amount_planar(T);

		all_arrangements ArrGen(T);

		// USAGE 1
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		while (not ArrGen.end()) {
			const linear_arrangement arr = ArrGen.get_arrangement();
			ArrGen.next();

			// Do some sanity checks.
			check_and_process_arrangement("Usage 1");
		}
		final_check("Usage 1");
		}

		// USAGE 2
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		for (; not ArrGen.end(); ArrGen.next()) {
			const linear_arrangement arr = ArrGen.get_arrangement();

			// Do some sanity checks.
			check_and_process_arrangement("Usage 2");
		}
		final_check("Usage 2");
		}

		// USAGE 3
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		while (not ArrGen.end()) {
			const linear_arrangement arr = ArrGen.yield_arrangement();

			// Do some sanity checks.
			check_and_process_arrangement("Usage 3");
		}
		final_check("Usage 3");
		}
	}
	return err_type::no_error;
}

err_type exe_gen_arr_all(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	uint64_t n, nrelabs;
	while (fin >> n >> nrelabs) {
		// do all trees of 'n' vertices
		all_ulab_free_trees TreeGen(n);

		while (not TreeGen.end()) {
			free_tree fT = TreeGen.get_tree();
			TreeGen.next();

			const err_type e = test_a_tree(fT, nrelabs);
			if (e != err_type::no_error) {
				return e;
			}
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
