/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
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

#pragma once

#include "common/definitions.hpp"
#include "common/tree_validity_check.hpp"

namespace tests {
namespace generate {

template <
	bool use_constructor,
	int seed,
	class Gen,
	class errstream
>
inline
err_type test_random_generation_of_trees
(uint64_t n1, uint64_t n2, int num_trees, errstream& errs)
noexcept
{

	Gen TreeGen;

	for (auto n = n1; n <= n2; ++n) {
		if constexpr (use_constructor) {
			TreeGen = Gen(n);
		}
		else {
			TreeGen.init(n);
		}

		for (int i = 0; i < num_trees; ++i) {
			const typename Gen::generated_tree_t T = TreeGen.get_tree();

			const auto err = test_validity_tree(n, T);
			if (err != decltype(err)::correct) {
				errs << ERROR << '\n';
				errs << "    Tree is not correct.\n";
				errs << "    Error: " << tree_check_to_string(err) << '\n';
				errs << T << '\n';
				return tests::err_type::test_execution;
			}
		}

		TreeGen.clear();
	}
	return tests::err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
