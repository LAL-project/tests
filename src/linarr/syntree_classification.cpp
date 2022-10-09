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

// C++ includes
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/syntactic_dependency_structure.hpp>
#include <lal/linarr/classify_syntactic_dependency_structure.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/detail/make_array.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace linarr {

namespace syntree_class {

typedef lal::linarr::syntactic_dependency_structure syndepstr_type;

std::string sdtt_to_string(const syndepstr_type& t) noexcept {
	switch (t) {
	case syndepstr_type::projective: return "prj";
	case syndepstr_type::planar: return "pla";
	case syndepstr_type::WG1: return "wg1";
	case syndepstr_type::EC1: return "1ec";
	default: return "none";
	}
}

std::pair<syndepstr_type, bool> std_to_syntreetype(const std::string& s) noexcept {
	if (s == "prj") { return std::make_pair(syndepstr_type::projective, true); }
	if (s == "pla") { return std::make_pair(syndepstr_type::planar, true); }
	if (s == "1ec") { return std::make_pair(syndepstr_type::EC1, true); }
	if (s == "wg1") { return std::make_pair(syndepstr_type::WG1, true); }
	if (s == "mh4") { return std::make_pair(syndepstr_type::unknown, false); }
	if (s == "mh5") { return std::make_pair(syndepstr_type::unknown, false); }

	return std::make_pair(syndepstr_type::unknown, true);
}

lal::graphs::rooted_tree parse_tree_in_line(const std::string& s) noexcept {
	// read numbers in line
	std::stringstream ss(s);
	std::vector<uint64_t> L;
	uint64_t v;
	while (ss >> v) { L.push_back(v); }
	return lal::graphs::from_head_vector_to_rooted_tree(L);
}

std::array<bool, lal::linarr::__syntactic_dependency_structure_size>
parse_ground_classes(std::string s)
noexcept
{
	// classes vector
	auto classes =
		lal::detail::make_array_with_value<bool, lal::linarr::__syntactic_dependency_structure_size, false>();

	if (s.length() == 0) {
		const syndepstr_type sdtt = std_to_syntreetype("none").first;
		classes[static_cast<std::size_t>(sdtt)] = true;
		return classes;
	}

	// parse classes in std::string
	int n_accepted_classes = 0;
	std::replace(s.begin(), s.end(), ',', ' ');
	std::stringstream ss(s);
	std::string cls;
	while (ss >> cls) {
		const auto [sdtt, accept] = std_to_syntreetype(cls);
		if (accept) {
			const std::size_t idx = static_cast<std::size_t>(sdtt);
			classes[idx] = true;
			++n_accepted_classes;
		}
	}
	if (n_accepted_classes == 0) {
		const syndepstr_type sdtt = std_to_syntreetype("none").first;
		classes[static_cast<std::size_t>(sdtt)] = true;
	}
	return classes;
}

err_type parse_single_file(const std::string& file) noexcept {

	if (not std::filesystem::exists(file)) {
		std::cerr << ERROR << '\n';
		std::cerr << "    File '" << file << "' does not exist.\n";
		return err_type::io;
	}

	std::ifstream F;
	F.open(file);

	std::string line;
	std::size_t lineno = 1;
	getline(F, line); // skip header

	while (getline(F, line)) {
		++lineno;

		std::size_t semicolon = line.find(';');
		if (semicolon == std::string::npos) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Input line is not correctly formatted.\n";
			std::cerr << "    In line: " << lineno << "' of file '" << file << "'.\n";
			std::cerr << "    Line '" << lineno << "' does not have the ';'.\n";
			return err_type::test_format;
		}

		// parse line
		const std::string treestr = line.substr(0, semicolon);
		const std::string classlist = line.substr(semicolon+1, line.length()-semicolon);

		// parse data in line
		const lal::graphs::rooted_tree T = parse_tree_in_line(treestr);
		const auto ground_classes = parse_ground_classes(classlist);

		// classify tree
		const auto LAL_classes = lal::linarr::syntactic_dependency_structure_class(T);

		// check result is correct
		if (LAL_classes != ground_classes) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Classes detected by LAL are not a subset of the actual classes.\n";
			std::cerr << "    In line '" << lineno << "' of file '" << file << "'.\n";
			std::cerr << "    Line's content: " << line << '\n';
			std::cerr << "    Ground truth classes:\n";
			for (std::size_t i = 0; i < ground_classes.size(); ++i) {
				if (ground_classes[i]) {
					std::cout << "        "
						 << sdtt_to_string(static_cast<syndepstr_type>(i))
						 << (not LAL_classes[i] ? "  <--- missing" : "")
						 << '\n';
				}
			}
			std::cerr << "    LAL's classes:\n";
			for (std::size_t i = 0; i < LAL_classes.size(); ++i) {
				if (LAL_classes[i]) {
					std::cout << "        "
						 << sdtt_to_string(static_cast<syndepstr_type>(i))
						 << (not ground_classes[i] ? "  <--- incorrect" : "")
						 << '\n';
				}
			}
			return err_type::test_execution;
		}
	}

	return err_type::no_error;
}

} // -- namespace syntree_class

err_type exe_linarr_syntree_classification(std::ifstream& fin) noexcept {

	std::string f;
	while (fin >> f) {
		const err_type e = syntree_class::parse_single_file(f);
		if (e != err_type::no_error) {
			// the complete error message is already
			// issued inside the function "parse_files"
			return e;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
