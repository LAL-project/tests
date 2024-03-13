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
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/Dmin.hpp>
#include <lal/linarr/formal_constraints.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

// linarr includes
#include "linarr/test_optimum_algorithm_one.hpp"
#include "linarr/arrangement_check.hpp"

namespace tests {
namespace linarr {

namespace tests_Dmin_planar {

std::pair<uint64_t,lal::linear_arrangement> Dmin_planar_quadratic
(const lal::graphs::free_tree& t)
noexcept
{
	uint64_t Dmin_planar = std::numeric_limits<uint64_t>::max();
	lal::linear_arrangement arr;

	for (lal::node u = 0; u < t.get_num_nodes(); ++u) {
		lal::graphs::rooted_tree rt(t, u);
		rt.calculate_size_subtrees();

		const auto Dmin_proj = lal::linarr::min_sum_edge_lengths_projective(rt);
		if (Dmin_planar > Dmin_proj.first) {
			Dmin_planar = Dmin_proj.first;
			arr = std::move(Dmin_proj.second);
		}
	}
	return {Dmin_planar, std::move(arr)};
}

err_type check_tree(
	const lal::graphs::free_tree& T,
	const std::string& algo_name,
	const lal::linarr::algorithms_Dmin_planar& algo
)
noexcept
{
	const auto Dmin_planar_library =
		lal::linarr::min_sum_edge_lengths_planar(T, algo);

	const auto Dmin_planar_quadratic =
		tests_Dmin_planar::Dmin_planar_quadratic(T);

	// check correctness of library's result
	const bool correct1 =
		check_correctness_arr
		(T, Dmin_planar_library, ERROR_str, "Library (" + algo_name + ") planar",
		 lal::linarr::is_planar);

	if (not correct1) { return err_type::test_execution; }

	// check correctness of quadratic's result
	const bool correct2 =
		check_correctness_arr
		(T, Dmin_planar_quadratic, ERROR_str, "Quadratic planar",
		 lal::linarr::is_planar);

	if (not correct2) { return err_type::test_execution; }

	if (Dmin_planar_quadratic.first != Dmin_planar_library.first) {
		std::cerr << ERROR << '\n';
		std::cerr << "    The library produced a result that does not correspond to\n";
		std::cerr << "    quadratic algorithm's result.\n";
		std::cerr << "    Library: " << Dmin_planar_library.first << '\n';
		std::cerr << "        Arrangement: " << Dmin_planar_library.second.direct_as_vector() << '\n';
		std::cerr << "    Quadratic: " << Dmin_planar_quadratic.first << '\n';
		std::cerr << "        Arrangement: " << Dmin_planar_quadratic.second.direct_as_vector() << '\n';
		std::cerr << "    For tree:\n";
		std::cerr << T << '\n';
		return err_type::test_execution;
	}
	return err_type::no_error;
}

} // -- namespace tests_Dmin_planar

err_type exe_linarr_Dmin_planar(std::ifstream& fin) noexcept {
	const std::set<std::string> allowed_algos({"AEF", "HS", "quadratic"});
	const std::set<std::string> allowed_quadratic_modes({"exhaustive", "random"});

	const input_list inputs = read_input_list(fin);

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

	err_type err = err_type::no_error;

	if (algo == "AEF" or algo == "HS") {
		if (inputs.size() != 1) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Exactly one input files are allowed in this test.\n";
			std::cerr << "    Instead, " << inputs.size() << " were given.\n";
			return err_type::test_format;
		}

		std::ifstream input_file(inputs[0].first);
		if (not input_file.is_open()) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Input file '"
					  << inputs[0].first
					  << "' could not be opened.\n";
			return err_type::io;
		}

		lal::linarr::algorithms_Dmin_planar algo_choice =
				lal::linarr::algorithms_Dmin_planar::AlemanyEstebanFerrer;
		if (algo == "AEF") {
			algo_choice = lal::linarr::algorithms_Dmin_planar::AlemanyEstebanFerrer;
		}
		else if (algo == "HS") {
			algo_choice = lal::linarr::algorithms_Dmin_planar::HochbergStallmann;
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "Algorithm '" << algo << "' not recognised.\n";
			return err_type::test_format;
		}

		err = single_arrangement::test_optimum_algorithm<lal::graphs::free_tree>
		(
			[](lal::graphs::free_tree&) { },
			[=](const lal::graphs::free_tree& t) {
				return lal::linarr::min_sum_edge_lengths_planar(t, algo_choice);
			},
			[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
				return lal::linarr::sum_edge_lengths(t, arr);
			},
			[](const lal::graphs::free_tree& t, const lal::linear_arrangement& arr) {
				return lal::linarr::is_planar(t, arr);
			},
			[](const std::vector<lal::node>& v) {
				return std::move(lal::graphs::from_head_vector_to_free_tree(v).first);
			},
			input_file
		);
	}
	else if (algo == "quadratic") {
		if (inputs.size() != 0) {
			std::cerr << ERROR << '\n';
			std::cerr << "    No input files are allowed in this test.\n";
			std::cerr << "    Instead, " << inputs.size() << " were given.\n";
			return err_type::test_format;
		}

		std::string mode;
		fin >> mode;
		if (allowed_quadratic_modes.find(mode) == allowed_quadratic_modes.end()) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Unrecognized mode for quadratic algorithm '" << mode << "'.\n";
			std::cerr << "    Allowed modes:\n";
			for (const auto& s : allowed_quadratic_modes) {
			std::cerr << "    - " << s << '\n';
			}
			return err_type::test_format;
		}

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
		lal::linarr::algorithms_Dmin_planar algo_choice;
		if (algo == "AEF") {
			algo_choice = lal::linarr::algorithms_Dmin_planar::AlemanyEstebanFerrer;
			algo_name = "AEF";
		}
		else if (algo == "HS") {
			algo_choice = lal::linarr::algorithms_Dmin_planar::HochbergStallmann;
			algo_name = "HS";
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "Algorithm '" << algo << "' not recognised.\n";
			return err_type::test_format;
		}

		uint64_t n;
		while (fin >> n) {
			if (mode == "exhaustive") {
				lal::generate::all_ulab_free_trees Gen(n);
				while (not Gen.end()) {
					const auto T = Gen.get_tree();
					Gen.next();
					err = tests_Dmin_planar::check_tree(T, algo_name, algo_choice);
					if (err != err_type::no_error) {
						return err;
					}
				}
			}
			else if (mode == "random") {
				std::size_t n_rand_trees;
				fin >> n_rand_trees;

				lal::generate::rand_ulab_free_trees Gen(n, 1234);
				for (std::size_t i = 0; i < n_rand_trees; ++i) {
					const auto T = Gen.get_tree();
					err = tests_Dmin_planar::check_tree(T, algo_name, algo_choice);
					if (err != err_type::no_error) {
						return err;
					}
				}
			}
		}
	}

	if (err != err_type::no_error) {
		return err;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
