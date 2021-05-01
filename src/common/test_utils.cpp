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
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <set>
using namespace std;

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
using namespace lal;
using namespace graphs;

namespace tests {

bool command_is_comment(const string& s) noexcept
{ return (s == "/*") or (s.find("/*") != string::npos); }

void process_comment(ifstream& fin) noexcept {
	string comment;
	do {
		fin >> comment;
	}
	while (comment.find("*/") == string::npos);
}

string read_output_string(ifstream& fin) noexcept {

	string total_message = "";
	string msg;

	// first string
	fin >> msg;
	total_message += msg.substr(1, msg.length());

	// first string is also last string
	if (msg[msg.length() - 1] == '"') {
		return total_message.substr(0, total_message.length() - 1);
	}

	// read the whole string
	while (fin >> msg and msg.find('"') == string::npos) {
		total_message += " " + msg;
	}

	// process last string
	total_message += " " + msg.substr(0, msg.length() - 1);
	return total_message;
}

/* -------------------------------------------------------------------------- */
/* ----- Utilities related to the library -- not so much to the tests ------- */

vector<node> invlinarr(const linear_arrangement& arr) noexcept {
	vector<node> ilin(arr.size());
	for (uint32_t p : arr) { ilin[ arr[p] ] = p; }
	return ilin;
}

#define to_uint32(x) static_cast<uint32_t>(x)

void relabel_edges(const uint32_t n, vector<edge>& edges, node& r) noexcept {
	std::mt19937 gen(1234);

	vector<node> relab(n);
	std::iota(relab.begin(), relab.end(), 0);

	// relabel 'n' times
	for (uint32_t i = 0; i < n; ++i) {
		std::shuffle(relab.begin(), relab.end(), gen);

		// relabel each vertex according to 'relab'
		for (auto& [s,t] : edges) {
			s = relab[s];
			t = relab[t];
		}
		r = relab[r];
	}
}

void shuffle_graph_edges(
	vector<edge>& edges, undirected_graph& G, bool normalise, bool check
)
noexcept
{
	const uint32_t n = G.get_num_nodes();
	std::mt19937 gen(1234);

	// shuffle 'n' times
	for (uint32_t i = 0; i < n; ++i) {
		std::shuffle(edges.begin(), edges.end(), gen);
	}

	G.clear();
	G.init(n);
	G.set_edges(edges, normalise, check);
}

void relabel_graph_vertices(
	vector<edge>& edges, undirected_graph& G, bool normalise, bool check
)
noexcept
{
	const uint32_t n = G.get_num_nodes();
	node dummy = 0;
	relabel_edges(G.get_num_nodes(), edges, dummy);

	G.clear();
	G.init(n);
	G.set_edges(edges, normalise, check);
}

void relabel_tree_vertices(
	vector<edge>& edges, rooted_tree& T, bool normalise, bool check
)
noexcept
{
	node r = T.get_root();
	relabel_edges(T.get_num_nodes(), edges, r);

	T.clear();
	T.init(to_uint32(edges.size() + 1));
	T.set_edges(edges, normalise, check);
	T.set_valid_orientation(true);
}

void relabel_tree_vertices(
	vector<edge>& edges, free_tree& T, bool normalise, bool check
)
noexcept
{
	node dummy = 0;
	relabel_edges(T.get_num_nodes(), edges, dummy);

	T.clear();
	T.init(to_uint32(edges.size() + 1));
	T.set_edges(edges, normalise, check);
}

} // -- namespace tests
