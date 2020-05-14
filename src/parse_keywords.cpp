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

#include "parse_keywords.hpp"

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// custom includes
#include "exe_tests.hpp"

namespace exe_tests {

void mark_wrong_keyword
(const vector<string>& keywords,
 const vector<size_t>& k, const string& tab)
{
	cerr << tab;
	// sure 'keywords' has at least one keyword
	for (size_t i = 0; i < keywords.size(); ++i) {
		if (keywords[i].length() >= 2) {
			cerr << keywords[i] << " ";
		}
		else {
			cerr << keywords[i] << "  ";
		}
	}
	cerr << endl;

	// display the /\ where needed
	size_t it = 0;
	cerr << tab;
	for (size_t i = 0; i < keywords.size(); ++i) {
		size_t l = keywords[i].length();
		if (it < k.size()) {
			if (k[it] != i) {
				cerr << string(l, ' ') << " ";
				if (l < 2) { cerr << " "; }
			}
			else {
				cerr << "/\\";
				if (l > 2) {
					cerr << string(l - 2, ' ') << " ";
				}
				else {
					cerr << "   ";
				}
				++it;
			}
		}
	}
	cerr << endl;
}

err_type call_main(const vector<string>& keywords, ifstream& fin) {
	const string& key = keywords[0];
	if (key == "construction_graph") {
		// no more keywords
		return exe_construction(fin);
	}
	if (key == "numeric") {
		return call_numeric(keywords, 1, fin);
	}
	if (key == "properties") {
		return call_properties(keywords, 1, fin);
	}
	if (key == "linarr") {
		return call_linarr(keywords, 1, fin);
	}
	if (key == "generate") {
		return call_generate(keywords, 1, fin);
	}
	if (key == "utils") {
		return call_utils(keywords, 1, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at 0: '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {0}, "    ");
	return err_type::wrong_keyword;
}

/* Functions to test the integer and rational classes */

err_type call_numeric(const vector<string>& keywords, size_t i, ifstream& fin) {
	const string& num_type1 = keywords[i];
	if (num_type1 != "integer" and num_type1 != "rational") {
		cerr << ERROR << endl;
		cerr << "    Wrong keyword at " << i << ": '" << num_type1 << "'." << endl;
		mark_wrong_keyword(keywords, {i}, "    ");
		return err_type::wrong_keyword;
	}

	if (num_type1 == "integer") {
		return exe_numeric_integer(fin);
	}

	if (num_type1 == "rational") {
		return exe_numeric_rational(fin);
	}

	cerr << ERROR << endl;
	cerr << "    Wrong keyword at " << i << ": '" << num_type1 << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

/* Functions to test the properties functions */

err_type call_properties(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "general") {
		return exe_properties_general(fin);
	}
	if (key == "MHD_All_Trees") {
		return exe_properties_MHD_All_trees(fin);
	}
	if (key == "exp_var_C") {
		return exe_properties_ExpVar_C(fin);
	}
	if (key == "exp_var_D") {
		return exe_properties_ExpVar_D(fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

/* Functions to test the linear arrangement-related functions */

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
		return call_linarr_D_related(keywords, i + 1, fin);
	}
	if (key == "Dmin") {
		return call_linarr_Dmin(keywords, i + 1, fin);
	}


	if (key == "approx_exp_C") {
		return exe_linarr_approx_Exp_C(fin);
	}
	if (key == "compute_headedness") {
		return exe_linarr_headedness(fin);
	}
	if (key == "syntree_classification") {
		return exe_linarr_syntree_classification(fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_linarr_C
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	// There is no need to give more keywords.
	// If, after this keyword, there are no more of them,
	// execute the simple test.
	if (i == keywords.size()) {
		return exe_linarr_C(fin);
	}

	const string& key = keywords[i];
	if (key == "list") {
		return exe_linarr_C_list(fin);
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

	return exe_linarr_klevel(level, key, fin);
}

err_type call_linarr_D_related
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const set<string> allowed_keywords({"D", "MDD"});

	const string& key = keywords[i];
	if (allowed_keywords.find(key) == allowed_keywords.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong keyword at " << i << ": '" << key << "'." << endl;
		mark_wrong_keyword(keywords, {i}, "    ");
		return err_type::wrong_keyword;
	}

	return exe_linarr_D(key, fin);
}

err_type call_linarr_Dmin
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const set<string> allowed_keywords({"Projective", "Unconstrained_YS", "Unconstrained_FC"});
	// Dmin algorithms for rooted trees
	const set<string> rooted_algorithms({"Projective"});
	// Dmin algorithms for free trees
	const set<string> free_algorithms({"Unconstrained_YS", "Unconstrained_FC"});

	const string& key = keywords[i];
	if (allowed_keywords.find(key) == allowed_keywords.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong keyword at " << i << ": '" << key << "'." << endl;
		mark_wrong_keyword(keywords, {i}, "    ");
		return err_type::wrong_keyword;
	}

	if (rooted_algorithms.find(key) != rooted_algorithms.end()) {
		return exe_linarr_Dmin_rooted(key, fin);
	}
	if (free_algorithms.find(key) != free_algorithms.end()) {
		return exe_linarr_Dmin_free(key, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Key '" << key << "' is not classified as an algorithm for" << endl;
	cerr << "    free trees or for rooted trees." << endl;
	return err_type::not_implemented;
}

/* Functions to test the tree generation functions and classes */

err_type call_generate
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "trees") {
		return call_generate_trees(keywords, i + 1, fin);
	}
	else if (key == "arrangements") {
		return call_generate_arrangements(keywords, i + 1, fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_generate_trees
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "alf") {
		return exe_gen_trees_alf(fin);
	}
	else if (key == "auf") {
		return exe_gen_trees_auf(fin);
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

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

err_type call_generate_arrangements
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "all_proj") {
		return exe_gen_arr_ap(fin);
	}
	else if (key == "rand_proj") {
		return exe_gen_arr_rp(fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

/* Functions to test the library's utilities */

err_type call_utils
(const vector<string>& keywords, size_t i, ifstream& fin)
{
	const string& key = keywords[i];
	if (key == "sorting") {
		return exe_utils_sorting(fin);
	}
	if (key == "traversal") {
		return exe_utils_bfs(fin);
	}
	if (key == "centre") {
		return exe_utils_centre(fin);
	}

	cerr << ERROR << endl;
	cerr << "    Unhandled keyword at " << i << ": '" << key << "'." << endl;
	mark_wrong_keyword(keywords, {i}, "    ");
	return err_type::wrong_keyword;
}

} // -- namespace exe_tests
