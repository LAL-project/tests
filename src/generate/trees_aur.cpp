/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <fstream>

// lal includes
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/utilities/tree_isomorphism.hpp>

// common includes
#include "common/definitions.hpp"

// generate includes
#include "generate/test_exhaustive_tree_enumeration.hpp"
#include "common/tree_validity_check.hpp"

namespace tests {
namespace generate {

namespace aur {
struct extra_params {
	std::vector<lal::numeric::integer> URT;
};

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
	gen += 1;																\
	/* ensure uniqueness */													\
	for (std::size_t j = 0; j < it; ++j) {									\
		if (lal::utilities::are_trees_isomorphic(all_rooted_trees[j], T)) {	\
			std::cerr << ERROR << '\n';										\
			std::cerr << "    Found two isomorphic trees!.\n";				\
			std::cerr << "    After generating the " << it << " tree.\n";	\
			std::cerr << "    The isomorphic tree is at j= " << j << '\n';	\
			std::cerr << "    Just generated:\n";							\
			std::cerr << T << '\n';											\
			std::cerr << "   The tree at position j= " << j << ":\n";		\
			std::cerr << all_rooted_trees[j] << '\n';						\
			return err_type::test_execution;								\
		}																	\
	}																		\
	all_rooted_trees[it] = std::move(T);									\
	++it;

#define check																	\
	/* make sure that the amount of trees generate coincides					\
	   with the series from the OEIS */											\
	if (n < URT.size() and gen != URT[n]) {										\
		std::cerr << ERROR << '\n';												\
		std::cerr << "    Exhaustive generation of unlabelled rooted trees\n";	\
		std::cerr << "    Amount of trees should be: " << URT[n] << '\n';		\
		std::cerr << "    But generated: " << gen << '\n';						\
		std::cerr << "    For a size of " << n << " vertices\n";				\
		return err_type::test_execution;										\
	}

err_type test_for_n_while
(uint64_t n, lal::generate::all_ulab_rooted_trees& TreeGen, const extra_params& params)
noexcept
{
	const auto& URT = params.URT;

	// number of generated trees
	lal::numeric::integer gen = 0;

	lal::detail::array<lal::graphs::rooted_tree> all_rooted_trees(URT[n].to_uint());
	std::size_t it = 0;

	while (not TreeGen.end()) {
		lal::graphs::rooted_tree T = TreeGen.get_tree();
		TreeGen.next();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_for
(uint64_t n, lal::generate::all_ulab_rooted_trees& TreeGen, const extra_params& params)
noexcept
{
	const auto& URT = params.URT;

	// number of generated trees
	lal::numeric::integer gen = 0;

	lal::detail::array<lal::graphs::rooted_tree> all_rooted_trees(URT[n].to_uint());
	std::size_t it = 0;

	for (; not TreeGen.end(); TreeGen.next()) {
		lal::graphs::rooted_tree T = TreeGen.get_tree();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_yield
(uint64_t n, lal::generate::all_ulab_rooted_trees& TreeGen, const extra_params& params)
noexcept
{
	const auto& URT = params.URT;

	// number of generated trees
	lal::numeric::integer gen = 0;

	lal::detail::array<lal::graphs::rooted_tree> all_rooted_trees(URT[n].to_uint());
	std::size_t it = 0;

	while (not TreeGen.end()) {
		lal::graphs::rooted_tree T = TreeGen.yield_tree();
		process;
	}
	check;
	return err_type::no_error;
}

template <bool init>
err_type call_test_exhaustive(
	uint64_t n1, uint64_t n2,
	const extra_params& ep,
	uint64_t R
)
noexcept
{
	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_ulab_rooted_trees>
		(n1, n2, test_for_n_while, ep, R);
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_ulab_rooted_trees>
		(n1, n2, test_for_n_for, ep, R);
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, lal::generate::all_ulab_rooted_trees>
		(n1, n2, test_for_n_yield, ep, R);
	if (err != err_type::no_error) { return err; }
	}
	return err_type::no_error;
}

} // -- namespace aur

err_type exe_gen_trees_aur(std::ifstream& fin) noexcept {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unlabelled free trees
	aur::extra_params params;

	params.URT =
	std::vector<lal::numeric::integer>{
		0,
		1,
		1,
		2,
		4,
		9,
		20,
		48,
		115,
		286,
		719,
		1842,
		4766,
		12486,
		32973,
		87811,
		235381,
		634847,
		1721159,
		4688676,
		12826228,
		35221832,
		97055181,
		268282855,
		743724984,
		{"2067174645"},
		{"5759636510"},
		{"16083734329"},
		{"45007066269"},
		{"126186554308"},
		{"354426847597"}
	};

	// --- do the tests

	uint64_t R;
	fin >> R;

	uint64_t n1, n2;
	while (fin >> n1 >> n2) {
		{
		const auto err = aur::call_test_exhaustive<true>(n1, n2, params, R);
		if (err != err_type::no_error) { return err; }
		}

		{
		const auto err = aur::call_test_exhaustive<false>(n1, n2, params, R);
		if (err != err_type::no_error) { return err; }
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
