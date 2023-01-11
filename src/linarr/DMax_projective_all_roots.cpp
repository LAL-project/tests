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
#include <iostream>
#include <fstream>
#include <set>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/DMax.hpp>
#include <lal/linarr/formal_constraints.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>

// common includes
#include "common/definitions.hpp"

namespace lal {
namespace linarr {
enum class algorithms_DMax_projective {
	AlemanyEstebanFerrer
};
}
}

namespace tests {
namespace linarr {

err_type check_from_source_file(std::ifstream& fin) noexcept {
	char delim;
	fin >> delim;
	uint64_t n;
	while (delim == '*') {
		fin >> n;

		lal::head_vector hv(n);
		while (fin >> delim and delim == '-') {
			for (uint64_t i = 0; i < n; ++i) { fin >> hv[i]; }

			const auto T =
				std::move(lal::graphs::from_head_vector_to_free_tree(hv).first);
			const auto all_dmax_roots =
				std::move(lal::linarr::max_sum_edge_lengths_projective_roots(T).first);

			uint64_t DMax_u;
			for (lal::node u = 0; u < n; ++u) {
				fin >> DMax_u;

				auto rtu = lal::graphs::rooted_tree(T, u);
				rtu.calculate_size_subtrees();
				const auto DMax_pr_u = lal::linarr::max_sum_edge_lengths_projective(rtu).first;

				if (all_dmax_roots[u] != DMax_pr_u) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When using lal::linarr::max_sum_edge_lengths_projective.\n";
					std::cerr << "    DMax projective at '" << u << "' differs from library's value.\n";
					std::cerr << "    Library value: " << all_dmax_roots[u] << '\n';
					std::cerr << "    Test value: " << DMax_pr_u << '\n';
					std::cerr << "    For tree:" << '\n';
					std::cerr << T << '\n';
				}

				if (DMax_u != all_dmax_roots[u]) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When using lal::linarr::max_sum_edge_lengths_projective_roots.\n";
					std::cerr << "    DMax projective at '" << u << "' differs from library's value.\n";
					std::cerr << "    Library value: " << all_dmax_roots[u] << '\n';
					std::cerr << "    Test value: " << DMax_u << '\n';
					std::cerr << "    For tree:" << '\n';
					std::cerr << T << '\n';
					return err_type::test_execution;
				}
			}
		}
	}

	return err_type::no_error;
}

err_type check_from_code(std::ifstream& fin) noexcept {
	uint64_t n;
	while (fin >> n) {
		lal::generate::all_ulab_free_trees gen(n);
		while (not gen.end()) {
			const auto t = gen.get_tree();

			// Library result
			const auto library_res = lal::linarr::max_sum_edge_lengths_projective_roots(t);

			// Brute force result
			lal::detail::data_array<uint64_t> brute_force_res(n);
			for (lal::node u = 0; u < n; ++u) {
				lal::graphs::rooted_tree rt(t, u);
				rt.calculate_size_subtrees();
				brute_force_res[u] = lal::linarr::max_sum_edge_lengths_projective(rt).first;
			}

			// compare
			for (lal::node u = 0; u < n; ++u) {
				if (library_res.first[u] != brute_force_res[u]) {
					std::cerr << ERROR << '\n';
					std::cerr << "    When using lal::linarr::max_sum_edge_lengths_projective_roots.\n";
					std::cerr << "    DMax projective at '" << u << "' differs from library's value.\n";
					std::cerr << "    Library value:     " << library_res.first[u] << '\n';
					std::cerr << "    Brute force value: " << brute_force_res[u] << '\n';
					std::cerr << "    For tree:" << '\n';
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
			}

			gen.next();
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_DMax_projective_all_max_roots(std::ifstream& fin) noexcept {
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
	[[maybe_unused]] lal::linarr::algorithms_DMax_projective algo_choice;
	if (algo == "AEF") {
		algo_choice = lal::linarr::algorithms_DMax_projective::AlemanyEstebanFerrer;
		algo_name = "AEF";
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Algorithm '" << algo << "' not recognised.\n";
		return err_type::test_format;
	}

	std::string source;
	fin >> source;

	err_type err;
	if (source == "from_file") {
		err = check_from_source_file(fin);
	}
	else if (source == "from_code") {
		err = check_from_code(fin);
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Unknown source: '" << source << "'.\n";
		err = err_type::test_format;
	}

	if (err != err_type::no_error) { return err; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
