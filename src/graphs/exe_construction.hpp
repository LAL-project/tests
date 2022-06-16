/*********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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

#pragma once

// C++ includes
#include <vector>
#include <map>
#include <set>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace graphs {

template <typename G>
inline bool equal_graphs(const G& g1, const G& g2)
{
	if (g1.get_num_nodes() != g2.get_num_nodes()) { return false; }

	const auto get_neighs =
	[](const G& g, lal::node u) {
		if constexpr (std::is_base_of_v<lal::graphs::directed_graph, G>) {
			return g.get_out_neighbours(u);
		}
		else {
			return g.get_neighbours(u);
		}
	};

	const uint64_t n = g1.get_num_nodes();
	for (lal::node u = 0; u < n; ++u) {
		const lal::neighbourhood& n1 = get_neighs(g1, u);
		const lal::neighbourhood& n2 = get_neighs(g2, u);

		if (n1.size() != n2.size()) { return false; }
		std::set<lal::node> sn1(n1.begin(), n1.end());
		std::set<lal::node> sn2(n2.begin(), n2.end());
		if (n1.size() != sn1.size()) { return false; }
		if (n2.size() != sn2.size()) { return false; }
		if (sn1 != sn2) { return false; }
	}
	if (g1.is_normalised() != g2.is_normalised()) { return false; }
	return true;
}

#define edge_out(e) "(" << e.first << ", " << e.second << ")"
#define edge_pair_out(p) "(" << edge_out(p.first) << ", " << edge_out(p.second) << ")"

#define map_has(map, key) (map.find(key) != map.end())

#define exists_variable(var) map_has(gtypes, var)

#define graph_type(var)	(gtypes[var])

#define in_collection(k, C) (std::find(C.begin(), C.end(), k) != C.end())

#define UGRAPH "ugraph"
#define DGRAPH "dgraph"
#define FTREE "ftree"
#define RTREE "rtree"

// directed rooted tree types
#define RTREE_arborescence rooted_tree::rooted_tree_type::arborescence
#define RTREE_anti_arborescence rooted_tree::rooted_tree_type::anti_arborescence
#define RTREE_none rooted_tree::rooted_tree_type::none

#define message_in_func(f) std::cerr << "    -- In '" << f << "' --\n";

static const std::vector<std::string> graph_types(
	{UGRAPH, DGRAPH}
);
static const std::vector<std::string> directed_types(
	{DGRAPH, RTREE}
);
static const std::vector<std::string> undirected_types(
	{UGRAPH, FTREE}
);
static const std::vector<std::string> tree_types(
	{FTREE, RTREE}
);
static const std::vector<std::string> rooted_tree_types(
	{RTREE}
);
static const std::vector<std::string> free_tree_types(
	{FTREE}
);
static const std::vector<std::string> all_types(
	{UGRAPH, DGRAPH, FTREE, RTREE}
);

/* ASSERTIONS */

#define assert_string_is_rooted_tree_type(assertion, t)					\
if (t != "arborescence" and t != "anti_arborescence" and t != "none") {	\
	std::cerr << ERROR << '\n';											\
	message_in_func(assertion)											\
	std::cerr << "    Invalid drtree type '" << t << "'.\n";			\
	return tests::err_type::test_format;										\
}

#define assert_correct_boolean(assertion, str)									\
if (str != "true" and str != "false") {											\
	std::cerr << ERROR << '\n';													\
	message_in_func(assertion)													\
	std::cerr << "    Invalid Boolean value.\n";								\
	std::cerr << "    Received '" << str << "'. Valid values: true/false.\n";	\
	return tests::err_type::test_format;										\
}

#define assert_exists_variable(FUNC, var)									\
if (not exists_variable(var)) {												\
	std::cerr << ERROR << '\n';												\
	message_in_func(FUNC)													\
	std::cerr << "    Variable graph '" << var << "' not declared.\n";		\
	return tests::err_type::test_execution;									\
}

#define assert_not_exists_variable(FUNC, var)								\
if (exists_variable(var)) {													\
	std::cerr << ERROR << '\n';												\
	message_in_func(FUNC)													\
	std::cerr << "    Variable graph '" << var << "' already declared.\n";	\
	return tests::err_type::test_execution;									\
}

#define assert_correct_graph_type(FUNC, type, TYPES)				\
if (not in_collection(type, TYPES)) {								\
	std::cerr << ERROR << '\n';										\
	message_in_func(FUNC)											\
	std::cerr << "    Graph type '" << type << "' is incorrect.\n";	\
	return tests::err_type::test_execution;							\
}

#define assert_equal_types(assertion, var1, var2)												\
if (graph_type(var1) != graph_type(var2)) {														\
	std::cerr << ERROR << '\n';																	\
	message_in_func(assertion)																	\
	std::cerr << "    Graphs '" << var1 << "' and '" << var2 << "' are not of the same type.\n";\
	std::cerr << "    Type of graph " << var1 << ": " << graph_type(var1) << '\n';				\
	std::cerr << "    Type of graph " << var2 << ": " << graph_type(var2) << '\n';				\
	return tests::err_type::test_execution;														\
}

/* USEFUL FUNCTIONS */

#define are_graphs_equal(v1, v2)																\
[&]() -> bool {																					\
	if (graph_type(v1) == UGRAPH)	{ return equal_graphs(ugraphvars[v1], ugraphvars[v2]); }	\
	if (graph_type(v1) == DGRAPH)	{ return equal_graphs(dgraphvars[v1], dgraphvars[v2]); }	\
	if (graph_type(v1) == FTREE)	{ return equal_graphs(ftreevars[v1], ftreevars[v2]); }		\
	if (graph_type(v1) == RTREE)	{ return equal_graphs(rtreevars[v1], rtreevars[v2]); }		\
	return false;																				\
}()


// macros for ALL graphs
#define if_ffunction(v, FUNC)									\
if (graph_type(v) == UGRAPH)		{ FUNC(ugraphvars[v]); }	\
else if (graph_type(v) == DGRAPH)	{ FUNC(dgraphvars[v]); }	\
else if (graph_type(v) == FTREE)	{ FUNC(ftreevars[v]); }		\
else if (graph_type(v) == RTREE)	{ FUNC(rtreevars[v]); }

#define if_mfunction(v, FUNC)									\
if (graph_type(v) == UGRAPH)		{ ugraphvars[v].FUNC; }		\
else if (graph_type(v) == DGRAPH)	{ dgraphvars[v].FUNC; }		\
else if (graph_type(v) == FTREE)	{ ftreevars[v].FUNC; }		\
else if (graph_type(v) == RTREE)	{ rtreevars[v].FUNC; }

#define ffunction(v, FUNC)											\
[&]() {																\
	if (graph_type(v) == UGRAPH)	{ return FUNC(ugraphvars[v]); }	\
	if (graph_type(v) == DGRAPH)	{ return FUNC(dgraphvars[v]); }	\
	if (graph_type(v) == FTREE)		{ return FUNC(ftreevars[v]); }	\
	return FUNC(rtreevars[v]);										\
}()

#define mfunction(v, FUNC)											\
[&]() {																\
	if (graph_type(v) == UGRAPH)	{ return ugraphvars[v].FUNC; }	\
	if (graph_type(v) == DGRAPH)	{ return dgraphvars[v].FUNC; }	\
	if (graph_type(v) == FTREE)		{ return ftreevars[v].FUNC; }	\
	return rtreevars[v].FUNC;										\
}()




// macros for UNDIRECTED graphs
#define if_ffunction_undir_graphs(v, FUNC)						\
if (graph_type(v) == UGRAPH)		{ FUNC(ugraphvars[v]); }	\
else if (graph_type(v) == FTREE)	{ FUNC(ftreevars[v]); }

#define if_mfunction_undir_graphs(v, FUNC)						\
if (graph_type(v) == UGRAPH)		{ ugraphvars[v].FUNC; }		\
else if (graph_type(v) == FTREE)	{ ftreevars[v].FUNC; }

#define ffunction_undir_graphs(v, FUNC)									\
[&]() {																	\
	if (graph_type(v) == UGRAPH)	{ return FUNC(ugraphvars[v]); }		\
	return FUNC(ftreevars[v]);											\
}()

#define mfunction_undir_graphs(v, FUNC)									\
[&]() {																	\
	if (graph_type(v) == UGRAPH)	{ return ugraphvars[v].FUNC; }		\
	return ftreevars[v].FUNC;											\
}()




// macros for DIRECTED graphs
#define if_ffunction_dir_graphs(v, FUNC)						\
if (graph_type(v) == DGRAPH)		{ FUNC(dgraphvars[v]); }	\
else if (graph_type(v) == RTREE)	{ FUNC(rtreevars[v]); }

#define if_mfunction_dir_graphs(v, FUNC)						\
if (graph_type(v) == DGRAPH)		{ dgraphvars[v].FUNC; }		\
else if (graph_type(v) == RTREE)	{ rtreevars[v].FUNC; }

#define ffunction_dir_graphs(v, FUNC)									\
[&]() {																	\
	if (graph_type(v) == DGRAPH)	{ return FUNC(dgraphvars[v]); }		\
	return FUNC(rtreevars[v]);											\
}()

#define mfunction_dir_graphs(v, FUNC)									\
[&]() {																	\
	if (graph_type(v) == DGRAPH)	{ return dgraphvars[v].FUNC; }		\
	return rtreevars[v].FUNC;											\
}()




// macros for ALL trees
#define if_ffunction_trees(v, FUNC)									\
if (graph_type(v) == FTREE)			{ FUNC(ftreevars[v]); }			\
else if (graph_type(v) == RTREE)	{ FUNC(rtreevars[v]); }

#define if_mfunction_trees(v, FUNC)									\
if (graph_type(v) == FTREE)			{ ftreevars[v].FUNC; }			\
else if (graph_type(v) == RTREE)	{ rtreevars[v].FUNC; }

#define ffunction_trees(v, FUNC)									\
[&]() {																\
	if (graph_type(v) == FTREE)		{ return FUNC(ftreevars[v]); }	\
	return FUNC(rtreevars[v]);										\
}()

#define mfunction_trees(v, FUNC)									\
[&]() {																\
	if (graph_type(v) == FTREE)		{ return ftreevars[v].FUNC; }	\
	return rtreevars[v].FUNC;										\
}()


// -----------------------
// macros for ROOTED trees

#define if_ffunction_rtrees(v, FUNC)								\
if (graph_type(v) == RTREE)			{ FUNC(rtreevars[v]); }

#define if_mfunction_rtrees(v, FUNC)								\
if (graph_type(v) == RTREE)			{ rtreevars[v].FUNC; }

#define ffunction_rtrees(v, FUNC)	\
[&]() {								\
	 return FUNC(rtreevars[v]);		\
}()

#define mfunction_rtrees(v, FUNC)	\
[&]() {								\
	return rtreevars[v].FUNC;		\
}()


#define output_graph(v)														\
if (graph_type(v) == UGRAPH)		{ std::cerr << ugraphvars[v] << '\n'; }	\
else if (graph_type(v) == DGRAPH)	{ std::cerr << dgraphvars[v] << '\n'; }	\
else if (graph_type(v) == FTREE)	{ std::cerr << ftreevars[v] << '\n'; }	\
else if (graph_type(v) == RTREE)	{ std::cerr << rtreevars[v] << '\n'; }

#define output_graph_stdout(v)												\
if (graph_type(v) == UGRAPH)		{ std::cout << ugraphvars[v] << '\n'; }	\
else if (graph_type(v) == DGRAPH)	{ std::cout << dgraphvars[v] << '\n'; }	\
else if (graph_type(v) == FTREE)	{ std::cout << ftreevars[v] << '\n'; }	\
else if (graph_type(v) == RTREE)	{ std::cout << rtreevars[v] << '\n'; }


#define WRONG_TYPE_EXT(my_assert, g, T, my_type)					\
std::cerr << ERROR << '\n';											\
message_in_func(my_assert)											\
std::cerr << "    Graph '" << g << "' is not " << T << ".\n";		\
std::cerr << "    Graph '" << g << "' is " << my_type << ".\n";

#define WRONG_TYPE(my_assert, g, T) WRONG_TYPE_EXT(my_assert, g, T, graph_type(g))

#define assert_is_rtree(g1, FUNC)									\
if (not mfunction_rtrees(g1, is_tree())) {							\
	std::cerr << "ERROR\n";											\
	message_in_func(FUNC)											\
	std::cerr << "    Tree '" << g1 << "' is not an actual tree.\n";\
	return tests::err_type::test_execution;							\
}																	\
if (not mfunction_rtrees(g1, has_root())) {							\
	std::cerr << "ERROR\n";											\
	message_in_func(FUNC)											\
	std::cerr << "    Tree '" << g1 << "' does not have a root.\n";	\
	return tests::err_type::test_execution;							\
}																	\
if (not mfunction_rtrees(g1, is_directed())) {						\
	std::cerr << "ERROR\n";											\
	message_in_func(FUNC)											\
	std::cerr << "    Tree '" << g1 << "' is not directed.\n";		\
	return tests::err_type::test_execution;							\
}

#define rooted_tree_type_to_string(t)							\
[](rooted_tree::rooted_tree_type __t) -> std::string {			\
	switch (__t) {												\
		case rooted_tree::rooted_tree_type::arborescence:		\
			return "arborescence";								\
		case rooted_tree::rooted_tree_type::anti_arborescence:	\
			return "anti_arborescence";							\
		default:												\
			return "none";										\
	}															\
}(t)

std::vector<lal::edge> enumerate_edges_brute_force(const lal::graphs::undirected_graph& g);
std::vector<lal::edge> enumerate_edges_brute_force(const lal::graphs::directed_graph& g);
bool share_vertices(const lal::edge_pair& st_uv);
std::vector<lal::edge_pair> enumerate_Q_brute_force(const lal::graphs::undirected_graph& g);
std::vector<lal::edge_pair> enumerate_Q_brute_force(const lal::graphs::directed_graph& g);

err_type process_assert(
	const std::string& assert_what,
	std::map<std::string, lal::graphs::undirected_graph>& ugraphvars,
	std::map<std::string, lal::graphs::directed_graph>& dgraphvars,
	std::map<std::string, lal::graphs::free_tree>& treevars,
	std::map<std::string, lal::graphs::rooted_tree>& rtreevars,
	std::map<std::string, std::string>& gtypes,
	std::ifstream& fin
);

} // -- namespace graphs
} // -- namespace tests
