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

// common includes
#include "common/parse_header.hpp"
#include "detail/parse_keywords.hpp"
#include "detail/exe_tests.hpp"

namespace tests {
namespace detail {

err_type call_internal(
	const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin
) noexcept
{
	const std::string& key = keywords[i];
	if (key == "sorting") {
		return exe_detail_sorting(fin);
	}
	if (key == "traversal") {
		return exe_detail_bfs(fin);
	}
	if (key == "avl_functions") {
		return exe_detail_avl_functions(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type
call_main(const std::vector<std::string>& keywords, std::ifstream& fin) noexcept
{
	const std::string& key = keywords[0];
	if (key == "detail") {
		return call_internal(keywords, 1, fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at 0: '" << key << "'.\n";
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

} // namespace detail
} // namespace tests
