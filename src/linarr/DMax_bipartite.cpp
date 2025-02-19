/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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
#include "linarr/test_optimum_algorithm_one.hpp"

namespace lal {
namespace linarr {
enum class algorithms_DMax_bipartite {
	AlemanyEstebanFerrer
};
} // namespace linarr
} // namespace lal

namespace tests {
namespace linarr {

namespace dmax_bipartite {

template <class T>
err_type examine_dmax_bipartite(
	const std::string& filename,
	[[maybe_unused]] const lal::linarr::algorithms_DMax_bipartite& algo
) noexcept
{
	std::ifstream input_file(filename);
	if (not input_file.is_open()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Input file '" << filename
				  << "' could not be opened.\n";
		return err_type::io;
	}

	lal::properties::bipartite_graph_coloring c;

	return single_arrangement::test_optimum_algorithm<lal::graphs::free_tree>(
		// tree_initializer
		[&](const lal::graphs::free_tree& t)
		{
			c = lal::properties::bipartite_coloring(t);
		},
		// solver
		[&](const lal::graphs::free_tree& t)
		{
			return lal::linarr::max_sum_edge_lengths_bipartite(t, c);
		},
		// tree_eval
		[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr)
		{
			return lal::linarr::sum_edge_lengths(t, arr);
		},
		// arrgmnt_check
		[&](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr)
		{
			return lal::linarr::is_bipartite(t, c, arr);
		},
		// conv
		[](const lal::head_vector& v)
		{
			return lal::graphs::from_head_vector_to_free_tree(v).first;
		},
		input_file
	);
}

} // namespace dmax_bipartite

err_type exe_linarr_DMax_bipartite(std::ifstream& fin) noexcept
{
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
	lal::linarr::algorithms_DMax_bipartite algo_choice;
	if (algo == "AEF") {
		algo_choice =
			lal::linarr::algorithms_DMax_bipartite::AlemanyEstebanFerrer;
		algo_name = "AEF";
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Algorithm '" << algo << "' not handled.\n";
		return err_type::test_format;
	}

	const auto err1 =
		dmax_bipartite::examine_dmax_bipartite<lal::graphs::free_tree>(
			inputs[0].first, algo_choice
		);

	if (err1 != err_type::no_error) {
		return err1;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace linarr
} // namespace tests
