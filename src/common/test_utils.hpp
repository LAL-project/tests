/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <fstream>
#include <vector>
#include <string>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace tests {

bool command_is_comment(const std::string& s) noexcept;

void process_comment(std::ifstream& fin) noexcept;

std::string read_output_string(std::ifstream& fin) noexcept;

/* -------------------------------------------------------------------------- */
/* ----- Utilities related to the library -- not so much to the tests ------- */

void shuffle_graph_edges(
	std::vector<lal::edge>& edges, lal::graphs::undirected_graph& G,
	bool normalise = false, bool check = false
)
noexcept;

void relabel_graph_vertices(
	std::vector<lal::edge>& edges, lal::graphs::undirected_graph& G,
	bool normalise = false, bool check = false
)
noexcept;

void relabel_tree_vertices(
	std::vector<lal::edge>& edges, lal::graphs::rooted_tree& T,
	bool normalise = false, bool check = false
)
noexcept;

void relabel_tree_vertices(
	std::vector<lal::edge>& edges, lal::graphs::free_tree& T,
	bool normalise = false, bool check = false
)
noexcept;

} // -- namespace tests
