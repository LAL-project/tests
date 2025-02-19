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
#include <lal/generate/all_ulab_free_bistar_trees.hpp>
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

namespace aufb {
struct extra_params {
	std::vector<lal::numeric::integer> number_of_trees;
};

inline bool is_star(const lal::graphs::free_tree& T) noexcept
{
	return T.is_of_tree_type(lal::graphs::tree_type::star);
}
inline bool is_bistar(const lal::graphs::free_tree& T) noexcept
{
	return T.is_of_tree_type(lal::graphs::tree_type::bistar);
}

#define process                                                                \
	const ftree_check err = test_validity_tree(n, T);                          \
	if (err != ftree_check::correct) {                                         \
		std::cerr << ERROR << '\n';                                            \
		std::cerr << "    Tree of index " << gen << " is not correct.\n";      \
		std::cerr << "    Error: " << tree_check_to_string(err) << '\n';       \
		std::cerr << T << '\n';                                                \
		return err_type::test_execution;                                       \
	}                                                                          \
	gen += 1;                                                                  \
	/* ensure uniqueness */                                                    \
	for (std::size_t j = 0; j < it; ++j) {                                     \
		if (lal::utilities::are_trees_isomorphic(all_trees[j], T)) {           \
			std::cerr << ERROR << '\n';                                        \
			std::cerr << "    Found two isomorphic trees!.\n";                 \
			std::cerr << "    After generating the " << it << " tree.\n";      \
			std::cerr << "    The isomorphic tree is at j= " << j << '\n';     \
			std::cerr << "    Just generated:\n";                              \
			std::cerr << T << '\n';                                            \
			std::cerr << "   The tree at position j= " << j << ":\n";          \
			std::cerr << all_trees[j] << '\n';                                 \
			return err_type::test_execution;                                   \
		}                                                                      \
	}                                                                          \
	if (not is_star(T) and not is_bistar(T)) {                                 \
		std::cerr << ERROR << '\n';                                            \
		std::cerr << "    The tree generated is not a bistar tree!.\n";        \
		std::cerr << T << '\n';                                                \
		return err_type::test_execution;                                       \
	}                                                                          \
	all_trees[it] = std::move(T);                                              \
	++it;

#define check                                                                  \
	if (n < UFT.size() and gen != UFT[n]) {                                    \
		std::cerr << ERROR << '\n';                                            \
		std::cerr << "    Exhaustive generation of unlabelled free trees\n";   \
		std::cerr << "    Amount of trees should be: " << UFT[n] << '\n';      \
		std::cerr << "    But generated: " << gen << '\n';                     \
		std::cerr << "    For a size of " << n << " vertices\n";               \
		return err_type::test_execution;                                       \
	}

err_type test_for_n_while(
	uint64_t n,
	lal::generate::all_ulab_free_bistar_trees& TreeGen,
	const extra_params& params
) noexcept
{
	const auto& UFT = params.number_of_trees;

	// number of generated trees
	lal::numeric::integer gen = 0;

	// store the trees so we can test for uniqueness
	lal::detail::array<lal::graphs::free_tree> all_trees(UFT[n].to_uint());
	std::size_t it = 0;

	while (not TreeGen.end()) {
		lal::graphs::free_tree T = TreeGen.get_tree();
		TreeGen.next();
		process;
	}

	check;
	return err_type::no_error;
}

err_type test_for_n_for(
	uint64_t n,
	lal::generate::all_ulab_free_bistar_trees& TreeGen,
	const extra_params& params
) noexcept
{
	const auto& UFT = params.number_of_trees;

	// number of caterpillar trees
	lal::numeric::integer n_caterpillar = 0;
	// number of generated trees
	lal::numeric::integer gen = 0;

	// store the trees so we can test for uniqueness
	lal::detail::array<lal::graphs::free_tree> all_trees(UFT[n].to_uint());
	std::size_t it = 0;

	for (; not TreeGen.end(); TreeGen.next()) {
		lal::graphs::free_tree T = TreeGen.get_tree();
		process;
	}

	check;
	return err_type::no_error;
}

err_type test_for_n_yield(
	uint64_t n,
	lal::generate::all_ulab_free_bistar_trees& TreeGen,
	const extra_params& params
) noexcept
{
	const auto& UFT = params.number_of_trees;

	// number of generated trees
	lal::numeric::integer gen = 0;

	// store the trees so we can test for uniqueness
	lal::detail::array<lal::graphs::free_tree> all_trees(UFT[n].to_uint());
	std::size_t it = 0;

	while (not TreeGen.end()) {
		lal::graphs::free_tree T = TreeGen.yield_tree();
		process;
	}

	check;
	return err_type::no_error;
}

template <bool init>
err_type call_test_exhaustive(
	uint64_t n1, uint64_t n2, const extra_params& ep, uint64_t R
) noexcept
{
	{
		const auto err = test_exhaustive_enumeration_of_trees<
			init,
			lal::generate::all_ulab_free_bistar_trees>(
			n1, n2, test_for_n_while, ep, R
		);

		if (err != err_type::no_error) {
			return err;
		}
	}

	{
		const auto err = test_exhaustive_enumeration_of_trees<
			init,
			lal::generate::all_ulab_free_bistar_trees>(
			n1, n2, test_for_n_for, ep, R
		);
		if (err != err_type::no_error) {
			return err;
		}
	}

	{
		const auto err = test_exhaustive_enumeration_of_trees<
			init,
			lal::generate::all_ulab_free_bistar_trees>(
			n1, n2, test_for_n_yield, ep, R
		);
		if (err != err_type::no_error) {
			return err;
		}
	}
	return err_type::no_error;
}

} // namespace aufb

err_type exe_gen_trees_aufb(std::ifstream& fin) noexcept
{

	/* BUILD TESTING DATA */

	// number of unlabelled free bistar trees
	aufb::extra_params params;
	// size of the vector with the number of unlabelled free trees

	params.number_of_trees = std::vector<lal::numeric::integer>{
		0,
		0,
		1,
		1,
		2,
		2,
		3,
		3,
		4,
		4,
		5,
		5,
		6,
		6,
		7,
		7,
		8,
		8,
	};

	// --- do the tests

	uint64_t R;
	fin >> R;

	uint64_t n1, n2;
	while (fin >> n1 >> n2) {
		{
			const auto err =
				aufb::call_test_exhaustive<true>(n1, n2, params, R);
			if (err != err_type::no_error) {
				return err;
			}
		}

		{
			const auto err =
				aufb::call_test_exhaustive<false>(n1, n2, params, R);
			if (err != err_type::no_error) {
				return err;
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace generate
} // namespace tests
