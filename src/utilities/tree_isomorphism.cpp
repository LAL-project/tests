/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
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
#include <random>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/utilities/tree_isomorphism.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace utilities;

// custom includes
#include "definitions.hpp"

#define to_uint32(x) static_cast<uint32_t>(x)
#define ARBOR rooted_tree::rooted_tree_type::arborescence
#define ANTIARBOR rooted_tree::rooted_tree_type::anti_arborescence

namespace exe_tests {

template<class T>
void read_free(ifstream& fin, T& t) {
	vector<edge> edges(t.n_nodes() - 1);
	for (auto& e : edges) {
		fin >> e.first >> e.second;
	}
	t.add_edges(edges);
}

template<class T>
void read_rooted(ifstream& fin, T& t) {
	read_free(fin, t);
	node r;
	fin >> r;
	t.set_root(r);
}

err_type free_isomorphism_test(ifstream& fin) {
	uint32_t n;
	while (fin >> n) {
		free_tree t1(n), t2(n);
		read_free(fin, t1);
		read_free(fin, t2);

		cout << "Are isomorphic? "
			 << std::boolalpha
			 << utilities::are_trees_isomorphic(t1,t2)
			 << endl;
	}
	return err_type::no_error;
}

err_type rooted_isomorphism_test(ifstream& fin) {
	uint32_t n;
	while (fin >> n) {
		rooted_tree t1(n), t2(n);
		read_rooted(fin, t1);
		read_rooted(fin, t2);

		const bool has_t1 = t1.find_rooted_tree_type();
		const bool has_t2 = t2.find_rooted_tree_type();

		// ensure correct format of trees
		if (not has_t1) {
			cerr << ERROR << endl;
			cerr << "    Rooted tree" << endl;
			cerr << t1 << endl;
			cerr << "    is neither an arborescence or an anti-arborescence" << endl;
			return err_type::test_format;
		}
		if (not has_t2) {
			cerr << ERROR << endl;
			cerr << "    Rooted tree" << endl;
			cerr << t2 << endl;
			cerr << "    is neither an arborescence or an anti-arborescence" << endl;
			return err_type::test_format;
		}

		cout << "Are isomorphic? "
			 << std::boolalpha
			 << utilities::are_trees_isomorphic(t1,t2)
			 << endl;
	}
	return err_type::no_error;
}

err_type exe_utils_tree_iso_manual(ifstream& fin) {
	string tree_type;
	fin >> tree_type;
	if (tree_type != "free" and tree_type != "rooted") {
		cerr << ERROR << endl;
		cerr << "    Invalid tree type '" << tree_type << "'." << endl;
		cerr << "    Must be one of: free/rooted" << endl;
		return err_type::test_format;
	}

	return (
		tree_type == "free" ?
		free_isomorphism_test(fin) :
		rooted_isomorphism_test(fin)
	);
}

// -----------------------------------------------------------------------------

#define relabel_node(x, c,nl)		\
	if (x == c) { x = nl; }			\
	else if (x == nl) { x = c; }

void relabel_edges(vector<edge>& edges, node& r) {
	const uint32_t n = to_uint32(edges.size() + 1);

	default_random_engine gen;
	uniform_int_distribution<node> dist(0,n - 1);

	for (uint32_t i = 0; i < n; ++i) {
		// choose a vertex to relabel
		const node choice = dist(gen);
		// choose the new label
		const node new_lab = dist(gen);

		for (edge& e : edges) {
			node& s = e.first;
			node& t = e.second;
			if (s == choice) { s = new_lab; }
			else if (s == new_lab) { s = choice; }

			if (t == choice) { t = new_lab; }
			else if (t == new_lab) { t = choice; }
		}

		if (r == choice) { r = new_lab; }
		else if (r == new_lab) { r = choice; }
	}
}

template<class Tree>
err_type make_relabelled(vector<edge>& edges_T1, Tree& T2) {
	if constexpr (std::is_base_of<undirected_graph, Tree>::value) {
		T2.clear();
		T2.init(to_uint32(edges_T1.size() + 1));
		node dummy = 0;
		relabel_edges(edges_T1, dummy);
		T2.add_edges(edges_T1);
	}
	else if constexpr (std::is_base_of<directed_graph, Tree>::value) {
		node r = T2.get_root();
		relabel_edges(edges_T1, r);

		T2.clear();
		T2.init(to_uint32(edges_T1.size() + 1));
		T2.set_root(r);
		T2.add_edges(edges_T1);
		const bool rtt = T2.find_rooted_tree_type();
		if (not rtt) {
			cerr << ERROR << endl;
			cerr << "    Relabelling of edges failed." << endl;
			cerr << "    Rooted tree produced is of an invalid type." << endl;
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

// ground truth: ISOMORPHIC

template<class Tree, class GEN>
err_type pos_exh_test(ifstream& fin) {
	Tree relab_tree;
	GEN Gen;

	uint32_t n, N_relabs;
	while (fin >> n >> N_relabs) {
		Gen.init(n);

		while (Gen.has_next()) {
			Gen.next();
			const Tree cur_tree = Gen.get_tree();
			vector<edge> edges_cur = cur_tree.edges();
			if constexpr (std::is_base_of<directed_graph, Tree>::value) {
				relab_tree.init(n);
				relab_tree.set_root(cur_tree.get_root());
			}

			for (uint32_t N = 0; N < N_relabs; ++N) {
				relab_tree.clear();
				const err_type relab_err = make_relabelled(edges_cur, relab_tree);
				if (relab_err != err_type::no_error) {
					cerr << "    For input tree:" << endl;
					cerr << cur_tree << endl;
					cerr << "    Output tree:" << endl;
					cerr << relab_tree << endl;
					return relab_err;
				}

				const bool r = utilities::are_trees_isomorphic(cur_tree, relab_tree);
				if (not r) {
					cerr << ERROR << endl;
					cerr << "    Isomorphism test returned false on isomorphic trees." << endl;
					cerr << "    Tree:" << endl;
					cerr << cur_tree << endl;
					cerr << "    Tree:" << endl;
					cerr << relab_tree << endl;
					return err_type::test_execution;
				}
			}
		}
	}
	return err_type::no_error;
}

template<class Tree, class GEN>
err_type pos_rand_test(ifstream& fin) {
	uint32_t n, N_trees, N_relabs;
	fin >> n >> N_trees >> N_relabs;

	Tree relab_tree;
	GEN Gen(n);

	for (uint32_t t = 0; t < N_trees; ++t) {
		const Tree cur_tree = Gen.make_rand_tree();
		vector<edge> edges_cur = cur_tree.edges();
		if constexpr (std::is_base_of<directed_graph, Tree>::value) {
			relab_tree.init(n);
			relab_tree.set_root(cur_tree.get_root());
		}

		for (uint32_t l = 0; l < N_relabs; ++l) {
			relab_tree.clear();
			const err_type relab_err = make_relabelled(edges_cur, relab_tree);
			if (relab_err != err_type::no_error) {
				cerr << "    For input tree:" << endl;
				cerr << cur_tree << endl;
				cerr << "    For tree:" << endl;
				cerr << relab_tree << endl;
				return relab_err;
			}

			const bool r = utilities::are_trees_isomorphic(cur_tree, relab_tree);
			if (not r) {
				cerr << ERROR << endl;
				cerr << "    Isomorphism test returned false on isomorphic trees." << endl;
				cerr << "    Tree:" << endl;
				cerr << cur_tree << endl;
				cerr << "    Tree:" << endl;
				cerr << relab_tree << endl;
				return err_type::test_execution;
			}
		}
	}
	return err_type::no_error;
}

// ground truth: NON-ISOMORPHIC

template<class Tree, class GEN>
err_type neg_exh_test(ifstream& fin) {
	uint32_t n, N_relabs;
	fin >> n >> N_relabs;

	if constexpr (std::is_base_of<undirected_graph, Tree>::value) {
		if (n > 21) {
			cerr << ERROR << endl;
			cerr << "    Using n>21 (n=" << n << ") is going to freeze the computer." << endl;
			cerr << "    Aborting." << endl;
			return err_type::test_execution;
		}
	}
	else if constexpr (std::is_base_of<directed_graph, Tree>::value) {
		if (n > 18) {
			cerr << ERROR << endl;
			cerr << "    Using n>18 (n=" << n << ") is going to freeze the computer." << endl;
			cerr << "    Aborting." << endl;
			return err_type::test_execution;
		}
	}

	vector<Tree> all_trees;
	GEN Gen(n);
	while (Gen.has_next()) {
		Gen.next();
		all_trees.push_back(Gen.get_tree());
	}

	Tree relab_tree;
	for (size_t i = 0; i < all_trees.size(); ++i) {
		const Tree& ti = all_trees[i];
		for (size_t j = i + 1; j < all_trees.size(); ++j) {

			const Tree& tj = all_trees[j];
			vector<edge> edges_tj = tj.edges();
			if constexpr (std::is_base_of<directed_graph, Tree>::value) {
				relab_tree.init(n);
				relab_tree.set_root(tj.get_root());
			}

			for (uint32_t l = 0; l < N_relabs; ++l) {
				const err_type relab_err = make_relabelled(edges_tj, relab_tree);
				if (relab_err != err_type::no_error) {
					cerr << "    For input tree:" << endl;
					cerr << tj << endl;
					cerr << "    Output tree:" << endl;
					cerr << relab_tree << endl;
					return relab_err;
				}

				const bool r = utilities::are_trees_isomorphic(ti, relab_tree);
				if (r) {
					cerr << ERROR << endl;
					cerr << "    Isomorphism test returned true on non-isomorphic trees." << endl;
					cerr << "    Tree:" << endl;
					cerr << ti << endl;
					cerr << "    Tree:" << endl;
					cerr << relab_tree << endl;
					return err_type::test_execution;
				}
			}

		}
	}
	return err_type::no_error;
}

template<class T, class Exh_Gen, class Rand_Gen>
err_type exe_test_auto(const string& expected, const string& mode, ifstream& fin) {
	if (expected == "positive") {
		if (mode == "exhaustive") { return pos_exh_test<T,Exh_Gen>(fin); }
		else { return pos_rand_test<T,Rand_Gen>(fin); }
	}
	else {
		return neg_exh_test<T,Exh_Gen>(fin);
	}
}

err_type exe_utils_tree_iso_auto(ifstream& fin) {
	string tree_type;
	fin >> tree_type;
	if (tree_type != "free" and tree_type != "rooted") {
		cerr << ERROR << endl;
		cerr << "    Invalid tree type '" << tree_type << "'." << endl;
		cerr << "    Must be one of: free/rooted" << endl;
		return err_type::test_format;
	}
	string expected_result;
	fin >> expected_result;
	if (expected_result != "positive" and expected_result != "negative") {
		cerr << ERROR << endl;
		cerr << "    Invalid expected result '" << expected_result << "'." << endl;
		cerr << "    Must be one of: positive/negative" << endl;
		return err_type::test_format;
	}
	string mode;
	fin >> mode;
	if (mode != "exhaustive" and mode != "random") {
		cerr << ERROR << endl;
		cerr << "    Invalid mode '" << mode << "'." << endl;
		cerr << "    Must be one of: exhasutive/random" << endl;
		return err_type::test_format;
	}

	if (expected_result == "negative" and mode == "random") {
		cerr << ERROR << endl;
		cerr << "    Combination of expected result '" << expected_result << "' and" << endl;
		cerr << "    mode of generation '" << mode << "' is invalid." << endl;
		return err_type::test_format;
	}

	return (tree_type == "free" ?
		exe_test_auto<free_tree,all_ulab_free_trees,rand_ulab_free_trees>
		(expected_result, mode, fin)
		:
		exe_test_auto<rooted_tree,all_ulab_rooted_trees,rand_ulab_rooted_trees>
		(expected_result, mode, fin)
	);
}

// -----------------------------------------------------------------------------

err_type exe_utilities_tree_isomorphism(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	string mode;
	fin >> mode;
	if (mode != "manual" and mode != "automatic") {
		cerr << ERROR << endl;
		cerr << "    Invalid mode '" << mode << "'." << endl;
		cerr << "    Must be one of: manual/automatic" << endl;
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

} // -- namespace exe_tests
