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
// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace tests {

bool command_is_comment(const std::string& s) noexcept
{ return (s == "/*") or (s.find("/*") != std::string::npos); }

void process_comment(std::ifstream& fin) noexcept {
	std::string comment;
	do {
		fin >> comment;
	}
	while (comment.find("*/") == std::string::npos);
}

std::string read_output_string(std::ifstream& fin) noexcept {

	std::string total_message = "";
	std::string msg;

	// first std::string
	fin >> msg;
	total_message += msg.substr(1, msg.length());

	// first std::string is also last std::string
	if (msg[msg.length() - 1] == '"') {
		return total_message.substr(0, total_message.length() - 1);
	}

	// read the whole std::string
	while (fin >> msg and msg.find('"') == std::string::npos) {
		total_message += " " + msg;
	}

	// process last std::string
	total_message += " " + msg.substr(0, msg.length() - 1);
	return total_message;
}

/* -------------------------------------------------------------------------- */
/* ----- Utilities related to the library -- not so much to the tests ------- */

#define to_uint64(x) static_cast<uint64_t>(x)

void relabel_edges(const uint64_t n, std::vector<lal::edge>& edges, lal::node& r) noexcept {
	std::mt19937 gen(1234);

	std::vector<lal::node> relab(n);
	std::iota(relab.begin(), relab.end(), 0);

	// relabel 'n' times
	for (uint64_t i = 0; i < n; ++i) {
		std::shuffle(relab.begin(), relab.end(), gen);

		// relabel each vertex according to 'relab'
		for (auto& [s,t] : edges) {
			s = relab[s];
			t = relab[t];
		}
		r = relab[r];
	}
}

void shuffle_graph_edges
(std::vector<lal::edge>& edges, lal::graphs::undirected_graph& G, bool normalise, bool check)
noexcept
{
	const uint64_t n = G.get_num_nodes();
	std::mt19937 gen(1234);

	// shuffle 'n' times
	for (uint64_t i = 0; i < n; ++i) {
		std::shuffle(edges.begin(), edges.end(), gen);
	}

	G.clear();
	G.init(n);
	G.set_edges(edges, normalise, check);
}

void relabel_graph_vertices(
	std::vector<lal::edge>& edges, lal::graphs::undirected_graph& G, bool normalise, bool check
)
noexcept
{
	const uint64_t n = G.get_num_nodes();
	lal::node dummy = 0;
	relabel_edges(G.get_num_nodes(), edges, dummy);

	G.clear();
	G.init(n);
	G.set_edges(edges, normalise, check);
}

void relabel_tree_vertices(
	std::vector<lal::edge>& edges, lal::graphs::rooted_tree& T, bool normalise, bool check
)
noexcept
{
	lal::node r = T.get_root();
	relabel_edges(T.get_num_nodes(), edges, r);

	T.clear();
	T.init(to_uint64(edges.size() + 1));
	T.set_edges(edges, normalise, check);
}

void relabel_tree_vertices(
	std::vector<lal::edge>& edges, lal::graphs::free_tree& T, bool normalise, bool check
)
noexcept
{
	lal::node dummy = 0;
	relabel_edges(T.get_num_nodes(), edges, dummy);

	T.clear();
	T.init(to_uint64(edges.size() + 1));
	T.set_edges(edges, normalise, check);
}

} // -- namespace tests
