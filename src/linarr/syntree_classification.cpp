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

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/tree_structure_class.hpp>
#include <lal/linarr/tree_structure.hpp>
#include <lal/internal/graphs/trees/convert_to_rtree.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"

namespace exe_tests {

string sdtt_to_string(const tree_structure& t) {
	switch (t) {
	case tree_structure::projective: return "prj";
	case tree_structure::planar: return "pla";
	case tree_structure::WG1: return "wg1";
	case tree_structure::EC1: return "1ec";
	case tree_structure::MH4: return "mh4";
	case tree_structure::MH5: return "mh5";
	case tree_structure::none: return "?";
	}
	// so compiler does not cry
	return "?";
}

tree_structure string_to_sdtt(const string& s) {
	if (s == "prj") { return tree_structure::projective; }
	if (s == "pla") { return tree_structure::planar; }
	if (s == "1ec") { return tree_structure::EC1; }
	if (s == "wg1") { return tree_structure::WG1; }
	if (s == "mh4") { return tree_structure::MH4; }
	if (s == "mh5") { return tree_structure::MH5; }
	cerr << ERROR << endl;
	cerr << "    String could not be converted to LAL's tree_structure type." << endl;
	cerr << "    Input string: " << s << endl;
	return tree_structure::none;
}

rtree parse_tree_in_line(const string& s) {
	// read numbers in line
	stringstream ss(s);
	vector<uint32_t> L(1, 0);
	uint32_t v;
	while (ss >> v) { L.push_back(v); }

	const uint32_t n = static_cast<uint32_t>(L.size() - 1);
	return lal::internal::linear_sequence_to_tree(L, n);
}

vector<bool> parse_classes(string s) {
	// classes vector
	vector<bool> classes(lal::linarr::__tree_structure_size, false);
	if (s.length() == 0) {
		const size_t idx = static_cast<size_t>(string_to_sdtt("none"));
		classes[idx] = true;
		return classes;
	}

	// parse classes in string
	std::replace(s.begin(), s.end(), ',', ' ');
	stringstream ss(s);
	string cls;
	while (ss >> cls) {
		const size_t idx = static_cast<size_t>(string_to_sdtt(cls));
		classes[idx] = true;
	}
	return classes;
}

err_type parse_single_file(const string& file) {

	if (not filesystem::exists(file)) {
		cerr << ERROR << endl;
		cerr << "    File '" << file << "' does not exist." << endl;
		return err_type::io_error;
	}

	ifstream F;
	F.open(file);

	string line;
	size_t lineno = 1;
	getline(F, line); // skip header

	while (getline(F, line)) {
		++lineno;

		size_t semicolon = line.find(';');
		if (semicolon == string::npos) {
			cerr << ERROR << endl;
			cerr << "    Input line is not correctly formatted." << endl;
			cerr << "    In line: " << lineno << "' of file '" << file << "'." << endl;
			cerr << "    Line '" << lineno << "' does not have the ';'." << endl;
			return err_type::test_format_error;
		}

		// parse line
		const string treestr = line.substr(0, semicolon);
		const string classlist = line.substr(semicolon+1, line.length()-semicolon);

		// parse data in line
		const rtree T = parse_tree_in_line(treestr);
		const vector<bool> ground_classes = parse_classes(classlist);

		// classify tree
		const vector<bool> LAL_classes = lal::linarr::classify_tree_structure(T);

		// check result is correct
		if (not (LAL_classes <= ground_classes)) {
			cerr << ERROR << endl;
			cerr << "    Classes detected by LAL are not a subset of the actual classes." << endl;
			cerr << "    In line '" << lineno << "' of file '" << file << "'." << endl;
			cerr << "    Line's content: " << line << endl;
			cerr << "    Actual classes:" << endl;
			for (size_t i = 0; i < ground_classes.size(); ++i) {
				if (ground_classes[i]) {
					cout << "        "
						 << sdtt_to_string(static_cast<lal::linarr::tree_structure>(i))
						 << endl;
				}
			}
			cerr << "    LAL's classes:" << endl;
			for (size_t i = 0; i < LAL_classes.size(); ++i) {
				if (LAL_classes[i]) {
					cout << "        "
						 << sdtt_to_string(static_cast<lal::linarr::tree_structure>(i))
						 << (not ground_classes[i] ? "  <--- incorrect" : "")
						 << endl;
				}
			}
			return err_type::test_exe_error;
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_syntree_classification(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	string f;
	while (fin >> f) {
		const err_type e = parse_single_file(f);
		if (e != err_type::no_error) {
			// the complete error message is already
			// issued inside the function "parse_files"
			return e;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
