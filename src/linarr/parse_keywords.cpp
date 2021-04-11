/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// common includes
#include "common/parse_keywords.hpp"
#include "common/parse_header.hpp"
#include "linarr/parse_keywords.hpp"
#include "linarr/exe_tests.hpp"

namespace tests {
namespace linarr {

err_type call_linarr_Dmin
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "Projective") {
		return parse_header(exe_linarr_Dmin_projective, fin);
	}
	if (key == "Planar") {
		return parse_header(exe_linarr_Dmin_planar, fin);
	}
	if (key == "Unconstrained") {
		return parse_header(exe_linarr_Dmin_unconstrained, fin);
	}
	if (key == "comparison") {
		return parse_header(exe_linarr_Dmin_comparison, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_linarr_klevel
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const set<string> allowed_levels({"1", "2"});
	const set<string> allowed_keywords({"MDD"});

	const string& level = keywords[i];
	bool correct_level = allowed_levels.find(level) != allowed_levels.end();
	const string& key = keywords[i + 1];
	bool correct_key = allowed_keywords.find(key) != allowed_keywords.end();

	if (not correct_level or not correct_key) {
		cerr << ERROR << endl;
		vector<size_t> what_keys;
		if (not correct_level) {
			cerr << "    Wrong keyword at " << i << ": '" << level << "'." << endl;
			what_keys.push_back(i);
		}
		if (not correct_key) {
			cerr << "    Wrong keyword at " << i + 1 << ": '" << key << "'." << endl;
			what_keys.push_back(i + 1);
		}
		mark_wrong_keyword(keywords, what_keys, "    ");
		return err_type::wrong_keyword;
	}

	return parse_header(exe_linarr_klevel, fin);
}

err_type call_linarr_C
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	// There is no need to give more keywords.
	// If, after this keyword, there are no more of them,
	// execute the simple test.
	if (i == keywords.size()) {
		return parse_header(exe_linarr_C, fin, false);
	}

	const string& key = keywords[i];
	if (key == "list") {
		return parse_header<char>(exe_linarr_C_list, fin, 0);
	}
	if (key == "upper_bound") {
		return parse_header(exe_linarr_C, fin, true);
	}
	if (key == "list_upper_bound") {
		return parse_header<char>(exe_linarr_C_list, fin, 1);
	}
	if (key == "list_upper_bound_list") {
		return parse_header<char>(exe_linarr_C_list, fin, 2);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_linarr
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "compute_C") {
		return call_linarr_C(keywords, i + 1, fin);
	}
	if (key == "klevel") {
		return call_linarr_klevel(keywords, i + 1, fin);
	}
	if (key == "D") {
		return parse_header(exe_linarr_D, fin);
	}
	if (key == "Dmin") {
		return call_linarr_Dmin(keywords, i + 1, fin);
	}

	if (key == "dependency_flux") {
		return parse_header(exe_linarr_dependency_flux, fin);
	}
	if (key == "approx_exp_C") {
		return parse_header(exe_linarr_approx_Exp_C, fin);
	}
	if (key == "compute_headedness") {
		return parse_header(exe_linarr_headedness, fin);
	}
	if (key == "syntree_classification") {
		return parse_header(exe_linarr_syntree_classification, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_main(const vector<string>& keywords, ifstream& fin) {
	const string& key = keywords[0];
	if (key == "linarr") {
		return call_linarr(keywords, 1, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at 0: '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

} // -- namespace linarr
} // -- namespace tests
