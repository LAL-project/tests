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
 ********************************************************************/

// C++ includes
#include <optional>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>

#include <lal/detail/utilities/tree_isomorphism.hpp>
#include <lal/detail/utilities/tree_isomorphism_string.hpp>
#include <lal/detail/utilities/tree_isomorphism_tuple.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

// utilities includes
#include "utilities/isomorphism_utils.hpp"

#define to_uint64(x) static_cast<uint64_t>(x)

static constexpr auto string_algo = lal::detail::isomorphism::algorithm::string;
static constexpr auto tuple_algo = lal::detail::isomorphism::algorithm::tuple;

namespace tests {
namespace utilities {

namespace manual {

template <lal::detail::isomorphism::algorithm algo, bool rooted>
err_type rooted_test(std::ifstream& fin) noexcept
{
	const auto sbi = read_should_be_or_not(fin);
	if (not sbi.has_value()) {
		return err_type::test_format;
	}
	const bool expected_isomorphism = *sbi;

	uint64_t n;
	while (fin >> n) {
		lal::graphs::rooted_tree t1(n);
		read_rooted(fin, t1);

		lal::graphs::rooted_tree t2(n);
		read_rooted(fin, t2);

		const bool calculated_isomorphism =
			lal::detail::are_trees_isomorphic<algo, false>(t1, t2);

		std::cout << "Are isomorphic? " << std::boolalpha
				  << calculated_isomorphism << '\n';

		if (expected_isomorphism != calculated_isomorphism) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Library function result: "
					  << calculated_isomorphism << '\n';
			std::cerr << "    Expected result: " << expected_isomorphism
					  << '\n';
			std::cerr << "Tree 1:\n";
			std::cerr << "·    "_tab << t1 << '\n';
			std::cerr << "Tree 2:\n";
			std::cerr << "·    "_tab << t2 << '\n';
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

err_type exe_utilities_tree_isomorphism_rooted(
	const std::string& algorithm, std::ifstream& fin
) noexcept
{
	if (algorithm == "string") {
		return rooted_test<string_algo, false>(fin);
	}
	else {
		return rooted_test<tuple_algo, false>(fin);
	}
}

} // namespace manual

namespace automatic {

template <lal::detail::isomorphism::algorithm algo>
err_type positive_exhaustive_test(std::ifstream& fin) noexcept
{
	std::mt19937 gen(1234);
	lal::graphs::rooted_tree relab_tree;

	uint64_t n, N;
	while (fin >> n >> N) {

		std::cout << n << " " << N << '\n';

		lal::generate::all_ulab_rooted_trees Gen(n);
		while (not Gen.end()) {
			const lal::graphs::rooted_tree cur_tree = Gen.get_tree();
			Gen.next();

			std::vector<lal::edge> edges_cur = cur_tree.get_edges();

			for (uint64_t r = 0; r < N; ++r) {
				relabel_tree_vertices(
					n, cur_tree.get_root(), edges_cur, relab_tree, gen
				);

				const bool res = lal::detail::are_trees_isomorphic<algo, false>(
					cur_tree, relab_tree
				);

				if (not res) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Isomorphism test returned false on "
								 "isomorphic trees.\n";
					std::cerr << "    Tree:\n";
					std::cerr << cur_tree << '\n';
					std::cerr << "    Tree:\n";
					std::cerr << relab_tree << '\n';
					return err_type::test_execution;
				}
			}
		}
	}
	return err_type::no_error;
}

template <lal::detail::isomorphism::algorithm algo>
err_type positive_random_test(std::ifstream& fin) noexcept
{
	std::mt19937 gen(1234);
	uint64_t n, N_trees, N;
	fin >> n >> N_trees >> N;

	lal::graphs::rooted_tree relab_tree;
	lal::generate::all_ulab_rooted_trees Gen(n);

	for (uint64_t t = 0; t < N_trees; ++t) {
		const lal::graphs::rooted_tree cur_tree = Gen.get_tree();
		std::vector<lal::edge> edges_cur = cur_tree.get_edges();

		for (uint64_t r = 0; r < N; ++r) {
			relabel_tree_vertices(
				n, cur_tree.get_root(), edges_cur, relab_tree, gen
			);

			const bool res = lal::detail::are_trees_isomorphic<algo, false>(
				cur_tree, relab_tree
			);

			if (not res) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Isomorphism test returned false on "
							 "isomorphic trees.\n";
				std::cerr << "    Tree:\n";
				std::cerr << cur_tree << '\n';
				std::cerr << "    Tree:\n";
				std::cerr << relab_tree << '\n';
				return err_type::test_execution;
			}
		}
	}
	return err_type::no_error;
}

template <lal::detail::isomorphism::algorithm algo>
err_type negative_exhaustive_test(std::ifstream& fin) noexcept
{
	std::mt19937 gen(1234);
	uint64_t n, N;
	fin >> n >> N;

	std::vector<lal::graphs::rooted_tree> all_trees;
	lal::generate::all_ulab_rooted_trees Gen(n);
	while (not Gen.end()) {
		all_trees.push_back(Gen.get_tree());
		Gen.next();
	}

	lal::graphs::rooted_tree relab_tree;
	for (std::size_t i = 0; i < all_trees.size(); ++i) {
		const lal::graphs::rooted_tree& ti = all_trees[i];
		for (std::size_t j = i + 1; j < all_trees.size(); ++j) {

			const lal::graphs::rooted_tree& tj = all_trees[j];
			std::vector<lal::edge> edges_tj = tj.get_edges();

			for (uint64_t r = 0; r < N; ++r) {
				relabel_tree_vertices(
					n, tj.get_root(), edges_tj, relab_tree, gen
				);

				const bool res = lal::detail::are_trees_isomorphic<algo, false>(
					ti, relab_tree
				);
				if (res) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Isomorphism test returned true on "
								 "non-isomorphic trees.\n";
					std::cerr << "    Tree:\n";
					std::cerr << ti << '\n';
					std::cerr << "    Tree:\n";
					std::cerr << relab_tree << '\n';
					return err_type::test_execution;
				}
			}
		}
	}
	return err_type::no_error;
}

template <lal::detail::isomorphism::algorithm algo>
err_type rooted_tests(
	const bool expected_isomorphism,
	const std::string& inspection,
	std::ifstream& fin
) noexcept
{
	if (expected_isomorphism) {
		if (inspection == "exhaustive") {
			return positive_exhaustive_test<algo>(fin);
		}
		else {
			return positive_random_test<algo>(fin);
		}
	}
	else {
		return negative_exhaustive_test<algo>(fin);
	}
	return err_type::no_error;
}

err_type exe_utilities_tree_isomorphism_rooted(
	const std::string& algorithm, std::ifstream& fin
) noexcept
{
	const auto sbi = read_should_be_or_not(fin);
	if (not sbi.has_value()) {
		return err_type::test_format;
	}
	const bool expected_isomorphism = *sbi;

	std::string inspection;
	fin >> inspection;
	if (inspection != "exhaustive" and inspection != "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Incorrect inspection method '" << inspection << "'\n";
		std::cerr << "    Must be one of: exhaustive/random\n";
		return err_type::test_format;
	}

	if (algorithm == "string") {
		return rooted_tests<string_algo>(expected_isomorphism, inspection, fin);
	}
	else {
		return rooted_tests<tuple_algo>(expected_isomorphism, inspection, fin);
	}
}

} // namespace automatic

err_type exe_utilities_tree_isomorphism_rooted(std::ifstream& fin) noexcept
{
	std::string mode;
	fin >> mode;
	if (mode != "manual" and mode != "automatic") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid mode '" << mode << "'.\n";
		std::cerr << "    Must be one of: manual/automatic\n";
		return err_type::test_format;
	}

	std::string algorithm;
	fin >> algorithm;
	if (algorithm != "string" and algorithm != "tuple") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid algorithm '" << mode << "'.\n";
		std::cerr << "    Must be one of: string/tuple\n";
		return err_type::test_format;
	}

	err_type r;
	if (mode == "manual") {
		r = manual::exe_utilities_tree_isomorphism_rooted(algorithm, fin);
	}
	else {
		r = automatic::exe_utilities_tree_isomorphism_rooted(algorithm, fin);
	}

	if (r == err_type::no_error) {
		TEST_GOODBYE;
	}
	return r;
}

} // namespace utilities
} // namespace tests
