/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
using namespace std;

// custom includes
#include "definitions.hpp"

namespace exe_tests {

err_type exe_construction(ifstream& fin);

err_type exe_numeric_integer(ifstream& fin);
err_type exe_numeric_rational_integer(ifstream& fin);
err_type exe_numeric_rational_rational(ifstream& fin);

err_type exe_properties_general(ifstream& fin);
err_type exe_properties_MHD_All_trees(ifstream& fin);
err_type exe_properties_ExpVar_C(ifstream& fin);
err_type exe_properties_ExpVar_D(ifstream& fin);
err_type exe_properties_approx_Exp_C(ifstream& fin);

err_type exe_linarr_compute_C(ifstream& fin);
err_type exe_linarr_compute_C_list(ifstream& fin);
err_type exe_linarr_compute_D(ifstream& fin);
err_type exe_linarr_compute_headedness(ifstream& fin);
err_type exe_linarr_syn_dep_tree_type(ifstream& fin);

err_type exe_gen_trees(ifstream& fin);

err_type exe_utils_sorting(ifstream& fin);
err_type exe_utils_bfs(ifstream& fin);

} // -- namespace exe_tests
