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
#include <random>
#include <set>

// lal includes
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/detail/properties/tree_maximum_caterpillar.hpp>
#include <lal/detail/macros/basic_convert.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

namespace tests {
namespace properties {

template <class tree_t, class str_t>
bool couple_checks(const tree_t& t, const str_t& cat_struct1, const str_t& cat_struct2) noexcept {
	[[maybe_unused]] const auto n = t.get_num_nodes();

	// equal distances
	{
	const uint64_t d = std::get<0>(cat_struct1);
	const uint64_t dr = std::get<0>(cat_struct2);
	if (d != dr) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Distance when using free tree:   " << d << '\n';
		std::cerr << "    Distance when using rooted tree: " << dr << '\n';
		return false;
	}
	}

	return true;
}

template <class tree_t, class str_t>
bool single_checks(const tree_t& t, const str_t& cat_struct) noexcept {
	[[maybe_unused]] const auto n = t.get_num_nodes();

	const uint64_t d = std::get<0>(cat_struct);

	// make sure the distance makes sense
	{
	std::size_t num_verts_in_caterpillar = 0;
	for (char c : std::get<2>(cat_struct)) {
		num_verts_in_caterpillar += lal::detail::to_uint64(c);
	}
	if (num_verts_in_caterpillar != n - d) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Distance and size of the caterpillar do not agree.\n";
		std::cerr << "    Distance:         " << d << '\n';
		std::cerr << "    Caterpillar size: " << num_verts_in_caterpillar << '\n';
		return false;
	}
	}

	// make sure the path returned is actually a path
	{
	const auto correct_path =
	[&](const auto path) {
		for (std::size_t i = 0; i < path.size() - 1; ++i) {
			const auto u = path[i];

			bool has_neighbour = false;
			if constexpr (std::is_base_of_v<lal::graphs::free_tree, tree_t>) {
				for (lal::node v : t.get_neighbours(u)) {
					has_neighbour = has_neighbour or (v == path[i + 1]);
				}
			}
			else {
				for (lal::node v : t.get_in_neighbours(u)) {
					has_neighbour = has_neighbour or (v == path[i + 1]);
				}
				for (lal::node v : t.get_out_neighbours(u)) {
					has_neighbour = has_neighbour or (v == path[i + 1]);
				}
			}
			if (not has_neighbour) {
				std::cerr << ERROR << '\n';
				std::cerr << "    None of the neighbours of '" << u << "' is '" << path[i + 1] << "'\n";
				return false;
			}
		}
		return true;
	};
	if (not correct_path(std::get<1>(cat_struct))) {
		return false;
	}
	}

	return true;
}

template <class tree_t>
auto exe_single_tree(const tree_t& t) noexcept {
	using namespace lal::detail::maximum_subtrees::caterpillar;

	const auto n = t.get_num_nodes();

	const auto cat_struct = max_subtree<result::distance_structure>(t);
	std::cout << "Distance: " << std::get<0>(cat_struct) << '\n';

	for (lal::node u = 0; u < n - 1; ++u) {
		std::cout
			<< std::setw( 1 + int(std::log10(u == 0 ? 1 : u)) )
			<< u
			<< ' ';
	}
	std::cout
		<< std::setw( 1 + int(std::log10(n - 1 == 0 ? 1 : n - 1)) )
		<< n - 1;
	std::cout << '\n';

	for (lal::node u = 0; u < n - 1; ++u) {
		std::cout
			<< std::setw( 1 + int(std::log10(u == 0 ? 1 : u)) )
			<< int(std::get<2>(cat_struct)[u])
			<< ' ';
	}
	std::cout
		<< std::setw( 1 + int(std::log10(n - 1 == 0 ? 1 : n - 1)) )
		<< int(std::get<2>(cat_struct)[n - 1]);

	std::cout << '\n';

	return cat_struct;
}

err_type input_mode(std::ifstream& fin) noexcept {
	std::string format;
	fin >> format;
	if (format != "edge_list" and format != "head_vector") {
		std::cerr << "Error: wrong format '" << format << "'.\n";
		std::cerr << "    Only 'head_vector' or 'edge_list' is allowed.\n";
		return err_type::test_format;
	}

	uint64_t n;
	fin >> n;

	const auto t = [&]() {
		if (format == "head_vector") {
			lal::head_vector hv(n);
			for (std::size_t i = 0; i < hv.size(); ++i) {
				fin >> hv[i];
			}
			return std::move(lal::graphs::from_head_vector_to_free_tree(hv).first);
		}
		else if (format == "edge_list") {
			std::vector<lal::edge> edges(n - 1);
			for (std::size_t i = 0; i < edges.size(); ++i) {
				fin >> edges[i].first >> edges[i].second;
			}
			return lal::graphs::from_edge_list_to_free_tree(edges);
		}
		else {
			return lal::graphs::free_tree(1);
		}
	}();

	const auto v = exe_single_tree(t);
	if (not single_checks(t, v)) {
		return err_type::test_execution;
	}

	for (lal::node u = 0; u < n; ++u) {
		const lal::graphs::rooted_tree rt(t, u);
		std::cout << "--------------------\n";
		std::cout << rt << '\n';
		const auto vr = exe_single_tree(rt);
		if (not single_checks(t, vr)) {
			return err_type::test_execution;
		}
		if (not couple_checks(t, v, vr)) {
			return err_type::test_execution;
		}
	}

	return err_type::no_error;
}

err_type automatic_caterpillar_mode(std::ifstream& fin) noexcept {
	using namespace lal::detail::maximum_subtrees::caterpillar;

	uint64_t n;

#define check																\
	if (t.is_of_tree_type(lal::graphs::tree_type::caterpillar)) {			\
		if (dist != 0) {													\
			std::cerr << "Error! Non-zero caterpillar distance for\n";		\
			std::cerr << "    caterpillar input tree:\n";					\
			std::cerr << t << '\n';											\
			return err_type::test_execution;								\
		}																	\
	}																		\
	else {																	\
		if (dist == 0) {													\
			std::cerr << "Error! Zero caterpillar distance for\n";			\
			std::cerr << "    non-caterpillar input tree:\n";				\
			std::cerr << t << '\n';											\
			return err_type::test_execution;								\
		}																	\
	}

	while (fin >> n) {
		lal::generate::all_ulab_free_trees gen(n);
		gen.set_calculate_tree_type(false);

		for (; not gen.end(); gen.next()) {
			auto t = gen.get_tree();

			// the tree has no valid tree type
			{
			const auto dist = max_subtree<result::distance>(t);
			t.calculate_tree_type();
			check;
			}

			// the tree has valid tree type
			{
			const auto dist = max_subtree<result::distance>(t);
			check;
			}
		}
	}

	return err_type::no_error;
}

err_type exe_properties_caterpillar_distance(std::ifstream& fin) noexcept
{
	const std::set<std::string> allowed_modes({"automatic", "input"});

	std::string mode;
	fin >> mode;
	if (allowed_modes.find(mode) == allowed_modes.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong mode '" << mode << "'\n";
		std::cerr << "    Allowed modes:\n";
		for (const auto& m : allowed_modes) {
		std::cerr << "    - " << m << '\n';
		}
		return err_type::test_format;
	}

	if (mode == "input") {
		const auto r = input_mode(fin);
		if (r != err_type::no_error) {
			std::cerr << ERROR << '\n';
			return err_type::test_execution;
		}
	}
	else if (mode == "automatic") {
		const auto r = automatic_caterpillar_mode(fin);
		if (r != err_type::no_error) {
			std::cerr << ERROR << '\n';
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
