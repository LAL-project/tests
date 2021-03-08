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
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/tree_type.hpp>
#include <lal/graphs/output.hpp>
#include <lal/internal/graphs/trees/convert_to_rtree.hpp>
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>
using namespace lal;
using namespace graphs;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"

namespace exe_tests {

namespace tree_type_class {

tree_type string_to_tt(const string& s) {
	if (s == "linear") { return tree_type::linear; }
	if (s == "star") { return tree_type::star; }
	if (s == "quasistar") { return tree_type::quasistar; }
	if (s == "bistar") { return tree_type::bistar; }
	if (s == "caterpillar") { return tree_type::caterpillar; }
	if (s == "spider") { return tree_type::spider; }
	if (s == "none") { return tree_type::none; }
	cerr << ERROR << endl;
	cerr << "    String '" << s << "' could not be converted into a tree type." << endl;
	return tree_type::none;
}

vector<uint32_t> parse_treestr(const string& s) {
	// read numbers in line
	stringstream ss(s);
	vector<uint32_t> L;
	uint32_t v;
	while (ss >> v) { L.push_back(v); }
	return L;
}

vector<bool> parse_classes_tt(string s) {
	// classes vector
	vector<bool> classes(lal::graphs::__tree_type_size, false);
	bool read_sth = false;

	// parse classes in string
	std::replace(s.begin(), s.end(), ',', ' ');
	stringstream ss(s);
	string cls;
	while (ss >> cls) {
		const size_t idx = static_cast<size_t>(string_to_tt(cls));
		classes[idx] = true;
		read_sth = true;
	}

	if (not read_sth) {
		const size_t idx = static_cast<size_t>(string_to_tt("none"));
		classes[idx] = true;
	}
	return classes;
}

} // -- namespace tree_type_class

err_type exe_graphs_tree_type_classification(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	string line;
	size_t lineno = 3;
	getline(fin, line); // skip header

	while (getline(fin, line)) {
		++lineno;

		size_t semicolon = line.find(';');
		if (semicolon == string::npos) {
			cerr << ERROR << endl;
			cerr << "    Input line is malformed." << endl;
			cerr << "    In line: " << lineno << "'." << endl;
			cerr << "    Line '" << lineno << "' does not have the ';'." << endl;
			return err_type::test_format;
		}

		// parse line
		const string treestr = line.substr(0, semicolon);
		const string classlist = line.substr(semicolon+1, line.length()-semicolon);

		// ground truth classification
		const vector<bool> ground_classes = tree_type_class::parse_classes_tt(classlist);

		// parse data in line
		const auto L = tree_type_class::parse_treestr(treestr);
		const auto P = lal::internal::head_vector_to_ftree(L);
		rooted_tree rT = rooted_tree(P.first, P.second);
		free_tree fT = P.first;

		// each tree's classification
		rT.calculate_tree_type();
		vector<string> rClasses = rT.get_tree_type_list();
		sort(rClasses.begin(), rClasses.end());
		fT.calculate_tree_type();
		vector<string> fClasses = fT.get_tree_type_list();
		sort(fClasses.begin(), fClasses.end());

		// check result is correct
		if (rClasses != fClasses) {
			cerr << ERROR << endl;
			cerr << "    Classes found at the two trees are not the same." << endl;
			cerr << "    Rooted tree:" << endl;
			for (const string& s : rClasses) {
			cerr << "        " << s
				 << (binary_search(fClasses.begin(), fClasses.end(), s) ?
						 "" : " <--- not in the free tree")
				 << endl;
			}
			cerr << "    Free tree:" << endl;
			for (const string& s : rClasses) {
			cerr << "        " << s
				 << (binary_search(rClasses.begin(), rClasses.end(), s) ?
						 "" : " <--- not in the rooted tree")
				 << endl;
			}
			return err_type::test_execution;
		}

		vector<bool> LAL_types(__tree_type_size, false);
		for (const string& s : fClasses) {
			LAL_types[ static_cast<size_t>(tree_type_class::string_to_tt(s)) ] = true;
		}

		if (LAL_types != ground_classes) {
			cerr << ERROR << endl;
			cerr << "    In line '" << lineno << "'." << endl;
			cerr << "    Line's content: '" << line << "'" << endl;
			cerr << "    Tree:" << endl;
			cerr << fT << endl;
			cerr << "    Ground truth classes:" << endl;
			for (size_t i = 0; i < ground_classes.size(); ++i) {
				if (ground_classes[i]) {
					cout << "        "
						 << graphs::tree_type_to_string(static_cast<tree_type>(i))
						 << (not LAL_types[i] ? "  <--- missing" : "")
						 << endl;
				}
			}
			cerr << "    LAL's classes:" << endl;
			for (size_t i = 0; i < LAL_types.size(); ++i) {
				if (LAL_types[i]) {
					cout << "        "
						 << graphs::tree_type_to_string(static_cast<tree_type>(i))
						 << (not ground_classes[i] ? "  <--- incorrect" : "")
						 << endl;
				}
			}
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
