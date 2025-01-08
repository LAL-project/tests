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

#pragma once

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace generate {

template <
	bool use_constructor,
	typename gen_t,
	typename Callable,
	typename extra_params
>
inline
err_type test_exhaustive_enumeration_of_trees(
	uint64_t n1, uint64_t n2,
	const Callable& f,
	const extra_params& ep,
	uint64_t R
)
noexcept
{
	gen_t TreeGen;

	for (auto n = n1; n <= n2; ++n) {
		if constexpr (use_constructor) {
			TreeGen = gen_t(n);
		}
		else {
			TreeGen.init(n);
		}
		TreeGen.deactivate_all_postprocessing_actions();
		TreeGen.set_calculate_tree_type(true);

		for (std::size_t i = 0; i < R; ++i) {
			const auto err = f(n, TreeGen, ep);
			if (err != tests::err_type::no_error) { return err; }
			TreeGen.reset();
		}

		TreeGen.clear();
	}
	return tests::err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
