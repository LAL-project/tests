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
#include <iostream>
#include <fstream>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/properties/Q.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;
using namespace generate;

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/time.hpp"

// properties includes
#include "properties/C_rla_brute_force_algorithms.hpp"

namespace tests {
namespace properties {

void output_ExpVar_C_BF(const undirected_graph& g) {
	const rational Vr = variance_C_freqs_Q_rational(g.get_Q());
	const rational E1r = exp_num_crossings_rational(g);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << endl;
}

void output_ExpVar_C_formula_Q(const undirected_graph& g) {
	const rational Vr = var_num_crossings_rational_Q(g, g.get_Q());
	const rational E1r = exp_num_crossings(g);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << endl;
}

void output_ExpVar_C_formula_no_Q(const undirected_graph& g, bool reuse) {
	const rational Vr = var_num_crossings_rational(g, reuse);
	const rational E1r = exp_num_crossings(g);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << endl;
}

void output_ExpVar_C_trees(const undirected_graph& g) {
	const free_tree t = g;
	const rational Vr = var_num_crossings_tree_rational(t);
	const rational E1r = exp_num_crossings(t);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << endl;
}

void output_ExpVar_C_forests(const undirected_graph& g) {
	const rational Vr = var_num_crossings_forest_rational(g);
	const rational E1r = exp_num_crossings(g);
	const rational E2r = Vr + E1r*E1r;
	cout << E1r << "\t" << E2r << "\t" << Vr << endl;
}

bool check_ExpVar_C_all_trees(uint32_t n) {
	all_ulab_free_trees TreeGen(n);

	uint32_t k = 0;

	while (TreeGen.has_next()) {
		TreeGen.next();
		const free_tree tree = TreeGen.get_tree();

		const rational Vr_bf = variance_C_freqs_rational(tree);
		const rational Vr_gen = var_num_crossings_rational(tree);
		const rational Vr_trees = var_num_crossings_tree_rational(tree);

		if (Vr_bf != Vr_gen or Vr_bf != Vr_trees) {
			cerr << "Error in tree " << k << " of size " << n << endl;
			cerr << "    Brute force alg: " << Vr_bf << endl
				 << "    Alg for general: " << Vr_gen << endl
				 << "    Alg for trees: " << Vr_trees
				 << endl;
			cerr << tree << endl;
			return false;
		}

		++k;
	}
	return true;
}

bool check_ExpVar_C_mixed_trees(uint32_t r, uint32_t n_trees, uint32_t size_trees) {
	rand_ulab_free_trees TreeGen(size_trees);
	undirected_graph forest;

	for (uint32_t i = 0; i < r; ++i) {
		forest.clear();
		for (uint32_t j = 0; j < n_trees; ++j) {
			const free_tree rand_tree = TreeGen.get_tree();
			forest.disjoint_union(rand_tree);
		}
		if (not forest.is_normalised()) {
			forest.normalise();
		}

		const rational Vr_bf = variance_C_freqs_rational(forest);
		const rational Vr_gen = var_num_crossings_rational(forest);
		const rational Vr_forests = var_num_crossings_forest_rational(forest);

		if (Vr_bf != Vr_gen or Vr_bf != Vr_forests) {
			cerr << "Error in forest with " << n_trees
				 << " trees each of size " << size_trees << endl;
			cerr << "    Brute force alg: " << Vr_bf << endl
				 << "    Alg for general: " << Vr_gen << endl
				 << "    Alg for forests: " << Vr_forests
				 << endl;
			cerr << forest << endl;
			return false;
		}
	}
	return true;
}

err_type exe_properties_ExpVar_C(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_procs(
		{"brute_force", "formula-Q",
		 "formula-no-Q-reuse", "formula-no-Q-no-reuse",
		 "trees", "all-trees", "forests", "mixed-trees"}
	);

	string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Received '" << proc << "'." << endl;
		return err_type::test_format;
	}

	if (proc == "all-trees") {
		uint32_t n;
		fin >> n;
		uint32_t k;
		for (uint32_t i = 0; i < n; ++i) {
			fin >> k;
			const bool r = check_ExpVar_C_all_trees(k);

			if (not r) {
				return err_type::test_execution;
			}
		}
		TEST_GOODBYE
	}
	else if (proc == "mixed-trees") {
		uint32_t rep, n_pairs, n_trees, size_trees;
		fin >> rep >> n_pairs;

		for (uint32_t i = 0; i < n_pairs; ++i) {
			fin >> n_trees >> size_trees;
			bool r = check_ExpVar_C_mixed_trees(rep, n_trees, size_trees);
			if (not r) {
				return err_type::test_execution;
			}
		}
		TEST_GOODBYE
	}
	else {
		undirected_graph G;
		for (size_t i = 0; i < inputs.size(); ++i) {
			err_type r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, G);

			if (r != err_type::no_error) {
				return r;
			}

			if (proc == "brute_force") {
				output_ExpVar_C_BF(G);
			}
			else if (proc == "formula-Q") {
				output_ExpVar_C_formula_Q(G);
			}
			else if (proc == "formula-no-Q-reuse") {
				output_ExpVar_C_formula_no_Q(G, true);
			}
			else if (proc == "formula-no-Q-no-reuse") {
				output_ExpVar_C_formula_no_Q(G, false);
			}
			else if (proc == "trees") {
				output_ExpVar_C_trees(G);
			}
			else if (proc == "forests") {
				output_ExpVar_C_forests(G);
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Procedure '" << proc << "' not captured." << endl;
				return err_type::test_execution;
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
