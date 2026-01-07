/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2026
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <algorithm>
#include <fstream>
#include <random>
#include <string>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace tests {

bool command_is_comment(const std::string& s) noexcept
{
	return (s == "/*") or (s.find("/*") != std::string::npos);
}

void process_comment(std::ifstream& fin) noexcept
{
	std::string comment;
	do {
		fin >> comment;
	}
	while (comment.find("*/") == std::string::npos);
}

std::string read_output_string(std::ifstream& fin) noexcept
{

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

void relabel_edges(
	const uint64_t n,
	std::vector<lal::edge>& edges,
	lal::node& r,
	std::mt19937& gen
) noexcept
{
	std::vector<lal::node> relab(n);
	std::iota(relab.begin(), relab.end(), 0);

	std::shuffle(relab.begin(), relab.end(), gen);

	// relabel each vertex according to 'relab'
	for (auto& [s, t] : edges) {
		s = relab[s];
		t = relab[t];
	}
	r = relab[r];
}

void shuffle_graph_edges(
	const uint64_t n,
	std::vector<lal::edge>& edges,
	lal::graphs::undirected_graph& G,
	std::mt19937& gen,
	bool normalize,
	bool check
) noexcept
{
	// shuffle 'n' times
	for (uint64_t i = 0; i < n; ++i) {
		std::shuffle(edges.begin(), edges.end(), gen);
	}

	G.clear();
	G.init(n);
	G.set_edges(edges, normalize, check);
}

void relabel_graph_vertices(
	const uint64_t n,
	std::vector<lal::edge>& edges,
	lal::graphs::undirected_graph& G,
	std::mt19937& gen,
	bool normalize,
	bool check
) noexcept
{
	lal::node dummy = 0;
	relabel_edges(n, edges, dummy, gen);

	G.clear();
	G.init(n);
	G.set_edges(edges, normalize, check);
}

void relabel_tree_vertices(
	const uint64_t n,
	lal::node r,
	std::vector<lal::edge>& edges,
	lal::graphs::rooted_tree& T,
	std::mt19937& gen,
	bool normalize,
	bool check
) noexcept
{
	relabel_edges(n, edges, r, gen);

	T.clear();
	T.init(n);
	T.set_edges(edges, normalize, check);
}

void relabel_tree_vertices(
	const uint64_t n,
	std::vector<lal::edge>& edges,
	lal::graphs::free_tree& T,
	std::mt19937& gen,
	bool normalize,
	bool check
) noexcept
{
	lal::node dummy = 0;
	relabel_edges(n, edges, dummy, gen);

	T.clear();
	T.init(n);
	T.set_edges(edges, normalize, check);
}

} // namespace tests
