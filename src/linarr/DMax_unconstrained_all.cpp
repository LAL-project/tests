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
#include <fstream>

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/DMax.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/properties/branchless_paths_compute.hpp>

#include <lal/detail/linarr/level_signature.hpp>
#include <lal/detail/linarr/D/DMax/necessary_conditions.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

// linarr includes
#include "linarr/arrangement_check.hpp"
#include "linarr/test_optimum_algorithm_all.hpp"

typedef std::pair<uint64_t, lal::linear_arrangement> algo_result;

namespace tests {
namespace linarr {

err_type exe_linarr_DMax_Unconstrained_all(std::ifstream& fin) noexcept {
	const input_list inputs = read_input_list(fin);

	if (not std::filesystem::exists(inputs[0].first)) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Input file '" << inputs[0].first << "' does not exist.\n";
		return err_type::io;
	}
	std::ifstream input_file(inputs[0].first);


	typedef lal::graphs::free_tree free_tree;
	typedef lal::linear_arrangement arrgmnt;

	std::vector<lal::detail::level_signature_per_position> lev_sigs1;
	std::vector<lal::detail::level_signature_per_position> mlev_sigs1;
	std::vector<lal::detail::level_signature_per_position> lev_sigs2;
	std::vector<lal::properties::branchless_path> bps;

	const err_type r =
	many_arrangements::test_optimum_algorithm<free_tree>(
		// function to initialise a (free) tree
		[&](free_tree& t) {
			lev_sigs1.clear();
			mlev_sigs1.clear();
			lev_sigs2.clear();

			const uint64_t n = t.get_num_nodes();
			lev_sigs1.reserve(n);
			mlev_sigs1.reserve(n);
			lev_sigs2.reserve(n);

			bps = lal::properties::branchless_paths_compute(t);
		},
		// calculate result
		[&](const free_tree& t) {
			if (t.get_num_nodes() < 15) {
				return lal::linarr::max_sum_edge_lengths_all(t, 1);
			}
			else {
				return lal::linarr::max_sum_edge_lengths_all(t, 4);
			}
		},
		// evaluate input tree with an arrangement
		[](const free_tree& t, const arrgmnt& arr) {
			return lal::linarr::sum_edge_lengths(t, arr);
		},
		// check correctness of "arrangement"
		[&](const free_tree& t, const arrgmnt& arr) {
			const bool is_arr = lal::linarr::is_arrangement(t, arr);
			if (not is_arr) { return false; }

			auto L = lal::detail::calculate_level_signature
				<lal::detail::level_signature_type::per_position>(t, arr);

			mlev_sigs1.push_back(lal::detail::mirror_level_signature(L));

			const bool condition =
				lal::detail::is_level_signature_nonincreasing(t, L, arr) and
				lal::detail::no_two_adjacent_vertices_have_same_level(t, L, arr) and
				lal::detail::no_vertex_in_antenna_is_thistle(t, bps, L, arr) and
				lal::detail::at_most_one_thistle_in_bridges(t, bps, L, arr);

			// now we move L
			lev_sigs1.push_back(std::move(L));
			return condition;
		},
		// convert what you read from input to a (free) tree
		[](const std::vector<lal::node>& v) {
			return std::move(lal::graphs::from_head_vector_to_free_tree(v).first);
		},
		// isomorphism test
		[&](
			const free_tree& t,
			[[maybe_unused]] const arrgmnt& arr1, std::size_t i,
			const arrgmnt& arr2, std::size_t j
		)
		{
			if (j >= lev_sigs2.size()) {
				auto L = lal::detail::calculate_level_signature
						 <lal::detail::level_signature_type::per_position>(t, arr2);
				lev_sigs2.push_back(std::move(L));
			}

			return (lev_sigs1[i] == lev_sigs2[j]) or (mlev_sigs1[i] == lev_sigs2[j]);
		},
		// where to read from
		input_file
	);

	input_file.close();

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
