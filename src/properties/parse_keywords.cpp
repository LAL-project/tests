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

// C++ includes
#include <iostream>

// common includes
#include "common/parse_header.hpp"
#include "properties/parse_keywords.hpp"
#include "properties/exe_tests.hpp"

namespace tests {
namespace properties {

err_type call_expected_D_planar(const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin)
noexcept
{
	const std::string& key = keywords[i];
	if (key == "brute_force") {
		return exe_properties_expected_D_planar_brute_force(fin);
	}
	if (key == "quadratic") {
		return exe_properties_expected_D_planar_quadratic(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_expected_D(const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin)
noexcept
{
	const std::string& key = keywords[i];
	if (key == "Projective") {
		return exe_properties_expected_D_projective(fin);
	}
	if (key == "Planar") {
		return call_expected_D_planar(keywords, i+1, fin);
	}
	if (key == "Bipartite") {
		return exe_properties_expected_D_bipartite(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_properties(const std::vector<std::string>& keywords, std::size_t i, std::ifstream& fin)
noexcept
{
	const std::string& key = keywords[i];
	if (key == "general") {
		return exe_properties_general(fin);
	}
	if (key == "MHD_All_Trees") {
		return exe_properties_MHD_All_trees(fin);
	}
	if (key == "orbits") {
		return exe_properties_orbits(fin);
	}
	if (key == "exp_var_C") {
		return exe_properties_ExpVar_C(fin);
	}
	if (key == "exp_var_D") {
		return exe_properties_ExpVar_D(fin);
	}
	if (key == "expected_D") {
		return call_expected_D(keywords, i+1, fin);
	}
	if (key == "centre") {
		return exe_properties_centre(fin);
	}
	if (key == "centroid") {
		return exe_properties_centroid(fin);
	}
	if (key == "connected_components") {
		return exe_properties_connected_components(fin);
	}
	if (key == "diameter") {
		return exe_properties_diameter(fin);
	}
	if (key == "caterpillar_distance") {
		return exe_properties_caterpillar_distance(fin);
	}
	if (key == "bipartite_coloring") {
		return exe_properties_bipartite_coloring(fin);
	}
	if (key == "branchless_paths") {
		return exe_properties_branchless_paths(fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at " << i << ": '" << key << "'.\n";
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_main(const std::vector<std::string>& keywords, std::ifstream& fin)
noexcept
{
	const std::string& key = keywords[0];
	if (key == "properties") {
		return call_properties(keywords, 1, fin);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unhandled keyword at 0: '" << key << "'.\n";
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

} // -- namespace properties
} // -- namespace tests
