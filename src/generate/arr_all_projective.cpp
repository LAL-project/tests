/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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
// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_projective_arrangements.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/linarr/formal_constraints.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace generate {

inline lal::numeric::integer factorial(uint64_t f) noexcept {
	if (f == 0) { return 1; }
	const lal::numeric::integer f1 = factorial(f - 1);
	return f1*f;
}

inline lal::numeric::integer amount_projective(const lal::graphs::rooted_tree& rT) noexcept {
	lal::numeric::integer k = 1;
	for (lal::node u = 0; u < rT.get_num_nodes(); ++u) {
		k *= factorial(rT.get_out_degree(u) + 1);
	}
	return k;
}

inline err_type test_a_tree(lal::graphs::rooted_tree& rT, uint64_t nrelabs) noexcept {
	std::vector<lal::edge> edges = rT.get_edges();

#define check_arrangement													\
	if (not lal::linarr::is_projective(rT, arr)) {							\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Generation of arrangement failed with error:\n";	\
		std::cerr << "    Arrangement:     " << arr.direct_as_vector() << '\n';	\
		std::cerr << "    Inv Arrangement: " << arr.inverse_as_vector() << '\n';\
		std::cerr << "    For tree:\n";										\
		std::cerr << rT << '\n';											\
		return err_type::test_execution;									\
	}																		\
	++iterations;															\
	list_arrs.insert(arr);

#define final_check															\
	if (formula != iterations or formula != list_arrs.size()) {				\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Number of projective arrangements generated\n";	\
		std::cerr << "    does not agree with the formula.\n";				\
		std::cerr << "        formula= " << formula << '\n';				\
		std::cerr << "        iterations= " << iterations << '\n';			\
		std::cerr << "        unique amount= " << list_arrs.size() << '\n';	\
		std::cerr << "    List of arrangements:\n";							\
		for (const auto& v : list_arrs) {									\
		std::cerr << "        " << v.direct_as_vector() << '\n';\
		}																	\
		std::cerr << "    For tree:\n";										\
		std::cerr << rT << '\n';											\
		return err_type::test_execution;									\
	}

	for (uint64_t i = 0; i < 2*nrelabs; ++i) {
		relabel_tree_vertices(edges, rT, (i < nrelabs ? false : true), false);

		uint64_t iterations = 0;
		std::set<lal::linear_arrangement> list_arrs;
		const lal::numeric::integer formula = amount_projective(rT);

		lal::generate::all_projective_arrangements ArrGen(rT);

		// USAGE 1
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		while (not ArrGen.end()) {
			const lal::linear_arrangement arr = ArrGen.get_arrangement();
			ArrGen.next();

			// Do some sanity checks.
			check_arrangement;
		}
		final_check;
		}

		// USAGE 2
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		for (; not ArrGen.end(); ArrGen.next()) {
			const lal::linear_arrangement arr = ArrGen.get_arrangement();

			// Do some sanity checks.
			check_arrangement;
		}
		final_check;
		}

		// USAGE 3
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
		while (not ArrGen.end()) {
			const lal::linear_arrangement arr = ArrGen.yield_arrangement();

			// Do some sanity checks.
			check_arrangement;
		}
		final_check;
		}
	}
	return err_type::no_error;
}

err_type exe_gen_arr_all_projective(const input_list& inputs, std::ifstream& fin) noexcept {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	uint64_t n, nrelabs;
	while (fin >> n >> nrelabs) {
		// do 'ntrees' trees of 'n' vertices
		lal::generate::all_ulab_rooted_trees TreeGen(n);

		while (not TreeGen.end()) {
			lal::graphs::rooted_tree rT = TreeGen.get_tree();
			TreeGen.next();

			const err_type e = test_a_tree(rT, nrelabs);
			if (e != err_type::no_error) {
				return e;
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
