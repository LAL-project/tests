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
#include "generate/parse_keywords.hpp"
#include "generate/exe_tests.hpp"

namespace tests {
namespace generate {

err_type call_generate_arrangements(
	const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin
) noexcept
{
	const std::string& key = keywords[i];
	if (key == "all_projective") {
		return exe_gen_arr_all_projective(fin);
	}
	else if (key == "rand_projective") {
		return exe_gen_arr_rand_projective(fin);
	}
	else if (key == "all_planar") {
		return exe_gen_arr_all_planar(fin);
	}
	else if (key == "rand_planar") {
		return exe_gen_arr_rand_planar(fin);
	}
	else if (key == "all_bipartite") {
		return exe_gen_arr_all_bipartite(fin);
	}
	else if (key == "rand_bipartite") {
		return exe_gen_arr_rand_bipartite(fin);
	}
	else if (key == "all") {
		return exe_gen_arr_all(fin);
	}
	else if (key == "rand") {
		return exe_gen_arr_rand(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_generate_trees(
	const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin
) noexcept
{
	const std::string& key = keywords[i];
	if (key == "alf") {
		return exe_gen_trees_alf(fin);
	}
	else if (key == "alr") {
		return exe_gen_trees_alr(fin);
	}
	else if (key == "auf") {
		return exe_gen_trees_auf(fin);
	}
	else if (key == "aufb") {
		return exe_gen_trees_aufb(fin);
	}
	else if (key == "aur") {
		return exe_gen_trees_aur(fin);
	}
	else if (key == "rlf") {
		return exe_gen_trees_rlf(fin);
	}
	else if (key == "rlr") {
		return exe_gen_trees_rlr(fin);
	}
	else if (key == "ruf") {
		return exe_gen_trees_ruf(fin);
	}
	else if (key == "rur") {
		return exe_gen_trees_rur(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_generate(
	const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin
) noexcept
{
	const std::string& key = keywords[i];
	if (key == "trees") {
		return call_generate_trees(keywords, i + 1, fin);
	}
	else if (key == "arrangements") {
		return call_generate_arrangements(keywords, i + 1, fin);
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
	if (key == "generate") {
		return call_generate(keywords, 1, fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at 0: '" << key << "'.\n";
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

} // namespace generate
} // namespace tests
