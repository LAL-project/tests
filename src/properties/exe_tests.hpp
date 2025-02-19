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

/* This file contains the definition of the different functions used for
 * testing the library.
 *
 * This file is not to be included by any of the implemented tests, as adding
 * a new function to this file will make ALL the corresponding .cpp files to
 * be recompiled.
 */

// C++ includes
#include <fstream>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace properties {

err_type exe_properties_general(std::ifstream& fin) noexcept;
err_type exe_properties_bipartite_coloring(std::ifstream& fin) noexcept;
err_type exe_properties_MHD_All_trees(std::ifstream& fin) noexcept;
err_type exe_properties_orbits(std::ifstream& fin) noexcept;
err_type exe_properties_ExpVar_C(std::ifstream& fin) noexcept;
err_type exe_properties_ExpVar_D(std::ifstream& fin) noexcept;
err_type exe_properties_centre(std::ifstream& fin) noexcept;
err_type exe_properties_centroid(std::ifstream& fin) noexcept;
err_type exe_properties_connected_components(std::ifstream& fin) noexcept;
err_type exe_properties_diameter(std::ifstream& fin) noexcept;
err_type exe_properties_caterpillar_distance(std::ifstream& fin) noexcept;
err_type exe_properties_expected_D_projective(std::ifstream& fin) noexcept;
err_type exe_properties_expected_D_planar_brute_force(std::ifstream& fin
) noexcept;
err_type exe_properties_expected_D_planar_quadratic(std::ifstream& fin
) noexcept;
err_type exe_properties_expected_D_bipartite(std::ifstream& fin) noexcept;
err_type exe_properties_branchless_paths(std::ifstream& fin) noexcept;

} // namespace properties
} // namespace tests
