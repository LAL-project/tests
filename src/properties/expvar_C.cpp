/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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
#include <iostream>
#include <fstream>
#include <cmath>
#include <set>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/Q.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/time.hpp"

// properties includes
#include "properties/C_rla_brute_force_algorithms.hpp"

namespace tests {
namespace properties {

void output_ExpVar_C_BF(const lal::graphs::undirected_graph& g) noexcept {
	const lal::numeric::rational Vr = nonLAL_variance_C_freqs_Q_rational(g.get_Q());
	const lal::numeric::rational E1r = lal::properties::exp_num_crossings_rational(g);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << '\n';
}

void output_ExpVar_C_formula_Q(const lal::graphs::undirected_graph& g) noexcept {
	const lal::numeric::rational Vr = nonLAL_var_num_crossings_rational_Q(g, g.get_Q());
	const lal::numeric::rational E1r = lal::properties::exp_num_crossings_rational(g);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << '\n';
}

err_type output_ExpVar_C_formula_no_Q(lal::graphs::undirected_graph& g, bool reuse) noexcept {
	g.normalise();
	const lal::numeric::rational Vr = lal::properties::var_num_crossings_rational(g, reuse);

	{
	std::vector<lal::edge> edges = g.get_edges();
	const std::size_t max_iters = 50;
	std::size_t iters = 0;
	while (g.is_normalised() and iters < max_iters) {
		shuffle_graph_edges(edges, g, false, true);
		++iters;
	}

	const lal::numeric::rational Vr_nonnormalised = lal::properties::var_num_crossings_rational(g, reuse);
	if (Vr_nonnormalised != Vr) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Variance obtained with a normalised graph differs from\n";
		std::cerr << "    the variance obtained with a non-normalised graph.\n";
		std::cerr << "    Normalised variance:     " << Vr << '\n';
		std::cerr << "    Non-normalised variance: " << Vr_nonnormalised << '\n';
		// mess the output so that the tester does not skip any error
		std::cout << "-1\n";
		return err_type::test_execution;
	}
	}

	const lal::numeric::rational E1r = lal::properties::exp_num_crossings_rational(g);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << '\n';
	return err_type::no_error;
}

void output_ExpVar_C_trees(const lal::graphs::undirected_graph& g) noexcept {
	const lal::graphs::free_tree t = g;
	const lal::numeric::rational Vr = lal::properties::var_num_crossings_tree_rational(t);
	const lal::numeric::rational E1r = lal::properties::exp_num_crossings_rational(t);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << '\n';
}

void output_ExpVar_C_forests(const lal::graphs::undirected_graph& g) noexcept {
	const lal::numeric::rational Vr = lal::properties::var_num_crossings_forest_rational(g);
	const lal::numeric::rational E1r = lal::properties::exp_num_crossings_rational(g);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << '\n';
}

bool check_ExpVar_C_all_trees(uint64_t n) noexcept {
	uint64_t k = 0;

	lal::generate::all_ulab_free_trees TreeGen(n);
	while (not TreeGen.end()) {
		const lal::graphs::free_tree tree = TreeGen.get_tree();
		TreeGen.next();

		const lal::numeric::rational Vr_bf = nonLAL_variance_C_freqs_rational(tree);
		const lal::numeric::rational Vr_gen = lal::properties::var_num_crossings_rational(tree);
		const lal::numeric::rational Vr_trees = lal::properties::var_num_crossings_tree_rational(tree);

		if (Vr_bf != Vr_gen or Vr_bf != Vr_trees) {
			std::cerr << "Error in tree " << k << " of size " << n << '\n';
			std::cerr << "    Brute force alg: " << Vr_bf << '\n'
				 << "    Alg for general: " << Vr_gen << '\n'
				 << "    Alg for trees: " << Vr_trees
				 << '\n';
			std::cerr << tree << '\n';
			return false;
		}

		++k;
	}
	return true;
}

bool check_ExpVar_C_mixed_trees(uint64_t r, uint64_t n_trees, uint64_t size_trees) {
	lal::generate::rand_ulab_free_trees TreeGen(size_trees);
	lal::graphs::undirected_graph forest;

	for (uint64_t i = 0; i < r; ++i) {
		forest.clear();
		for (uint64_t j = 0; j < n_trees; ++j) {
			const lal::graphs::free_tree rand_tree = TreeGen.get_tree();
			forest.disjoint_union(rand_tree);
		}
		if (not forest.is_normalised()) {
			forest.normalise();
		}

		const lal::numeric::rational Vr_bf = nonLAL_variance_C_freqs_rational(forest);
		const lal::numeric::rational Vr_gen = lal::properties::var_num_crossings_rational(forest);
		const lal::numeric::rational Vr_forests = lal::properties::var_num_crossings_forest_rational(forest);

		if (Vr_bf != Vr_gen or Vr_bf != Vr_forests) {
			std::cerr << "Error in forest with " << n_trees
				 << " trees each of size " << size_trees << '\n';
			std::cerr << "    Brute force alg: " << Vr_bf << '\n'
				 << "    Alg for general: " << Vr_gen << '\n'
				 << "    Alg for forests: " << Vr_forests
				 << '\n';
			std::cerr << forest << '\n';
			return false;
		}
	}
	return true;
}

err_type exe_properties_ExpVar_C(const input_list& inputs, std::ifstream& fin) {
	const std::set<std::string> allowed_procs(
		{"brute_force", "formula-Q",
		 "formula-no_Q-reuse", "formula-no_Q-no-reuse",
		 "trees", "all-trees", "forests", "mixed-trees"}
	);

	std::string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Received '" << proc << "'.\n";
		return err_type::test_format;
	}

	if (proc == "all-trees") {
		uint64_t n;
		fin >> n;
		uint64_t k;
		for (uint64_t i = 0; i < n; ++i) {
			fin >> k;
			const bool r = check_ExpVar_C_all_trees(k);

			if (not r) {
				return err_type::test_execution;
			}
		}
		TEST_GOODBYE
	}
	else if (proc == "mixed-trees") {
		uint64_t rep, n_pairs, n_trees, size_trees;
		fin >> rep >> n_pairs;

		for (uint64_t i = 0; i < n_pairs; ++i) {
			fin >> n_trees >> size_trees;
			bool r = check_ExpVar_C_mixed_trees(rep, n_trees, size_trees);
			if (not r) {
				return err_type::test_execution;
			}
		}
		TEST_GOODBYE
	}
	else {
		lal::graphs::undirected_graph G;
		for (std::size_t i = 0; i < inputs.size(); ++i) {
			G.clear();
			const err_type r =
			io_wrapper::read_graph(inputs[i].first, inputs[i].second, G, false);
			if (r != err_type::no_error) { return r; }

			if (proc == "brute_force") {
				output_ExpVar_C_BF(G);
			}
			else if (proc == "formula-Q") {
				output_ExpVar_C_formula_Q(G);
			}
			else if (proc == "formula-no_Q-reuse") {
				const auto err = output_ExpVar_C_formula_no_Q(G, true);
				if (err != err_type::no_error) { return err; }
			}
			else if (proc == "formula-no_Q-no-reuse") {
				const auto err = output_ExpVar_C_formula_no_Q(G, false);
				if (err != err_type::no_error) { return err; }
			}
			else if (proc == "trees") {
				output_ExpVar_C_trees(G);
			}
			else if (proc == "forests") {
				output_ExpVar_C_forests(G);
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr << "    Procedure '" << proc << "' not captured.\n";
				return err_type::test_execution;
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
