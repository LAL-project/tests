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

void mmt_deg(const undirected_graph& g, uint32_t p) {
	const rational kp = moment_degree_rational(g, p);
	cout << "<k^" << p << ">= " << kp << endl;
}

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
		"mmt_deg",
		"hubiness_coefficient", "Mean_Hierarchical_Distance"
	});

	string ins;
	string all_instructions = "";
	while (fin >> ins) {
		all_instructions += ins + " ";
	}

	// The input file has been parsed completely.
	// It is time to execute the instructions for each graph.

	undirected_graph G;
	for (size_t i = 0; i < inputs.size(); ++i) {
		const err_type r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, G);
		if (r != err_type::no_error) {
			return r;
		}

		stringstream ss(all_instructions);
		while (ss >> ins) {
			if (ins == "enumerate_E") {
				enum_E(G);
			}
			else if (ins == "enumerate_E_rand_dir") {
				const directed_graph dG = make_rand_dgraph(G);
				cout << dG << endl;
				enum_E(dG);
			}
			else if (ins == "enumerate_Q") {
				enum_Q(G);
			}
			else if (ins == "enumerate_Q_rand_dir") {
				const directed_graph dG = make_rand_dgraph(G);
				cout << dG << endl;
				enum_Q(dG);
			}
			else if (ins == "Q_size") {
				Q_size(G);
			}
			else if (ins == "Q_size_rand_dir") {
				const directed_graph dG = make_rand_dgraph(G);
				cout << dG << endl;
				Q_size(dG);
			}
			else if (ins == "mmt_deg") {
				uint32_t p;
				ss >> p;
				mmt_deg(G, p);
			}
			else if (ins == "hubiness_coefficient") {
				hubiness_coefficient(G);
			}
			else if (ins == "Mean_Hierarchical_Distance") {
				node root;
				ss >> root;
				MHD(G, root);
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
