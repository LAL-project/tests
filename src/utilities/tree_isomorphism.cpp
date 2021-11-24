/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library.
 *
 *  Tests of the Linear Arrangement Library is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU Affero
 *  General Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later version.
 *
 *  Tests of the Linear Arrangement Library is distributed in the hope
 *  that it will be useful, but WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Tests of the Linear Arrangement Library.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <optional>
#include <iostream>
#include <fstream>
#include <random>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/utilities/tree_isomorphism.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"

#define to_uint64(x) static_cast<uint64_t>(x)

namespace tests {
namespace utilities {

std::optional<bool> read_should_be_or_not(std::ifstream& fin) {
	std::string should_what;
	fin >> should_what;
	if (should_what == "ISOMORPHIC") {
		return true;
	}
	if (should_what == "NOT_ISOMORPHIC") {
		return false;
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    String '" << should_what << "' is not a valid identifier of the test.\n";
	std::cerr << "    Should be either: 'ISOMORPHIC' or 'NOT_ISOMORPHIC'.\n";
	return {};
}

template<class T>
void read_free(std::ifstream& fin, T& t) {
	std::vector<lal::edge> edges(t.get_num_nodes() - 1);
	for (auto& e : edges) {
		fin >> e.first >> e.second;
	}
	t.add_edges(edges);
}

template<class T>
void read_rooted(std::ifstream& fin, T& t) {
	read_free(fin, t);
	lal::node r;
	fin >> r;
	t.set_root(r);
}

err_type free_isomorphism_test(std::ifstream& fin) {
	const auto sbi = read_should_be_or_not(fin);
	if (not sbi.has_value()) { return err_type::test_format; }
	const bool should_be_isomorphic = *sbi;

	uint64_t n;
	while (fin >> n) {
		lal::graphs::free_tree t1(n), t2(n);
		read_free(fin, t1);
		read_free(fin, t2);

		const bool are_iso = lal::utilities::are_trees_isomorphic(t1,t2);
		std::cout << "Are isomorphic? " << std::boolalpha << are_iso << '\n';

		if (should_be_isomorphic and not are_iso) {
			std::cerr << ERROR << '\n';
			std::cerr << "    The trees should be isomorphic but they are NOT!\n";
			return err_type::test_execution;
		}

		if (not should_be_isomorphic and are_iso) {
			std::cerr << ERROR << '\n';
			std::cerr << "    The trees should not be isomorphic but they are!\n";
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

err_type rooted_isomorphism_test(std::ifstream& fin) {
	const auto sbi = read_should_be_or_not(fin);
	if (not sbi.has_value()) { return err_type::test_format; }
	const bool should_be_isomorphic = *sbi;

	uint64_t n;
	while (fin >> n) {
		lal::graphs::rooted_tree t1(n), t2(n);
		read_rooted(fin, t1);
		read_rooted(fin, t2);

		const bool are_iso = lal::utilities::are_trees_isomorphic(t1,t2);
		std::cout << "Are isomorphic? " << std::boolalpha << are_iso << '\n';

		if (should_be_isomorphic and not are_iso) {
			std::cerr << ERROR << '\n';
			std::cerr << "    The trees should be isomorphic but they are NOT!\n";
			return err_type::test_execution;
		}

		if (not should_be_isomorphic and are_iso) {
			std::cerr << ERROR << '\n';
			std::cerr << "    The trees should not be isomorphic but they are!\n";
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

err_type exe_utils_tree_iso_manual(std::ifstream& fin) {
	std::string tree_type;
	fin >> tree_type;
	if (tree_type != "free" and tree_type != "rooted") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid tree type '" << tree_type << "'.\n";
		std::cerr << "    Must be one of: free/rooted\n";
		return err_type::test_format;
	}

	return (
		tree_type == "free" ?
		free_isomorphism_test(fin) :
		rooted_isomorphism_test(fin)
	);
}

// -----------------------------------------------------------------------------

// ground truth: ISOMORPHIC

template<class Tree, class GEN>
err_type pos_exh_test(std::ifstream& fin) {
	Tree relab_tree;

	uint64_t n, N_relabs;
	while (fin >> n >> N_relabs) {
		GEN Gen(n);
		while (not Gen.end()) {
			const Tree cur_tree = Gen.get_tree();
			Gen.next();

			std::vector<lal::edge> edges_cur = cur_tree.get_edges();
			if constexpr (std::is_base_of<lal::graphs::directed_graph, Tree>::value) {
				relab_tree.init(n);
				relab_tree.set_root(cur_tree.get_root());
			}

			for (uint64_t N = 0; N < N_relabs; ++N) {
				relab_tree.clear();
				relabel_tree_vertices(edges_cur, relab_tree, true, true);

				const bool r =
					lal::utilities::are_trees_isomorphic(cur_tree, relab_tree);

				if (not r) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Isomorphism test returned false on isomorphic trees.\n";
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

template<class Tree, class GEN>
err_type pos_rand_test(std::ifstream& fin) {
	uint64_t n, N_trees, N_relabs;
	fin >> n >> N_trees >> N_relabs;

	Tree relab_tree;
	GEN Gen(n);

	for (uint64_t t = 0; t < N_trees; ++t) {
		const Tree cur_tree = Gen.get_tree();
		std::vector<lal::edge> edges_cur = cur_tree.get_edges();
		if constexpr (std::is_base_of<lal::graphs::directed_graph, Tree>::value) {
			relab_tree.init(n);
			relab_tree.set_root(cur_tree.get_root());
		}

		for (uint64_t l = 0; l < N_relabs; ++l) {
			relab_tree.clear();
			relabel_tree_vertices(edges_cur, relab_tree, true, true);

			const bool r =
				lal::utilities::are_trees_isomorphic(cur_tree, relab_tree);
			if (not r) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Isomorphism test returned false on isomorphic trees.\n";
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

// ground truth: NON-ISOMORPHIC

template<class Tree, class GEN>
err_type neg_exh_test(std::ifstream& fin) {
	uint64_t n, N_relabs;
	fin >> n >> N_relabs;

	if constexpr (std::is_base_of<lal::graphs::undirected_graph, Tree>::value) {
		if (n > 21) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Using n>21 (n=" << n << ") is going to freeze the computer.\n";
			std::cerr << "    Aborting.\n";
			return err_type::test_execution;
		}
	}
	else if constexpr (std::is_base_of<lal::graphs::directed_graph, Tree>::value) {
		if (n > 18) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Using n>18 (n=" << n << ") is going to freeze the computer.\n";
			std::cerr << "    Aborting.\n";
			return err_type::test_execution;
		}
	}

	std::vector<Tree> all_trees;
	GEN Gen(n);
	while (not Gen.end()) {
		all_trees.push_back(Gen.get_tree());
		Gen.next();
	}

	Tree relab_tree;
	for (std::size_t i = 0; i < all_trees.size(); ++i) {
		const Tree& ti = all_trees[i];
		for (std::size_t j = i + 1; j < all_trees.size(); ++j) {

			const Tree& tj = all_trees[j];
			std::vector<lal::edge> edges_tj = tj.get_edges();
			if constexpr (std::is_base_of<lal::graphs::directed_graph, Tree>::value) {
				relab_tree.init(n);
				relab_tree.set_root(tj.get_root());
			}

			for (uint64_t l = 0; l < N_relabs; ++l) {
				relabel_tree_vertices(edges_tj, relab_tree, true, true);

				const bool r =
					lal::utilities::are_trees_isomorphic(ti, relab_tree);
				if (r) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Isomorphism test returned true on non-isomorphic trees.\n";
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

template<class T, class Exh_Gen, class Rand_Gen>
err_type exe_test_auto(const std::string& expected, const std::string& mode, std::ifstream& fin) {
	if (expected == "positive") {
		if (mode == "exhaustive") { return pos_exh_test<T,Exh_Gen>(fin); }
		else { return pos_rand_test<T,Rand_Gen>(fin); }
	}
	else {
		return neg_exh_test<T,Exh_Gen>(fin);
	}
}

err_type exe_utils_tree_iso_auto(std::ifstream& fin) {
	std::string tree_type;
	fin >> tree_type;
	if (tree_type != "free" and tree_type != "rooted") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid tree type '" << tree_type << "'.\n";
		std::cerr << "    Must be one of: free/rooted\n";
		return err_type::test_format;
	}
	std::string expected_result;
	fin >> expected_result;
	if (expected_result != "positive" and expected_result != "negative") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid expected result '" << expected_result << "'.\n";
		std::cerr << "    Must be one of: positive/negative\n";
		return err_type::test_format;
	}
	std::string mode;
	fin >> mode;
	if (mode != "exhaustive" and mode != "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid mode '" << mode << "'.\n";
		std::cerr << "    Must be one of: exhasutive/random\n";
		return err_type::test_format;
	}

	if (expected_result == "negative" and mode == "random") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Combination of expected result '" << expected_result << "' and\n";
		std::cerr << "    mode of generation '" << mode << "' is invalid.\n";
		return err_type::test_format;
	}

	return (tree_type == "free" ?
		exe_test_auto<lal::graphs::free_tree, lal::generate::all_ulab_free_trees, lal::generate::rand_ulab_free_trees>
		(expected_result, mode, fin)
		:
		exe_test_auto<lal::graphs::rooted_tree, lal::generate::all_ulab_rooted_trees, lal::generate::rand_ulab_rooted_trees>
		(expected_result, mode, fin)
	);
}

// -----------------------------------------------------------------------------

err_type exe_utilities_tree_isomorphism(const input_list& inputs, std::ifstream& fin) {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::string mode;
	fin >> mode;
	if (mode != "manual" and mode != "automatic") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Invalid mode '" << mode << "'.\n";
		std::cerr << "    Must be one of: manual/automatic\n";
		return err_type::test_format;
	}

	err_type r;
	if (mode == "manual") {
		r = exe_utils_tree_iso_manual(fin);
	}
	else {
		r = exe_utils_tree_iso_auto(fin);
	}

	if (r == err_type::no_error) {
		TEST_GOODBYE
	}
	return r;
}

} // -- namespace utilities
} // -- namespace tests
