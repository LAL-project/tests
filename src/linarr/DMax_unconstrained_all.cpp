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
#include <filesystem>
#include <fstream>

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/DMax.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/graphs/conversions.hpp>

#include <lal/detail/linarr/level_signature.hpp>

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

	std::vector<lal::detail::level_signature_per_vertex> lev_sigs1;
	std::vector<lal::detail::level_signature_per_vertex> mlev_sigs1;
	std::vector<lal::detail::level_signature_per_vertex> lev_sigs2;

	const err_type r =
	many_arrangements::test_optimum_algorithm<free_tree>(
		// calculate result
		[&](const free_tree& t) {
			if (t.get_num_nodes() < 15) {
				return lal::linarr::max_sum_edge_lengths_all(t);
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
		[](const free_tree& t, const arrgmnt& arr) {
			return lal::linarr::is_arrangement(t, arr);
		},
		// convert what you read from input to a (free) tree
		[](const std::vector<lal::node>& v) {
			return std::move(lal::graphs::from_head_vector_to_free_tree(v).first);
		},
		// function to initialise a (free) tree
		[&](free_tree& t) {
			lev_sigs1.clear();
			mlev_sigs1.clear();
			lev_sigs2.clear();

			lev_sigs1.reserve(t.get_num_nodes());
			mlev_sigs1.reserve(t.get_num_nodes());
			lev_sigs2.reserve(t.get_num_nodes());
		},
		// isomorphism test
		[&](
			const free_tree& t,
			const arrgmnt& arr1, std::size_t idx1,
			const arrgmnt& arr2, std::size_t idx2
		)
		{
			if (idx1 >= lev_sigs1.size()) {
				auto L = lal::detail::calculate_level_signature
						 <lal::detail::level_signature_type::per_vertex>(t, arr1);

				lev_sigs1.push_back(std::move(L));
				mlev_sigs1.push_back(lal::detail::mirror_level_signature(lev_sigs1.back()));
			}
			if (idx2 >= lev_sigs2.size()) {
				auto L = lal::detail::calculate_level_signature
						 <lal::detail::level_signature_type::per_vertex>(t, arr2);
				lev_sigs2.push_back(std::move(L));
			}

			return  (lev_sigs1[idx1] == lev_sigs2[idx2]) or
				   (mlev_sigs1[idx1] == lev_sigs2[idx2]);
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