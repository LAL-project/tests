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

// lal includes
#include <lal/generate/all_lab_rooted_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/properties/degrees.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

// generate includes
#include "generate/test_exhaustive_tree_enumeration.hpp"
#include "common/tree_validity_check.hpp"

namespace tests {
namespace generate {

namespace alr {
struct extra_params { };

#define process																\
	const rtree_check err = test_validity_tree(n, T);						\
	if (err != rtree_check::correct) {										\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Tree of index " << gen << " is not correct.\n";	\
		std::cerr << "    Error: " << tree_check_to_string(err) << '\n';	\
		std::cerr << T << '\n';												\
		return err_type::test_execution;									\
	}																		\
	/* compute 'statistics' */												\
	gen += 1;

#define check																				\
	/* Prüfer's formula: make sure that the generator made									\
	   as many trees as n^(n - 1) <- note that we are dealing								\
	   with labelled ROOTED trees!															\
	   https://oeis.org/A000169/list */														\
	const lal::numeric::integer nn(n);														\
	const lal::numeric::integer total(n == 1 ? 1 : (nn.power(nn - 1)));						\
	if (gen != total) {																		\
		std::cerr << ERROR << '\n';															\
		std::cerr << "    Exhaustive generation of labelled rooted trees\n";				\
		std::cerr << "    Amount of trees of " << n << " vertices should be: " << total << '\n';\
		std::cerr << "    But generated: " << gen << '\n';									\
		std::cerr << "    For a size of " << n << " vertices\n";							\
		return err_type::test_execution;													\
	}

err_type test_for_n_while
(uint64_t n, lal::generate::all_lab_rooted_trees& TreeGen, const extra_params&)
{
	uint64_t gen = 0;
	while (not TreeGen.end()) {
		const lal::graphs::rooted_tree T = TreeGen.get_tree();
		TreeGen.next();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_for
(uint64_t n, lal::generate::all_lab_rooted_trees& TreeGen, const extra_params&)
{
	uint64_t gen = 0;
	for (; not TreeGen.end(); TreeGen.next()) {
		const lal::graphs::rooted_tree T = TreeGen.get_tree();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_yield
(uint64_t n, lal::generate::all_lab_rooted_trees& TreeGen, const extra_params&)
{
	uint64_t gen = 0;
	while (not TreeGen.end()) {
		const lal::graphs::rooted_tree T = TreeGen.yield_tree();
		process;
	}
	check;
	return err_type::no_error;
}

template<bool init>
err_type call_test_exhaustive(uint64_t n1, uint64_t n2) noexcept {
	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_lab_rooted_trees>
		(n1, n2, test_for_n_while, extra_params{});
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_lab_rooted_trees>
		(n1, n2, test_for_n_for, extra_params{});
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_lab_rooted_trees>
		(n1, n2, test_for_n_yield, extra_params{});
	if (err != err_type::no_error) { return err; }
	}
	return err_type::no_error;
}

} // -- namespace alr

err_type exe_gen_trees_alr(const input_list& inputs, std::ifstream& fin) {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	// --- do the tests

	uint64_t n1, n2;
	while (fin >> n1 >> n2) {
		{
		const auto err = alr::call_test_exhaustive<true>(n1, n2);
		if (err != err_type::no_error) { return err; }
		}

		{
		const auto err = alr::call_test_exhaustive<false>(n1, n2);
		if (err != err_type::no_error) { return err; }
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
