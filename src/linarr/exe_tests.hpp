/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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
namespace linarr {

err_type exe_linarr_approx_Exp_C(const input_list& inputs, std::ifstream& fin);
// upper_bound: is there an upper bound after every arrangement?
err_type exe_linarr_C(const input_list& inputs,std::ifstream& fin, bool has_upper_bound);
// upper_bound: type of upper_bound
//	0: no upper bound at all
//	1: an upper bound at the end of the file
//	2: an upper bound for every arrangement
err_type exe_linarr_C_list(const input_list& inputs, std::ifstream& fin, char upper_bound_kind);
err_type exe_linarr_D(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_headedness(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_syntree_classification(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_linear_arrangement(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_dependency_flux(const input_list& inputs,std::ifstream& fin);
err_type exe_linarr_klevel(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_projective(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_planar(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_unconstrained(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_Dmin_comparison(const input_list& inputs, std::ifstream& fin);
err_type exe_linarr_arrangement_validity(const input_list& inputs, std::ifstream& fin);

} // -- namespace linarr
} // -- namespace tests
