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
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_bipartite_arrangements.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/linarr/formal_constraints.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

#define check_and_process_arrangement(c, coloring)                             \
	if (not lal::linarr::is_bipartite(T, coloring, arr)) {                     \
		std::cerr << ERROR << '\n';                                            \
		std::cerr << "    In check: " << c << '\n';                            \
		std::cerr << "    Arrangement:         " << arr.direct_as_vector()     \
				  << '\n';                                                     \
		std::cerr << "    Inverse Arrangement: " << arr.inverse_as_vector()    \
				  << '\n';                                                     \
		std::cerr << "    For tree:\n";                                        \
		std::cerr << T << '\n';                                                \
		std::cerr << T.get_head_vector() << '\n';                              \
		return err_type::test_execution;                                       \
	}                                                                          \
	++iterations;                                                              \
	list_arrs.insert(arr);

#define final_check(c)                                                         \
	if (formula != iterations or formula != list_arrs.size()) {                \
		std::cerr << ERROR << '\n';                                            \
		std::cerr << "    In check: " << c << '\n';                            \
		std::cerr << "    Number of bipartite arrangements generated\n";       \
		std::cerr << "    does not agree with the formula.\n";                 \
		std::cerr << "        formula= " << formula << '\n';                   \
		std::cerr << "        iterations= " << iterations << '\n';             \
		std::cerr << "        unique amount= " << list_arrs.size() << '\n';    \
		std::cerr << "    List of arrangements:\n";                            \
		for (const auto& v : list_arrs) {                                      \
			std::cerr << "        " << v.inverse_as_vector() << '\n';          \
		}                                                                      \
		std::cerr << "    For tree:\n";                                        \
		std::cerr << T << '\n';                                                \
		std::cerr << T.get_head_vector() << '\n';                              \
		return err_type::test_execution;                                       \
	}

namespace tests {
namespace generate {
namespace bipartite {

inline lal::numeric::integer factorial(uint64_t f) noexcept
{
	if (f == 0) {
		return 1;
	}
	const lal::numeric::integer f1 = factorial(f - 1);
	return f1 * f;
}

inline lal::numeric::integer amount_bipartite(
	const lal::graphs::free_tree& T,
	const lal::properties::bipartite_graph_coloring& c
) noexcept
{
	const auto n = T.get_num_nodes();
	if (n == 1) {
		return 1;
	}

	static constexpr auto blue =
		lal::properties::bipartite_graph_coloring::blue;
	static constexpr auto red = lal::properties::bipartite_graph_coloring::red;
	uint64_t n_blue = 0;
	uint64_t n_red = 0;
	for (lal::node u = 0; u < n; ++u) {
		n_blue += c.get_color_of(u) == blue;
		n_red += c.get_color_of(u) == red;
	}
	return 2 * factorial(n_blue) * factorial(n_red);
}

inline err_type
test_a_tree(lal::graphs::free_tree& T, uint64_t nrelabs) noexcept
{
	std::vector<lal::edge> edges = T.get_edges();

	for (uint64_t i = 0; i < 2 * nrelabs; ++i) {
		relabel_tree_vertices(edges, T, (i < nrelabs ? false : true), false);

		const lal::properties::bipartite_graph_coloring c =
			lal::properties::bipartite_coloring(T);
		const lal::numeric::integer formula = amount_bipartite(T, c);

		uint64_t iterations = 0;
		std::set<lal::linear_arrangement> list_arrs;

		lal::generate::all_bipartite_arrangements ArrGen(T);

		// USAGE 1
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
			while (not ArrGen.end()) {
				const lal::linear_arrangement arr = ArrGen.get_arrangement();
				ArrGen.next();

				// Do some sanity checks.
				check_and_process_arrangement("Usage 1", c);
			}
			final_check("Usage 1");
		}

		// USAGE 2
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
			for (; not ArrGen.end(); ArrGen.next()) {
				const lal::linear_arrangement arr = ArrGen.get_arrangement();

				// Do some sanity checks.
				check_and_process_arrangement("Usage 2", c);
			}
			final_check("Usage 2");
		}

		// USAGE 3
		iterations = 0;
		ArrGen.reset();
		list_arrs.clear();
		{
			while (not ArrGen.end()) {
				const lal::linear_arrangement arr = ArrGen.yield_arrangement();

				// Do some sanity checks.
				check_and_process_arrangement("Usage 3", c);
			}
			final_check("Usage 3");
		}
	}
	return err_type::no_error;
}

} // namespace bipartite

err_type exe_gen_arr_all_bipartite(std::ifstream& fin) noexcept
{
	const std::set<std::string> allowed_modes({"automatic", "manual"});

	std::string mode;
	fin >> mode;

	if (allowed_modes.find(mode) == allowed_modes.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid mode '" << mode << "'.\n";
		std::cerr << "    Expected one of:\n";
		for (const auto& s : allowed_modes) {
			std::cerr << "    - " << s << '\n';
		}
		return err_type::test_format;
	}

	if (mode == "automatic") {
		uint64_t n, nrelabs;
		while (fin >> n >> nrelabs) {
			// do all trees of 'n' vertices
			lal::generate::all_ulab_free_trees TreeGen(n);

			while (not TreeGen.end()) {
				lal::graphs::free_tree fT = TreeGen.get_tree();
				TreeGen.next();

				const err_type e = bipartite::test_a_tree(fT, nrelabs);
				if (e != err_type::no_error) {
					return e;
				}
			}
		}
	}
	else if (mode == "manual") {
		std::string line;
		getline(fin, line);

		while (getline(fin, line)) {
			std::cout << "---------------\n";
			lal::head_vector hv;
			std::stringstream ss(line);
			uint64_t k;
			while (ss >> k) {
				hv.push_back(k);
			}

			const lal::graphs::free_tree T =
				std::move(lal::graphs::from_head_vector_to_free_tree(hv).first);

			std::cout << T << '\n';

			const lal::properties::bipartite_graph_coloring c =
				lal::properties::bipartite_coloring(T);

			const auto formula = bipartite::amount_bipartite(T, c);

			std::set<lal::linear_arrangement> list_arrs;
			std::size_t iterations = 0;
			for (lal::generate::all_bipartite_arrangements ArrGen(T);
				 not ArrGen.end();
				 ArrGen.next()) {
				const auto arr = ArrGen.get_arrangement();
				std::cout << iterations << ") " << arr.inverse_as_vector()
						  << '\n';

				check_and_process_arrangement(
					"Exhaustive enumeration (displayed)", c
				);
			}
			final_check("Exhaustive enumeration (displayed)");
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace generate
} // namespace tests
