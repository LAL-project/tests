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
namespace generate {

err_type exe_gen_arr_all(std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand(std::ifstream& fin) noexcept;
err_type exe_gen_arr_all_bipartite(std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand_bipartite(std::ifstream& fin) noexcept;
err_type exe_gen_arr_all_projective(std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand_projective(std::ifstream& fin) noexcept;
err_type exe_gen_arr_all_planar(std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand_planar(std::ifstream& fin) noexcept;
err_type exe_gen_trees_alf(std::ifstream& fin) noexcept;
err_type exe_gen_trees_alr(std::ifstream& fin) noexcept;
err_type exe_gen_trees_auf(std::ifstream& fin) noexcept;
err_type exe_gen_trees_aufb(std::ifstream& fin) noexcept;
err_type exe_gen_trees_aur(std::ifstream& fin) noexcept;
err_type exe_gen_trees_rlf(std::ifstream& fin) noexcept;
err_type exe_gen_trees_rlr(std::ifstream& fin) noexcept;
err_type exe_gen_trees_ruf(std::ifstream& fin) noexcept;
err_type exe_gen_trees_rur(std::ifstream& fin) noexcept;

} // namespace generate
} // namespace tests
