/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/rutree.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/mhd.hpp>
#include <lal/numeric/rational.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace properties;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"

namespace exe_tests {

void enum_Q(const ugraph& g) {
	vector<edge_pair> Q = g.Q();

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
	integer Q = size_Q_integer(g);
	cout << "size of Q: " << Q << endl;

	uint64_t dQ = size_Q(g);
	cout << "size of Q: " << dQ << endl;
}

void snd_mmt_deg(const ugraph& g) {
	rational k2 = second_mmt_degree_rational(g);
	cout << "<k^2>= " << k2 << endl;
}

void trd_mmt_deg(const ugraph& g) {
	rational k3 = third_mmt_degree_rational(g);
	cout << "<k^3>= " << k3 << endl;
}

void hubiness_coefficient(const ugraph& g) {
	if (g.n_nodes() > 3) {
		rational h = hubiness_rational(g);
		cout << "hubiness= " << h << endl;
	}
}

void MHD(const ugraph& g, node r) {
	rutree R(g, r);
	rational mhd = MHD_rational(R);
	cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << endl;
}

err_type exe_properties_general(ifstream& fin) {
	const set<string> allowed_instructions({
		"enumerate_Q", "Q_size", "2nd_mmt_deg",
		"3rd_mmt_deg", "hubiness_coefficient",
		"Mean_Hierarchical_Distance"
	});

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n;
	fin >> n;
	vector<string> graph_name(n), graph_format(n);
	for (size_t i = 0; i < n; ++i) {
		fin >> graph_name[i] >> graph_format[i];
	}

	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string ins;
	string all_instructions = "";
	while (fin >> ins) {
		all_instructions += ins + " ";
	}

	// The input file has been parsed completely.
	// It is time to execute the instructions
	// for each graph.

	ugraph G;
	for (size_t i = 0; i < n; ++i) {
		err_type r = io_wrapper::read_graph(graph_name[i], graph_format[i], G);
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
			else if (ins == "2nd_mmt_deg") {
				snd_mmt_deg(G);
			}
			else if (ins == "3rd_mmt_deg") {
				trd_mmt_deg(G);
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
