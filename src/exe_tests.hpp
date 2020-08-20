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
#include <functional>
#include <fstream>
#include <vector>
#include <string>

// custom includes
#include "definitions.hpp"

typedef std::vector<std::pair<std::string,std::string>> input_list;

namespace exe_tests {

err_type execute_test_function(
	const std::function<err_type (input_list, std::ifstream&)> F,
	const input_list& inputs,
	std::ifstream& fin
);

err_type parse_header(
	const std::function<err_type (input_list, std::ifstream&)> F,
	std::ifstream& fin
);

err_type exe_graphs_construction(const input_list& inputs, std::ifstream& fin);

err_type exe_numeric_integer(const input_list& inputs,std::ifstream& fin);
err_type exe_numeric_rational(const input_list& inputs,std::ifstream& fin);

err_type exe_properties_general(const input_list& inputs,std::ifstream& fin);
err_type exe_properties_MHD_All_trees(const input_list& inputs,std::ifstream& fin);
err_type exe_properties_ExpVar_C(const input_list& inputs,std::ifstream& fin);
err_type exe_properties_ExpVar_D(const input_list& inputs,std::ifstream& fin);

err_type exe_linarr_approx_Exp_C(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_C(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_C_list(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_D(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_headedness(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_syntree_classification(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_klevel(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_projective(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_planar(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_unconstrained(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_comparison(const input_list& inputs, std::ifstream& fin);

err_type exe_gen_arr_all_proj(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_arr_rand_proj(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_alf(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_alr(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_auf(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_aur(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_rlf(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_rlr(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_ruf(const input_list& inputs, std::ifstream& fin);
err_type exe_gen_trees_rur(const input_list& inputs, std::ifstream& fin);

err_type exe_utilities_tree_isomorphism(const input_list& inputs, std::ifstream& fin);

err_type exe_internal_sorting(const input_list& inputs, std::ifstream& fin);
err_type exe_internal_bfs(const input_list& inputs, std::ifstream& fin);
err_type exe_internal_centre(const input_list& inputs, std::ifstream& fin);
err_type exe_internal_centroid(const input_list& inputs, std::ifstream& fin);

} // -- namespace exe_tests
