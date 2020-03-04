/*********************************************************************
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

#pragma once

// C++ includes
#include <vector>
#include <map>
#include <set>

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/graphs/dgraph.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/utree.hpp>
#include <lal/graphs/dtree.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/graphs/drtree.hpp>

// custom includes
#include "exe_tests.hpp"




template<class G>
inline bool equal_graphs(const G& g1, const G& g2) {
	if (g1.n_nodes() != g2.n_nodes()) { return false; }
	const uint32_t n = g1.n_nodes();
	for (uint32_t i = 0; i < n; ++i) {
		const lal::neighbourhood& n1 = g1.get_neighbours(i);
		const lal::neighbourhood& n2 = g2.get_neighbours(i);
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

template<class G>
void make_disjoint_union(
	const std::string& v1, const std::string& v2, const std::string& v3,
	std::map<std::string, G>& vars
)
{
	G g1 = vars[v2];
	g1.disjoint_union(vars[v3]);
	vars[v1] = g1;
}

#define edge_out(e) "(" << e.first << ", " << e.second << ")"
#define edge_pair_out(p) "(" << edge_out(p.first) << ", " << edge_out(p.second) << ")"

#define map_has(map, key) (map.find(key) != map.end())

#define exists_variable(var) map_has(gtypes, var)

#define graph_type(var)	(gtypes[var])

#define in_collection(k, C) (std::find(C.begin(), C.end(), k) != C.end())

#define UGRAPH "ugraph"
#define DGRAPH "dgraph"
#define UTREE "utree"
#define DTREE "dtree"
#define URTREE "urtree"
#define DRTREE "drtree"

// directed rooted tree types
#define DRTREE_arborescence drtree::drtree_type::arborescence
#define DRTREE_anti_arborescence drtree::drtree_type::anti_arborescence
#define DRTREE_none drtree::drtree_type::none

#define message_in_func(f) cerr << "    -- In '" << f << "' --" << endl;

static const std::vector<std::string> dir_undir_types(
	{UGRAPH, DGRAPH}
);
static const std::vector<std::string> directed_types(
	{DGRAPH, DRTREE, DTREE}
);
static const std::vector<std::string> drtree_types(
	{DRTREE}
);
static const std::vector<std::string> urtree_types(
	{URTREE}
);
static const std::vector<std::string> utree_types(
	{UTREE}
);
static const std::vector<std::string> rtree_types(
	{URTREE, DRTREE}
);
static const std::vector<std::string> tree_types(
	{UTREE, DTREE, URTREE, DRTREE}
);
static const std::vector<std::string> all_types(
	{UGRAPH, DGRAPH, UTREE, DTREE, URTREE, DRTREE}
);

/* ASSERTIONS */

#define assert_correct_file_type(assertion, ft)								\
	if (ft != "edge_list") {												\
		cerr << ERROR << endl;												\
		message_in_func(assertion)											\
		cerr << "    Invalid file type '" << ft << "'." << endl;			\
		return err_type::test_format_error;									\
	}

#define assert_string_is_drtree_type(assertion, t)							\
	if (t != "arborescence" and t != "anti_arborescence" and t != "none") {	\
		cerr << ERROR << endl;												\
		message_in_func(assertion)											\
		cerr << "    Invalid drtree type '" << t << "'." << endl;			\
		return err_type::test_format_error;									\
	}

#define assert_correct_normalise(assertion, str)									\
	if (norm != "true" and norm != "false") {										\
		cerr << ERROR << endl;														\
		message_in_func(assertion)													\
		cerr << "    Invalid value for boolean in add_edge command." << endl;		\
		cerr << "    Received '" << norm << "'. Valid values: true/false." << endl;	\
		return err_type::test_format_error;											\
	}

#define assert_exists_variable(FUNC, var)									\
	if (not exists_variable(var)) {											\
		cerr << ERROR << endl;												\
		message_in_func(FUNC)												\
		cerr << "    Variable graph '" << var << "' not declared." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_correct_graph_type(FUNC, type, TYPES)						\
	if (not in_collection(type, TYPES)) {									\
		cerr << ERROR << endl;												\
		message_in_func(FUNC)												\
		cerr << "    Graph type '" << type << "' is incorrect." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_equal_types(assertion, var1, var2)														\
	if (graph_type(var1) != graph_type(var2)) {															\
		cerr << ERROR << endl;																			\
		message_in_func(assertion)																	\
		cerr << "    Graphs '" << var1 << "' and '" << var2 << "' are not of the same type." << endl;	\
		cerr << "    Type of graph " << var1 << ": " << graph_type(var1) << endl;						\
		cerr << "    Type of graph " << var2 << ": " << graph_type(var2) << endl;						\
		return err_type::test_exe_error;																\
	}

/* USEFUL FUNCTIONS */

#define are_graphs_equal(v1, v2)																	\
	[&]() -> bool {																					\
		if (graph_type(v1) == UGRAPH)	{ return equal_graphs(dgraphvars[v1], dgraphvars[v2]); }	\
		if (graph_type(v1) == DGRAPH)	{ return equal_graphs(ugraphvars[v1], ugraphvars[v2]); }	\
		if (graph_type(v1) == UTREE)	{ return equal_graphs(utreevars[v1], utreevars[v2]); }		\
		if (graph_type(v1) == DTREE)	{ return equal_graphs(dtreevars[v1], dtreevars[v2]); }		\
		if (graph_type(v1) == URTREE)	{ return equal_graphs(urtreevars[v1], urtreevars[v2]); }	\
		if (graph_type(v1) == DRTREE)	{ return equal_graphs(drtreevars[v1], drtreevars[v2]); }	\
		return false;																				\
	}()


// macros for ALL graphs
#define if_ffunction(v, FUNC)										\
	if (graph_type(v) == UGRAPH)		{ FUNC(ugraphvars[v]); }	\
	else if (graph_type(v) == DGRAPH)	{ FUNC(dgraphvars[v]); }	\
	else if (graph_type(v) == UTREE)	{ FUNC(utreevars[v]); }		\
	else if (graph_type(v) == DTREE)	{ FUNC(dtreevars[v]); }		\
	else if (graph_type(v) == URTREE)	{ FUNC(urtreevars[v]); }	\
	else if (graph_type(v) == DRTREE)	{ FUNC(drtreevars[v]); }

#define if_mfunction(v, FUNC)										\
	if (graph_type(v) == UGRAPH)		{ ugraphvars[v].FUNC; }		\
	else if (graph_type(v) == DGRAPH)	{ dgraphvars[v].FUNC; }		\
	else if (graph_type(v) == UTREE)	{ utreevars[v].FUNC; }		\
	else if (graph_type(v) == DTREE)	{ dtreevars[v].FUNC; }		\
	else if (graph_type(v) == URTREE)	{ urtreevars[v].FUNC; }		\
	else if (graph_type(v) == DRTREE)	{ drtreevars[v].FUNC; }

#define ffunction(v, FUNC)												\
	[&]() {																\
		if (graph_type(v) == UGRAPH)	{ return FUNC(ugraphvars[v]); }	\
		if (graph_type(v) == DGRAPH)	{ return FUNC(dgraphvars[v]); }	\
		if (graph_type(v) == UTREE)		{ return FUNC(utreevars[v]); }	\
		if (graph_type(v) == DTREE)		{ return FUNC(dtreevars[v]); }	\
		if (graph_type(v) == URTREE)	{ return FUNC(urtreevars[v]); }	\
		return FUNC(drtreevars[v]);										\
	}()

#define mfunction(v, FUNC)												\
	[&]() {																\
		if (graph_type(v) == UGRAPH)	{ return ugraphvars[v].FUNC; }	\
		if (graph_type(v) == DGRAPH)	{ return dgraphvars[v].FUNC; }	\
		if (graph_type(v) == UTREE)		{ return utreevars[v].FUNC; }	\
		if (graph_type(v) == DTREE)		{ return dtreevars[v].FUNC; }	\
		if (graph_type(v) == URTREE)	{ return urtreevars[v].FUNC; }	\
		return drtreevars[v].FUNC;										\
	}()



// macros for ALL trees
#define if_ffunction_trees(v, FUNC)									\
	if (graph_type(v) == UTREE)			{ FUNC(utreevars[v]); }		\
	else if (graph_type(v) == DTREE)	{ FUNC(dtreevars[v]); }		\
	else if (graph_type(v) == URTREE)	{ FUNC(urtreevars[v]); }	\
	else if (graph_type(v) == DRTREE)	{ FUNC(drtreevars[v]); }

#define if_mfunction_trees(v, FUNC)									\
	if (graph_type(v) == UTREE)			{ utreevars[v].FUNC; }		\
	else if (graph_type(v) == DTREE)	{ dtreevars[v].FUNC; }		\
	else if (graph_type(v) == URTREE)	{ urtreevars[v].FUNC; }		\
	else if (graph_type(v) == DRTREE)	{ drtreevars[v].FUNC; }

#define ffunction_trees(v, FUNC)										\
	[&]() {																\
		if (graph_type(v) == UTREE)		{ return FUNC(utreevars[v]); }	\
		if (graph_type(v) == DTREE)		{ return FUNC(dtreevars[v]); }	\
		if (graph_type(v) == URTREE)	{ return FUNC(urtreevars[v]); }	\
		return FUNC(drtreevars[v]);										\
	}()

#define mfunction_trees(v, FUNC)										\
	[&]() {																\
		if (graph_type(v) == UTREE)		{ return utreevars[v].FUNC; }	\
		if (graph_type(v) == DTREE)		{ return dtreevars[v].FUNC; }	\
		if (graph_type(v) == URTREE)	{ return urtreevars[v].FUNC; }	\
		return drtreevars[v].FUNC;										\
	}()



// macros for DIRECTED graphs
#define if_ffunction_dir_graphs(v, FUNC)							\
	if (graph_type(v) == DGRAPH)		{ FUNC(dgraphvars[v]); }	\
	else if (graph_type(v) == DTREE)	{ FUNC(dtreevars[v]); }		\
	else if (graph_type(v) == DRTREE)	{ FUNC(drtreevars[v]); }

#define if_mfunction_dir_graphs(v, FUNC)							\
	if (graph_type(v) == DGRAPH)		{ dgraphvars[v].FUNC; }		\
	else if (graph_type(v) == DTREE)	{ dtreevars[v].FUNC; }		\
	else if (graph_type(v) == DRTREE)	{ drtreevars[v].FUNC; }

#define ffunction_dir_graphs(v, FUNC)										\
	[&]() {																	\
		if (graph_type(v) == DGRAPH)		{ return FUNC(dgraphvars[v]); }	\
		else if (graph_type(v) == DTREE)	{ return FUNC(dtreevars[v]); }	\
		return FUNC(drtreevars[v]);											\
	}()

#define mfunction_dir_graphs(v, FUNC)										\
	[&]() {																	\
		if (graph_type(v) == DGRAPH)		{ return dgraphvars[v].FUNC; }	\
		else if (graph_type(v) == DTREE)	{ return dtreevars[v].FUNC; }	\
		return drtreevars[v].FUNC;											\
	}()


// -----------------------
// macros for ROOTED trees

#define if_ffunction_rtrees(v, FUNC)								\
	if (graph_type(v) == URTREE)		{ FUNC(urtreevars[v]); }	\
	else if (graph_type(v) == DRTREE)	{ FUNC(drtreevars[v]); }

#define if_mfunction_rtrees(v, FUNC)								\
	if (graph_type(v) == URTREE)		{ urtreevars[v].FUNC; }		\
	else if (graph_type(v) == DRTREE)	{ drtreevars[v].FUNC; }

#define ffunction_rtrees(v, FUNC)										\
	[&]() {																\
		if (graph_type(v) == URTREE)	{ return FUNC(urtreevars[v]); }	\
		return FUNC(drtreevars[v]);										\
	}()

#define mfunction_rtrees(v, FUNC)										\
	[&]() {																\
		if (graph_type(v) == URTREE)	{ return urtreevars[v].FUNC; }	\
		return drtreevars[v].FUNC;										\
	}()


#define output_graph(v)														\
	if (graph_type(v) == UGRAPH)		{ cerr << ugraphvars[v] << endl; }	\
	else if (graph_type(v) == DGRAPH)	{ cerr << dgraphvars[v] << endl; }	\
	else if (graph_type(v) == UTREE)	{ cerr << utreevars[v] << endl; }	\
	else if (graph_type(v) == DTREE)	{ cerr << dtreevars[v] << endl; }	\
	else if (graph_type(v) == URTREE)	{ cerr << urtreevars[v] << endl; }	\
	else if (graph_type(v) == DRTREE)	{ cerr << drtreevars[v] << endl; }

#define output_graph_stdout(v)												\
	if (graph_type(v) == UGRAPH)		{ cout << ugraphvars[v] << endl; }	\
	else if (graph_type(v) == DGRAPH)	{ cout << dgraphvars[v] << endl; }	\
	else if (graph_type(v) == UTREE)	{ cout << utreevars[v] << endl; }	\
	else if (graph_type(v) == DTREE)	{ cout << dtreevars[v] << endl; }	\
	else if (graph_type(v) == URTREE)	{ cout << urtreevars[v] << endl; }	\
	else if (graph_type(v) == DRTREE)	{ cout << drtreevars[v] << endl; }


#define WRONG_TYPE_EXT(my_assert, g, T, my_type)						\
	cerr << ERROR << endl;												\
	message_in_func(my_assert)											\
	cerr << "    Graph '" << g << "' is not " << T << "." << endl;		\
	cerr << "    Graph '" << g << "' is " << my_type << "." << endl;

#define WRONG_TYPE(my_assert, g, T) WRONG_TYPE_EXT(my_assert, g, T, graph_type(g))

#define assert_is_rtree(g1, FUNC)										\
	if (not mfunction_rtrees(g1, is_tree())) {							\
		cerr << "ERROR" << endl;										\
		message_in_func(FUNC)											\
		cerr << "    Graph '" << g1 << "' is not a tree." << endl;		\
		return err_type::test_exe_error;								\
	}																	\
	if (not mfunction_rtrees(g1, is_rooted())) {						\
		cerr << "ERROR" << endl;										\
		message_in_func(FUNC)											\
		cerr << "    Tree '" << g1 << "' is not rooted." << endl;		\
		return err_type::test_exe_error;								\
	}																	\

#define assert_is_drtree(g1, FUNC)										\
	assert_is_rtree(g1, FUNC)											\
	if (not mfunction_rtrees(g1, is_directed())) {						\
		cerr << "ERROR" << endl;										\
		message_in_func(FUNC)											\
		cerr << "    Tree '" << g1 << "' is not directed." << endl;		\
		return err_type::test_exe_error;								\
	}


#define drtree_type_to_string(t)							\
	[](drtree::drtree_type __t) -> std::string {			\
		switch (__t) {										\
			case drtree::drtree_type::arborescence:			\
				return "arborescence";						\
			case drtree::drtree_type::anti_arborescence:	\
				return "anti_arborescence";					\
			default:										\
				return "none";								\
		}													\
	}(t)

namespace exe_tests {

std::vector<lal::edge> enumerate_edges_brute_force(const lal::graphs::graph& g);
bool share_vertices(const lal::edge_pair& st_uv);
std::vector<lal::edge_pair> enumerate_Q_brute_force(const lal::graphs::graph& g);

err_type process_assert(
	const std::string& assert_what,
	std::map<std::string, lal::graphs::ugraph>& ugraphvars,
	std::map<std::string, lal::graphs::dgraph>& dgraphvars,
	std::map<std::string, lal::graphs::utree>& utreevars,
	std::map<std::string, lal::graphs::dtree>& dtreevars,
	std::map<std::string, lal::graphs::urtree>& urtreevars,
	std::map<std::string, lal::graphs::drtree>& drtreevars,
	std::map<std::string, std::string>& gtypes,
	std::ifstream& fin
);

} // -- namespace exe_tests
