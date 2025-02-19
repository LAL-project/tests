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

#pragma once

// C++ includes
#include <functional>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/output.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

// linarr includes
#include "linarr/test_optimum_algorithm.hpp"

namespace tests {
namespace linarr {

// Test the algorithms that return only ONE! arrangement
namespace many_arrangements {

template <class tree_t>
using Solver = std::function<
	std::pair<uint64_t, std::vector<lal::linear_arrangement>>(const tree_t&)>;

template <class tree_t>
using ArrgmntIso = std::function<bool(
	const tree_t&,
	const lal::linear_arrangement&,
	std::size_t,
	const lal::linear_arrangement&,
	std::size_t
)>;

template <
	class tree_t,
	class Solver_f,
	class ArrgmntEval_f,
	class ArrgmntCheck_f,
	class InputConv_f,
	class TreeInit_f,
	class ArrgmntIso_f>
err_type test_optimum_algorithm(
	// function that initializes the input tree
	const TreeInit_f& initialize_tree,
	// function that computes the solution
	const Solver_f& solver,
	// function that evaluates the input tree on a given arrangement
	const ArrgmntEval_f& arrgmnt_eval,
	// function that tests the correctness of the resulting arrangement
	const ArrgmntCheck_f& arrgmnt_check,
	// function that converts the input data into a tree
	const InputConv_f& conv,
	// function that tells whether or not two arrangements are isomorphic
	const ArrgmntIso_f& arrgmnt_iso,
	// the input stream
	std::ifstream& fin
) noexcept
{
	static_assert(std::is_constructible_v<Solver<tree_t>, Solver_f>);
	static_assert(std::is_constructible_v<ArrgmntEval<tree_t>, ArrgmntEval_f>);
	static_assert(std::
					  is_constructible_v<ArrgmntCheck<tree_t>, ArrgmntCheck_f>);
	static_assert(std::is_constructible_v<InputConv<tree_t>, InputConv_f>);
	static_assert(std::is_constructible_v<TreeInit<tree_t>, TreeInit_f>);
	static_assert(std::is_constructible_v<ArrgmntIso<tree_t>, ArrgmntIso_f>);

	// read number of nodes
	uint64_t n;
	fin >> n;

	if (n == 1) {
		// nothing to do
		return tests::err_type::no_error;
	}

	lal::head_vector tree_as_head_vector(n);
	while (fin >> tree_as_head_vector[0]) {
		// read head vector
		for (uint64_t i = 1; i < n; ++i) {
			fin >> tree_as_head_vector[i];
		}

		// construct tree
		tree_t tree = conv(tree_as_head_vector);
		initialize_tree(tree);

		// read value of D calculated by brute force
		uint64_t file_opt_value;
		fin >> file_opt_value;

		// read and check correctness of all arrangements
		std::size_t n_classes;
		fin >> n_classes;
		lal::detail::array<lal::linear_arrangement> file_arrs(n_classes);

		for (std::size_t c = 0; c < n_classes; ++c) {
			uint64_t mult;
			fin >> mult; // multiplicity
			char star;
			fin >> star; // *

			// arrangement
			file_arrs[c].resize(n);
			lal::position pu;
			for (lal::node u = 0; u < n; ++u) {
				fin >> pu;
				file_arrs[c].assign(u, pu);
			}

			// check correctness of the arrangement
			const uint64_t check_value = arrgmnt_eval(tree, file_arrs[c]);
			if (check_value != file_opt_value) {
				std::cerr << ERROR << '\n';
				std::cerr
					<< "    Input value (calculated by brute force) does not\n";
				std::cerr << "    agree with the evaluation of the tree at "
							 "said arrangement\n";
				std::cerr << "    calculated by brute force.\n";
				std::cerr << "        File arrangement:     "
						  << file_arrs[c].direct_as_vector() << '\n';
				std::cerr << "        File Inv Arrangement: "
						  << file_arrs[c].inverse_as_vector() << '\n';
				std::cerr << "        File value:           " << file_opt_value
						  << '\n';
				std::cerr << "        Evaluation at arrangement:   "
						  << check_value << '\n';
				std::cerr << "    For tree: \n";
				std::cerr << tree << '\n';
				std::cerr << "    Head vector: " << tree.get_head_vector()
						  << '\n';
				return tests::err_type::test_format;
			}
		}

		// execute library's algorithm
		const std::pair<uint64_t, std::vector<lal::linear_arrangement>>
			library_res = solver(tree);

		// check all algorithms actually yield the value 'library_res.first'
		for (const lal::linear_arrangement& library_arr : library_res.second) {

			// ensure that the arrangement is correctly built
			if (not arrgmnt_check(tree, library_arr)) {
				std::cerr << ERROR << '\n';
				std::cerr << "    The arrangement produced by the algorithm "
							 "does not pass the check.\n";
				std::cerr << "        Size: " << library_arr.size() << '\n';
				std::cerr << "        Arrangement:     "
						  << library_arr.direct_as_vector() << '\n';
				std::cerr << "        Inv Arrangement: "
						  << library_arr.inverse_as_vector() << '\n';
				std::cerr << "    For tree: \n";
				std::cerr << tree << '\n';
				std::cerr << "    Head vector: " << tree.get_head_vector()
						  << '\n';
				return tests::err_type::test_execution;
			}

			// ensure that value of D matches the evaluation of the arrangement
			const uint64_t check_value = arrgmnt_eval(tree, library_arr);
			if (check_value != library_res.first) {
				std::cerr << ERROR << '\n';
				std::cerr << "    The value calculated by the library's "
							 "algorithm does not\n";
				std::cerr << "    agree with the evaluation of the tree at the "
							 "arrangement\n";
				std::cerr << "    that the library's algorithm calculated.\n";
				std::cerr << "        Algorithm's Arrangement:     "
						  << library_arr.direct_as_vector() << '\n';
				std::cerr << "        Algorithm's Inv Arrangement: "
						  << library_arr.inverse_as_vector() << '\n';
				std::cerr << "        Algorithm's value:           "
						  << library_res.first << '\n';
				std::cerr << "        Evaluation at arrangement:   "
						  << check_value << '\n';
				std::cerr << "    For tree: \n";
				std::cerr << tree << '\n';
				std::cerr << "    Head vector: " << tree.get_head_vector()
						  << '\n';
				return tests::err_type::test_execution;
			}

			// ensure that the value of D is actually minimum
			if (library_res.first != file_opt_value) {
				std::cerr << ERROR << '\n';
				std::cerr << "    The value calculated by the library and by "
							 "bruteforce do not agree.\n";
				std::cerr << "    Library:\n";
				std::cerr << "        Value:           " << library_res.first
						  << '\n';
				std::cerr << "        Arrangement:     "
						  << library_arr.direct_as_vector() << '\n';
				std::cerr << "        Inv Arrangement: "
						  << library_arr.inverse_as_vector() << '\n';
				std::cerr << "    File:\n";
				std::cerr << "        Value:           " << file_opt_value
						  << '\n';
				std::cerr << "    For tree: \n";
				std::cerr << tree << '\n';
				std::cerr << "    Head vector: " << tree.get_head_vector()
						  << '\n';
				return tests::err_type::test_execution;
			}
		}

		// compare the result of the library with the data in the file
		if (library_res.second.size() != file_arrs.size()) {
			std::cerr << ERROR << '\n';
			std::cerr
				<< "    The library did not calculate all the arrangements.\n";
			std::cerr << "    Library:\n";
			std::cerr << "        Classes: " << library_res.second.size()
					  << '\n';
			for (std::size_t i = 0; i < library_res.second.size(); ++i) {
				const lal::linear_arrangement& arr = library_res.second[i];
				std::cerr << "        (" << i << ")\n";
				std::cerr << "            Arrangement:     "
						  << arr.direct_as_vector() << '\n';
				std::cerr << "            Inv Arrangement: "
						  << arr.inverse_as_vector() << '\n';
			}
			std::cerr << "    File:\n";
			std::cerr << "        Classes: " << file_arrs.size() << '\n';
			for (std::size_t i = 0; i < file_arrs.size(); ++i) {
				const lal::linear_arrangement& arr = file_arrs[i];
				std::cerr << "        (" << i << ")\n";
				std::cerr << "            Arrangement:     "
						  << arr.direct_as_vector() << '\n';
				std::cerr << "            Inv Arrangement: "
						  << arr.inverse_as_vector() << '\n';
			}
			std::cerr << "    For tree: \n";
			std::cerr << tree << '\n';
			std::cerr << "    Head vector: " << tree.get_head_vector() << '\n';
			return tests::err_type::test_execution;
		}

		lal::detail::array<char> matched(file_arrs.size(), 0);

		// make sure the arrangements are isomorphic in both sets are isomorphic
		for (std::size_t i = 0; i < library_res.second.size(); ++i) {
			const lal::linear_arrangement& arr1 = library_res.second[i];

			bool found = false;
			for (std::size_t j = 0; j < file_arrs.size(); ++j) {
				if (matched[j] == 1) {
					continue;
				}

				const lal::linear_arrangement& arr2 = file_arrs[j];
				if (arrgmnt_iso(tree, arr1, i, arr2, j)) {
					found = true;
					matched[j] = 1;
					break;
				}
			}

			if (not found) {
				std::cerr << ERROR << '\n';
				std::cerr
					<< "    Arrangement '" << i
					<< "' calculated by the library's algorithm could not be\n";
				std::cerr
					<< "    found in the list of arrangements in the file.\n";
				std::cerr << '\n';
				std::cerr << "    Library:\n";
				std::cerr << "        Classes: " << library_res.second.size()
						  << '\n';
				for (std::size_t k = 0; k < library_res.second.size(); ++k) {
					const lal::linear_arrangement& arr = library_res.second[k];
					std::cerr << "        (" << k << ")\n";
					std::cerr << "            Arrangement:     "
							  << arr.direct_as_vector() << '\n';
					std::cerr << "            Inv Arrangement: "
							  << arr.inverse_as_vector() << '\n';
				}
				std::cerr << "    File:\n";
				std::cerr << "        Classes: " << file_arrs.size() << '\n';
				for (std::size_t k = 0; k < file_arrs.size(); ++k) {
					const lal::linear_arrangement& arr = file_arrs[k];
					std::cerr << "        (" << k << ")\n";
					std::cerr << "            Arrangement:     "
							  << arr.direct_as_vector() << '\n';
					std::cerr << "            Inv Arrangement: "
							  << arr.inverse_as_vector() << '\n';
				}

				std::cerr << "    For tree: \n";
				std::cerr << tree << '\n';
				std::cerr << "Head vector: " << tree.get_head_vector() << '\n';
				return tests::err_type::test_execution;
			}
		}
	}
	return tests::err_type::no_error;
}

} // namespace many_arrangements

} // namespace linarr
} // namespace tests
