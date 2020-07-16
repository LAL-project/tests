/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
 *
 *  This file is part of Tests of the Linear Arrangement Library.
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
 * be compiled again.
 */

// C++ includes
#include <fstream>
#include <vector>
#include <string>

// custom includes
#include "definitions.hpp"

namespace exe_tests {

err_type exe_construction(std::ifstream& fin);

err_type exe_numeric_integer(std::ifstream& fin);
err_type exe_numeric_rational(std::ifstream& fin);

err_type exe_properties_general(std::ifstream& fin);
err_type exe_properties_MHD_All_trees(std::ifstream& fin);
err_type exe_properties_ExpVar_C(std::ifstream& fin);
err_type exe_properties_ExpVar_D(std::ifstream& fin);

err_type exe_linarr_approx_Exp_C(std::ifstream& fin);
err_type exe_linarr_C(std::ifstream& fin);
err_type exe_linarr_C_list(std::ifstream& fin);
err_type exe_linarr_D(const std::string& what, std::ifstream& fin);
err_type exe_linarr_headedness(std::ifstream& fin);
err_type exe_linarr_syntree_classification(std::ifstream& fin);
err_type exe_linarr_klevel(const std::string& level, const std::string& what, std::ifstream& fin);
err_type exe_linarr_Dmin_rooted(const std::string& algorithm, std::ifstream& fin);
err_type exe_linarr_Dmin_free(const std::string& algorithm, std::ifstream& fin);

err_type exe_gen_arr_all_proj(std::ifstream& fin);
err_type exe_gen_arr_rand_proj(std::ifstream& fin);
err_type exe_gen_trees_alf(std::ifstream& fin);
err_type exe_gen_trees_alr(std::ifstream& fin);
err_type exe_gen_trees_auf(std::ifstream& fin);
err_type exe_gen_trees_aur(std::ifstream& fin);
err_type exe_gen_trees_rlf(std::ifstream& fin);
err_type exe_gen_trees_rlr(std::ifstream& fin);
err_type exe_gen_trees_ruf(std::ifstream& fin);
err_type exe_gen_trees_rur(std::ifstream& fin);

err_type exe_utils_sorting(std::ifstream& fin);
err_type exe_utils_bfs(std::ifstream& fin);
err_type exe_utils_centre(std::ifstream& fin);
err_type exe_utils_centroid(std::ifstream& fin);

} // -- namespace exe_tests
