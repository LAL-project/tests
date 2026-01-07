/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2026
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
 ********************************************************************/

#pragma once

// C++ includes
#include <optional>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/basic_types.hpp>

// custom includes
#include "common/definitions.hpp"

namespace tests {
namespace utilities {

inline std::optional<bool> read_should_be_or_not(std::ifstream& fin) noexcept
{
	std::string should_what;
	fin >> should_what;
	if (should_what == "ISOMORPHIC") {
		return true;
	}
	if (should_what == "NON_ISOMORPHIC") {
		return false;
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    String '" << should_what
			  << "' is not a valid identifier of the test.\n";
	std::cerr << "    Should be either: 'ISOMORPHIC' or 'NON_ISOMORPHIC'.\n";
	return {};
}

template <class tree_t>
inline void read_free(std::ifstream& fin, tree_t& t) noexcept
{
	std::vector<lal::edge> edges(t.get_num_nodes() - 1);
	for (auto& e : edges) {
		fin >> e.first >> e.second;
	}
	t.add_edges(edges);
}

template <class tree_t>
inline void read_rooted(std::ifstream& fin, tree_t& t) noexcept
{
	read_free(fin, t);
	lal::node r;
	fin >> r;
	t.set_root(r);
}

} // namespace utilities
} // namespace tests
