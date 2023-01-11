/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2023
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
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <cassert>
#include <queue>
#include <set>

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/detail/graphs/conversions.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"
#include "common/test_utils.hpp"

// linarr includes
#include "linarr/arrangement_check.hpp"

typedef std::pair<uint64_t, lal::linear_arrangement> algo_result;

bool lt(const algo_result& r1, const algo_result& r2) noexcept { return r1.first < r2.first; }
bool le(const algo_result& r1, const algo_result& r2) noexcept { return r1.first <= r2.first; }
bool eq(const algo_result& r1, const algo_result& r2) noexcept { return r1.first == r2.first; }
bool ge(const algo_result& r1, const algo_result& r2) noexcept { return r1.first >= r2.first; }
bool gt(const algo_result& r1, const algo_result& r2) noexcept { return r1.first > r2.first; }

namespace tests {
namespace linarr {

#define correct_algo_str(algo)												\
{																			\
	if (allowed_algos.find(algo) == allowed_algos.end()) {					\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Algorithm name '" << algo << "' is invalid.\n";	\
		std::cerr << "    Must be one of:\n";								\
		for (const std::string& s : allowed_algos) {						\
		std::cerr << "        " << s << '\n';								\
		}																	\
		return err_type::test_format;										\
	}																		\
}

#define correct_comp_str(comp)											\
{																		\
	if (allowed_comps.find(comp) == allowed_comps.end()) {				\
		std::cerr << ERROR << '\n';										\
		std::cerr << "    Comparison '" << comp << "' is invalid.\n";	\
		std::cerr << "    Must be one of:\n";							\
		for (const std::string& s : allowed_comps) {					\
		std::cerr << "        " << s << '\n';							\
		}																\
		return err_type::test_format;									\
	}																	\
}

err_type exe_linarr_Dmin_comparison(std::ifstream& fin) noexcept {

	const std::set<std::string> allowed_algos({"Plan_AEF", "Plan_HS", "Unc_YS", "Unc_FC2"});

	const auto Plan_HS =
		[](const lal::graphs::free_tree& t) -> algo_result
		{ return lal::linarr::min_sum_edge_lengths_planar(t, lal::linarr::algorithms_Dmin_planar::HochbergStallmann); };
	const auto Plan_AEF =
		[](const lal::graphs::free_tree& t) -> algo_result
		{ return lal::linarr::min_sum_edge_lengths_planar(t, lal::linarr::algorithms_Dmin_planar::AlemanyEstebanFerrer); };
	const auto Unc_FC2 =
		[](const lal::graphs::free_tree& t) -> algo_result
		{ return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Chung_2); };
	const auto Unc_YS =
		[](const lal::graphs::free_tree& t) -> algo_result
		{ return lal::linarr::min_sum_edge_lengths(t, lal::linarr::algorithms_Dmin::Shiloach); };

	std::map<std::string, std::function<algo_result (const lal::graphs::free_tree&)> > ALGOS;
	ALGOS["Plan_AEF"] = Plan_AEF;
	ALGOS["Plan_HS"] = Plan_HS;
	ALGOS["Unc_YS"] = Unc_YS;
	ALGOS["Unc_FC2"] = Unc_FC2;

	const std::set<std::string> allowed_comps({"<", "<=", "==", ">=", ">"});
	std::map<std::string, std::function<bool (const algo_result&, const algo_result&)>> COMPS;
	COMPS["<"] = lt;
	COMPS["<="] = le;
	COMPS["=="] = eq;
	COMPS[">="] = ge;
	COMPS[">"] = gt;

	std::string mode;
	while (fin >> mode) {
		if (mode != "exhaustive" and mode != "random") {
			std::cerr << ERROR << '\n';
			std::cerr << "    Mode '" << mode << "' is invalid.\n";
			std::cerr << "    Must be either 'exhaustive' or 'random'.\n";
			return err_type::test_format;
		}

		std::string algo1, comp, algo2;
		fin >> algo1 >> comp >> algo2;

		correct_algo_str(algo1);
		correct_comp_str(comp);
		correct_algo_str(algo2);

		uint64_t n;
		fin >> n;

		if (mode == "exhaustive") {
			lal::generate::all_ulab_free_trees TreeGen(n);
			while (not TreeGen.end()) {
				const lal::graphs::free_tree tree = TreeGen.get_tree();
				TreeGen.next();

				const auto res1 = ALGOS[algo1](tree);
				const auto res2 = ALGOS[algo2](tree);

				const bool correct1 =
					check_correctness_arr
					(tree, res1, ERROR_str, algo1, lal::linarr::is_arrangement);

				const bool correct2 =
					check_correctness_arr
					(tree, res2, ERROR_str, algo2, lal::linarr::is_arrangement);

				if (not correct1 or not correct2) { return err_type::test_execution; }

				if (not COMPS[comp](res1, res2)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Result of algorithm '" << algo1 << "' is not "
						 << "'" << comp << "'"
						 << " with respect to the result of algorithm "
						 << "'" << algo2 << "'."
						 << '\n';
					std::cerr << "    Algorithm: " << algo1 << '\n';
					std::cerr << "        D= " << res1.first << '\n';
					std::cerr << "        Arrangement: " << res1.second.direct_as_vector() << '\n';
					std::cerr << "    Algorithm: " << algo2 << '\n';
					std::cerr << "        D= " << res2.first << '\n';
					std::cerr << "        Arrangement: " << res2.second.direct_as_vector() << '\n';
					std::cerr << " In tree:\n";
					std::cerr << tree << '\n';
					return err_type::test_execution;
				}
			}
		}
		else {
			uint64_t N;
			fin >> N;

			lal::generate::rand_ulab_free_trees TreeGen(n, 9999);
			for (uint64_t i = 0; i < N; ++i) {
				const lal::graphs::free_tree tree = TreeGen.get_tree();

				const auto res1 = ALGOS[algo1](tree);
				const auto res2 = ALGOS[algo2](tree);

				const bool correct1 =
					check_correctness_arr
					(tree, res1, ERROR_str, algo1, lal::linarr::is_arrangement);
				const bool correct2 =
					check_correctness_arr
					(tree, res2, ERROR_str, algo2, lal::linarr::is_arrangement);

				if (not correct1 or not correct2) { return err_type::test_execution; }

				if (not COMPS[comp](res1, res2)) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Result of algorithm '" << algo1 << "' is not "
						 << "'" << comp << "'"
						 << " with respect to the result of algorithm "
						 << "'" << algo2 << "'."
						 << '\n';
					std::cerr << "    Algorithm: " << algo1 << '\n';
					std::cerr << "        D= " << res1.first << '\n';
					std::cerr << "        Arrangement:     " << res1.second.direct_as_vector() << '\n';
					std::cerr << "        Inv Arrangement: " << res1.second.inverse_as_vector() << '\n';
					std::cerr << "    Algorithm: " << algo2 << '\n';
					std::cerr << "        D= " << res2.first << '\n';
					std::cerr << "        Arrangement: " << res2.second.direct_as_vector() << '\n';
					std::cerr << "        Inv Arrangement: " << res2.second.inverse_as_vector() << '\n';
					std::cerr << "    For tree: \n";
					std::cerr << "        Head vector: [" << tree.get_head_vector(0) << "]\n";
					std::cerr << "        Edge list: " << tree.get_edges() << '\n';
					std::cerr << tree << '\n';
					return err_type::test_execution;
				}
			}
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
