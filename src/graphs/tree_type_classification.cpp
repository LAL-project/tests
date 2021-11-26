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

// C++ includes
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/tree_type.hpp>
#include <lal/graphs/output.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/detail/graphs/tree_type.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace graphs {

namespace tree_type_class {

lal::graphs::tree_type string_to_tt(const std::string& s) {
	if (s == "empty") { return lal::graphs::tree_type::empty; }
	if (s == "singleton") { return lal::graphs::tree_type::singleton; }
	if (s == "linear") { return lal::graphs::tree_type::linear; }
	if (s == "star") { return lal::graphs::tree_type::star; }
	if (s == "quasistar") { return lal::graphs::tree_type::quasistar; }
	if (s == "bistar") { return lal::graphs::tree_type::bistar; }
	if (s == "caterpillar") { return lal::graphs::tree_type::caterpillar; }
	if (s == "spider") { return lal::graphs::tree_type::spider; }
	if (s == "unknown") { return lal::graphs::tree_type::unknown; }
	std::cerr << ERROR << '\n';
	std::cerr << "    String '" << s << "' could not be converted into a tree type.\n";
	return lal::graphs::tree_type::unknown;
}

std::vector<uint64_t> parse_treestr(const std::string& s) {
	// read numbers in line
	std::stringstream ss(s);
	std::vector<uint64_t> L;
	uint64_t v;
	while (ss >> v) { L.push_back(v); }
	return L;
}

std::vector<bool> parse_classes_tt(std::string s) {
	// classes vector
	std::vector<bool> classes(lal::graphs::__tree_type_size, false);
	bool read_sth = false;

	// parse classes in std::string
	std::replace(s.begin(), s.end(), ',', ' ');
	std::stringstream ss(s);
	std::string cls;
	while (ss >> cls) {
		const std::size_t idx = static_cast<std::size_t>(string_to_tt(cls));
		classes[idx] = true;
		read_sth = true;
	}

	if (not read_sth) {
		const std::size_t idx = static_cast<std::size_t>(string_to_tt("unknown"));
		classes[idx] = true;
	}
	return classes;
}

} // -- namespace tree_type_class

err_type exe_graphs_tree_type_classification(const input_list& inputs, std::ifstream& fin) {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::string line;
	std::size_t lineno = 3;
	getline(fin, line); // skip header

	while (getline(fin, line)) {
		++lineno;

		std::size_t semicolon = line.find(';');
		if (semicolon == std::string::npos) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Input line is malformed.\n";
			std::cerr << "    In line: " << lineno << "'.\n";
			std::cerr << "    Line '" << lineno << "' does not have the ';'.\n";
			return err_type::test_format;
		}

		// parse line
		const std::string treestr = line.substr(0, semicolon);
		const std::string classlist = line.substr(semicolon+1, line.length()-semicolon);

		// ground truth classification
		const std::vector<bool> ground_classes = tree_type_class::parse_classes_tt(classlist);

		// parse data in line
		const auto L = tree_type_class::parse_treestr(treestr);
		const auto P = lal::graphs::from_head_vector_to_free_tree(L);
		lal::graphs::rooted_tree rT(P.first, P.second);
		lal::graphs::free_tree fT = P.first;

		// each tree's classification
		rT.calculate_tree_type();
		std::vector<std::string> rClasses = rT.get_tree_type_list();
		sort(rClasses.begin(), rClasses.end());
		fT.calculate_tree_type();
		std::vector<std::string> fClasses = fT.get_tree_type_list();
		sort(fClasses.begin(), fClasses.end());

		// check result is correct
		if (rClasses != fClasses) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Classes found at the two trees are not the same.\n";
			std::cerr << "    Rooted tree:\n";
			for (const std::string& s : rClasses) {
			std::cerr << "        " << s
				 << (binary_search(fClasses.begin(), fClasses.end(), s) ?
						 "" : " <--- not in the free tree")
				 << '\n';
			}
			std::cerr << "    Free tree:\n";
			for (const std::string& s : rClasses) {
			std::cerr << "        " << s
				 << (binary_search(rClasses.begin(), rClasses.end(), s) ?
						 "" : " <--- not in the rooted tree")
				 << '\n';
			}
			return err_type::test_execution;
		}

		std::vector<bool> LAL_types(lal::graphs::__tree_type_size, false);
		for (const std::string& s : fClasses) {
			LAL_types[ static_cast<std::size_t>(tree_type_class::string_to_tt(s)) ] = true;
		}

		if (LAL_types != ground_classes) {
			std::cerr << ERROR << '\n';
			std::cerr << "    In line '" << lineno << "'.\n";
			std::cerr << "    Line's content: '" << line << "'\n";
			std::cerr << "    Tree:\n";
			std::cerr << fT << '\n';
			std::cerr << "    Ground truth classes:\n";
			for (std::size_t i = 0; i < ground_classes.size(); ++i) {
				if (ground_classes[i]) {
					std::cout << "        "
						 << lal::detail::tree_type_string(static_cast<lal::graphs::tree_type>(i))
						 << (not LAL_types[i] ? "  <--- missing" : "")
						 << '\n';
				}
			}
			std::cerr << "    LAL's classes:\n";
			for (std::size_t i = 0; i < LAL_types.size(); ++i) {
				if (LAL_types[i]) {
					std::cout << "        "
						 << lal::detail::tree_type_string(static_cast<lal::graphs::tree_type>(i))
						 << (not ground_classes[i] ? "  <--- incorrect" : "")
						 << '\n';
				}
			}
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace graphs
} // -- namespace tests
