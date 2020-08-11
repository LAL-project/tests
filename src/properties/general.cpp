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
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/internal/graphs/trees/is_tree.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"

namespace exe_tests {

void enum_Q(const ugraph& g) {
	const vector<edge_pair> Q = g.Q();

	cout << "Elements of Q (" << Q.size() << "):" << endl;
	for (const edge_pair& ep : Q) {
		const edge& e1 = ep.first;
		const edge& e2 = ep.second;
		cout << "("
			 << "(" << e1.first << ", " << e1.second << "), "
			 << "(" << e2.first << ", " << e2.second << ")"
			 << ")" << endl;
	}
}

void Q_size(const ugraph& g) {
	const integer Q = size_Q_integer(g);
	cout << "size of Q: " << Q << endl;

	const uint32_t dQ = size_Q(g);
	cout << "size of Q: " << dQ << endl;
}

void mmt_deg(const ugraph& g, uint32_t p) {
	const rational kp = mmt_degree_rational(g, p);
	cout << "<k^" << p << ">= " << kp << endl;
}

void hubiness_coefficient(const ugraph& g) {
	if (not internal::is_graph_a_tree(g)) { return; }
	if (g.n_nodes() > 3) {
		rational h = hubiness_rational(g);
		cout << "hubiness= " << h << endl;
	}
}

void MHD(const ugraph& g, node r) {
	if (not internal::is_graph_a_tree(g)) { return; }
	const ftree t(g);
	const rtree R(t, r);
	const rational mhd = mean_hierarchical_distance_rational(R);
	cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << endl;
}

err_type exe_properties_general(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_instructions({
		"enumerate_Q", "Q_size", "mmt_deg",
		"hubiness_coefficient", "Mean_Hierarchical_Distance"
	});

	string ins;
	string all_instructions = "";
	while (fin >> ins) {
		all_instructions += ins + " ";
	}

	// The input file has been parsed completely.
	// It is time to execute the instructions
	// for each graph.

	ugraph G;
	for (size_t i = 0; i < inputs.size(); ++i) {
		err_type r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, G);
		if (r != err_type::no_error) {
			return r;
		}

		stringstream ss(all_instructions);
		while (ss >> ins) {
			if (ins == "enumerate_Q") {
				enum_Q(G);
			}
			else if (ins == "Q_size") {
				Q_size(G);
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
				cerr << "    Instruction not recognised." << endl;
				cerr << "    Allowed instructions:" << endl;
				for (const string& s : allowed_instructions) {
					cout << "        " << s << endl;
				}
				cerr << "    Instead, '" << ins << "' was found." << endl;
				return err_type::test_format_error;
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
