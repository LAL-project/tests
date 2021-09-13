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
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/internal/graphs/is_tree.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;
using namespace iterators;

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace properties {

directed_graph make_rand_dgraph(const undirected_graph& g) {
	// coin flips
	default_random_engine gen(1234);
	uniform_int_distribution<int> U(0,1);

	// directed graph
	directed_graph dg(g.get_num_nodes());
	vector<edge> es = g.get_edges();

	const size_t m = es.size();
	for (size_t i = 0; i < m; ++i) {
		edge& e = es[i];

		// add edge in one orientation or the other
		const int c1 = U(gen);
		if (c1 == 1) {
			std::swap(e.first, e.second);
		}

		// add (or not) the opposite edge
		const int c2 = U(gen);
		if (c2 == 1) {
			edge flip = e;
			std::swap(flip.first, flip.second);
			es.push_back(flip);
		}
	}

	// construct graph and finish
	dg.add_edges(es);
	return dg;
}

template<class G>
void enum_E(const G& g) {
	size_t total = 0;

	cout << "Elements of E:" << endl;

	for (E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		cout << "(" << u << "," << v << ")" << endl;
		++total;
	}
	cout << "Total number of elements: " << total << endl;
}

template<class G>
void enum_Q(const G& g) {
	size_t total = 0;

	cout << "Elements of Q:" << endl;

	for (Q_iterator q_it(g); not q_it.end(); q_it.next()) {
		const edge_pair& ep = q_it.get_edge_pair();
		const edge& e1 = ep.first;
		const edge& e2 = ep.second;
		cout << "("
			 << "(" << e1.first << ", " << e1.second << "), "
			 << "(" << e2.first << ", " << e2.second << ")"
			 << ")"
			 << endl;

		++total;
	}
	cout << "Total number of elements: " << total << endl;
}

template<class G>
void Q_size(const G& g) {
	const integer Q = num_pairs_independent_edges_integer(g);
	cout << "size of Q: " << Q << endl;

	const uint64_t dQ = num_pairs_independent_edges(g);
	cout << "size of Q: " << dQ << endl;
}

//

void mmt_deg(const undirected_graph& g, uint64_t p) {
	cout << "(udir) <k^" << p << ">= "
		 << moment_degree_rational(g, p)
		 << endl;
}

void mmt_deg(const directed_graph& g, uint64_t p) {
	cout << "( dir) <k^" << p << ">= "
		 << moment_degree_rational(g, p)
		 << endl;
}

//

err_type mmt_in_deg(const undirected_graph& uG, const directed_graph& dG, uint64_t p) {
	cout << "(graph) <k_out^" << p << ">= "
		 << moment_in_degree_rational(dG, p)
		 << endl;

	const bool is_tree_dG = internal::is_graph_a_tree(dG);
	const bool is_tree_uG = internal::is_graph_a_tree(uG);

	if (is_tree_dG != is_tree_uG) {
		cerr << ERROR << endl;
		cerr << "    Error in deciding whether a graph is a tree or not." << endl;
		cerr << "    It has been determined that an undirected graph uG" << endl;
		cerr << "    " << (is_tree_uG ? "is" : "is not") << " a tree, however" << endl;
		cerr << "    it has been determined that its oriented version dG" << endl;
		cerr << "    " << (is_tree_dG ? "is" : "is not") << " a tree, hence" << endl;
		cerr << "    a contradiction." << endl;
		cerr << "    Is undirected a tree? " << is_tree_uG << endl;
		cerr << "    Is   directed a tree? " << is_tree_dG << endl;
		cerr << "    Undirected graph:" << endl;
		cerr << uG << endl;
		cerr << "    Directed graph:" << endl;
		cerr << dG << endl;
		return err_type::test_execution;
	}

	if (is_tree_dG) {
		const auto n = uG.get_num_nodes();
		for (node r = 0; r < n; ++r) {
			const rooted_tree T(uG, r);
			const auto kin = moment_in_degree_rational(T, p);

			cout << "(rtree at " << r << ") <k_in^" << p << ">= " << kin << endl;
			if (kin != lal::numeric::rational(n-1, n)) {
				cerr << ERROR << endl;
				cerr << "    Moment of in-degree is not equal to the expected value." << endl;
				cerr << "    Expected: " << lal::numeric::rational(n-1, n) << endl;
				cerr << "    Received: " << kin << endl;
				return err_type::test_execution;
			}

		}
	}
	return err_type::no_error;
}

//

err_type mmt_out_deg(const undirected_graph& uG, const directed_graph& dG, uint64_t p) {
	cout << "(graph) <k_out^" << p << ">= "
		 << moment_out_degree_rational(dG, p)
		 << endl;

	const bool is_tree_dG = internal::is_graph_a_tree(dG);
	const bool is_tree_uG = internal::is_graph_a_tree(uG);

	if (is_tree_dG != is_tree_uG) {
		cerr << ERROR << endl;
		cerr << "    Error in deciding whether a graph is a tree or not." << endl;
		cerr << "    It has been determined that an undirected graph uG" << endl;
		cerr << "    " << (is_tree_uG ? "is" : "is not") << " a tree, however" << endl;
		cerr << "    it has been determined that its oriented version dG" << endl;
		cerr << "    " << (is_tree_dG ? "is" : "is not") << " a tree, hence" << endl;
		cerr << "    a contradiction." << endl;
		cerr << "    Is undirected a tree? " << is_tree_uG << endl;
		cerr << "    Is   directed a tree? " << is_tree_dG << endl;
		cerr << "    Undirected graph:" << endl;
		cerr << uG << endl;
		cerr << "    Directed graph:" << endl;
		cerr << dG << endl;
		return err_type::test_execution;
	}

	if (is_tree_dG) {
		for (node r = 0; r < uG.get_num_nodes(); ++r) {
			const rooted_tree T(uG, r);

			cout << "(rtree at " << r << ") <k_out^" << p << ">= "
				 << moment_out_degree_rational(T, p)
				 << endl;
		}
	}
	return err_type::no_error;
}

//

void hubiness_coefficient(const undirected_graph& g) {
	if (not internal::is_graph_a_tree(g)) { return; }
	if (g.get_num_nodes() > 3) {
		rational h = hubiness_rational(g);
		cout << "hubiness= " << h << endl;
	}
}

void MHD(const undirected_graph& g, node r) {
	if (not internal::is_graph_a_tree(g)) { return; }
	const free_tree t(g);
	const rooted_tree R(t, r);
	const rational mhd = mean_hierarchical_distance_rational(R);
	cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << endl;
}

err_type exe_properties_general(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_instructions({
		"enumerate_E", "enumerate_E_rand_dir",
		"enumerate_Q", "enumerate_Q_rand_dir",
		"Q_size", "Q_size_rand_dir",
		"mmt_deg", "mmt_in_deg", "mmt_out_deg",
		"hubiness_coefficient", "Mean_Hierarchical_Distance"
	});

	string ins;
	string all_instructions = "";
	while (fin >> ins) {
		all_instructions += ins + " ";
	}

	// The input file has been parsed completely.
	// It is time to execute the instructions for each graph.

	undirected_graph uG;
	directed_graph dG;
	for (size_t i = 0; i < inputs.size(); ++i) {

		err_type r;

		r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, uG);
		if (r != err_type::no_error) { return r; }

		r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, dG);
		if (r != err_type::no_error) { return r; }

		// this graph may have edges of the form
		// (u,v)  ..  (v,u)
		const directed_graph rdG = make_rand_dgraph(uG);

		stringstream ss(all_instructions);
		while (ss >> ins) {
			if (ins == "enumerate_E") {
				enum_E(uG);
			}
			else if (ins == "enumerate_E_rand_dir") {
				cout << rdG << endl;
				enum_E(rdG);
			}
			else if (ins == "enumerate_Q") {
				enum_Q(uG);
			}
			else if (ins == "enumerate_Q_rand_dir") {
				cout << rdG << endl;
				enum_Q(rdG);
			}
			else if (ins == "Q_size") {
				Q_size(uG);
			}
			else if (ins == "Q_size_rand_dir") {
				cout << rdG << endl;
				Q_size(rdG);
			}
			else if (ins == "mmt_deg") {
				uint64_t p;
				ss >> p;
				mmt_deg(uG, p);
				mmt_deg(rdG, p);
			}
			else if (ins == "mmt_in_deg") {
				uint64_t p;
				ss >> p;
				r = mmt_in_deg(uG, dG, p);
				if (r != err_type::no_error) { return r; }
			}
			else if (ins == "mmt_out_deg") {
				uint64_t p;
				ss >> p;
				r = mmt_out_deg(uG, dG, p);
				if (r != err_type::no_error) { return r; }
			}
			else if (ins == "hubiness_coefficient") {
				hubiness_coefficient(uG);
			}
			else if (ins == "Mean_Hierarchical_Distance") {
				node root;
				ss >> root;
				MHD(uG, root);
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Invalid instruction." << endl;
				cerr << "    Allowed instructions:" << endl;
				for (const string& s : allowed_instructions) {
					cout << "        " << s << endl;
				}
				cerr << "    Instead, '" << ins << "' was found." << endl;
				return err_type::test_format;
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
