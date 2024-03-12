/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <fstream>
#include <set>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/DMax.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/graphs/conversions.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

// linarr includes
#include "linarr/linarr_brute_force_testing.hpp"

namespace lal {
namespace linarr {
enum class algorithms_DMax_projective {
	AlemanyEstebanFerrer
};
}
}

namespace tests {
namespace linarr {

namespace dmax_projective {

template <class T>
err_type examine_DMax_projective
(
	const std::string& filename,
	const TreeInit<T>& tree_initializer,
	[[maybe_unused]] const lal::linarr::algorithms_DMax_projective& algo
)
noexcept
{
	std::ifstream input_file(filename);
	if (not input_file.is_open()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Input file '" << filename << "' could not be opened.\n";
		return err_type::io;
	}

	return single_arrangement::test_optimum_algorithm<lal::graphs::rooted_tree>
	(
		[&](const lal::graphs::rooted_tree& t) {
			return lal::linarr::max_sum_edge_lengths_projective(t);
		},
		[](const lal::graphs::rooted_tree& t, const lal::linear_arrangement& arr) {
			return lal::linarr::sum_edge_lengths(t, arr);
		},
		[](const lal::graphs::rooted_tree& t, const lal::linear_arrangement& arr) {
			return lal::linarr::is_projective(t, arr);
		},
		[](const lal::head_vector& v) {
			return lal::graphs::from_head_vector_to_rooted_tree(v);
		},
		tree_initializer,
		input_file
	);
}

} // -- namespace dmax_projective

err_type exe_linarr_DMax_projective(std::ifstream& fin) noexcept {
	const input_list inputs = read_input_list(fin);

	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Exactly one input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	const std::set<std::string> allowed_algos({"AEF"});

	std::string algo;
	fin >> algo;

	if (allowed_algos.find(algo) == allowed_algos.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unrecognized algorithm '" << algo << "'.\n";
		std::cerr << "    Allowed algorithms:\n";
		for (const auto& s : allowed_algos) {
		std::cerr << "    - " << s << '\n';
		}
		return err_type::test_format;
	}

	std::string algo_name;
	lal::linarr::algorithms_DMax_projective algo_choice;
	if (algo == "AEF") {
		algo_choice = lal::linarr::algorithms_DMax_projective::AlemanyEstebanFerrer;
		algo_name = "AEF";
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Algorithm '" << algo << "' not handled.\n";
		return err_type::test_format;
	}

	const auto err1 =
	dmax_projective::examine_DMax_projective<lal::graphs::rooted_tree>
	(
		inputs[0].first,
		[](lal::graphs::rooted_tree& t) {
			t.calculate_size_subtrees();
		},
		algo_choice
	);
	if (err1 != err_type::no_error) { return err1; }

	// do not calculate size subtrees so as to be able to test it
	const auto err2 =
	dmax_projective::examine_DMax_projective<lal::graphs::rooted_tree>
	(
		inputs[0].first,
		[](lal::graphs::rooted_tree&) { },
		algo_choice
	);
	if (err2 != err_type::no_error) { return err2; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
