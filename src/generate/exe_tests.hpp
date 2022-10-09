/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
 *
 *  Tests of the Linear Arrangement Library is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU Affero
 *  General Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later version.
 *
 *  Tests of the Linear Arrangement Library is distributed in the hope
 *  that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Tests of the Linear Arrangement Library.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
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
#include <functional>
#include <fstream>
#include <vector>
#include <string>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace generate {

err_type exe_gen_arr_all(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_arr_all_projective(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand_projective(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_arr_all_planar(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_arr_rand_planar(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_alf(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_alr(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_auf(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_aur(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_rlf(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_rlr(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_ruf(const input_list& inputs, std::ifstream& fin) noexcept;
err_type exe_gen_trees_rur(const input_list& inputs, std::ifstream& fin) noexcept;

} // -- namespace generate
} // -- namespace tests
