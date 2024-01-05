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

#include <lal/detail/linarr/level_signature.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

// linarr includes
#include "linarr/linarr_brute_force_testing.hpp"

namespace lal {
namespace linarr {
enum class algorithms_DMax_1_thistle{
	AlemanyEstebanFerrer
};
}
}

namespace tests {
namespace linarr {

namespace dmax_1_thistle{

template <class T>
err_type examine_dmax_1_thistle
(
	const std::string& filename,
	[[maybe_unused]] const lal::linarr::algorithms_DMax_1_thistle& algo
)
noexcept
{
	std::ifstream input_file(filename);
	if (not input_file.is_open()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Input file '" << filename << "' could not be opened.\n";
		return err_type::io;
	}

	lal::properties::bipartite_graph_coloring c;

	const auto err = linarr_brute_force_testing<lal::graphs::free_tree>
	(
		// solver
		[&](const lal::graphs::free_tree& t) {
			return lal::linarr::max_sum_edge_lengths_1_thistle(t, c);
		},
		// tree_eval
		[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
			return lal::linarr::sum_edge_lengths(t, arr);
		},
		// arrgmnt_check
		[&](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
			const auto n = t.get_num_nodes();
			lal::linarr::level_signature_per_vertex lev_seq(n);
			lal::detail::calculate_level_signature(t, arr, lev_seq);

			std::size_t num_thistles = 0;
			for (lal::node_t u = 0ull; u < n; ++u) {
				num_thistles += lal::linarr::is_thistle_vertex(t, arr, lev_seq, u);
			}
			return num_thistles == 1;
		},
		// conv
		[](const lal::head_vector& v) {
			return lal::graphs::from_head_vector_to_free_tree(v).first;
		},
		// tree_initializer
		[&](const lal::graphs::free_tree& t) {
			c = lal::properties::coloring(t);
		},
		input_file
	);
	return err;
}

} // -- namespace dmax_1_thistle

err_type exe_linarr_DMax_1_thistle(std::ifstream& fin) noexcept {
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
	lal::linarr::algorithms_DMax_1_thistle algo_choice;
	if (algo == "AEF") {
		algo_choice = lal::linarr::algorithms_DMax_1_thistle::AlemanyEstebanFerrer;
		algo_name = "AEF";
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Algorithm '" << algo << "' not handled.\n";
		return err_type::test_format;
	}

	const auto err1 =
		dmax_1_thistle::examine_dmax_1_thistle<lal::graphs::free_tree>
		(inputs[0].first, algo_choice);

	if (err1 != err_type::no_error) { return err1; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
