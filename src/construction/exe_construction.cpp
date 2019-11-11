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

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <memory>
#include <map>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/dgraph.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/utree.hpp>
#include <lal/graphs/dtree.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/graphs/drtree.hpp>
#include <lal/properties/Q.hpp>
#include <lal/iterators/edge_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/io/edge_list.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace iterators;

// custom includes
#include "exe_tests.hpp"

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

static const vector<string> dir_undir_types(
	{UGRAPH, DGRAPH}
);
static const vector<string> rooted_types(
	{URTREE, DRTREE}
);
static const vector<string> tree_types(
	{UTREE, DTREE, URTREE, DRTREE}
);
static const vector<string> all_types(
	{UGRAPH, DGRAPH, UTREE, DTREE, URTREE, DRTREE}
);

/* ASSERTIONS */

#define assert_correct_file_type(ft)										\
	if (ft != "edge_list") {												\
		cerr << ERROR << endl;												\
		cerr << "    Invalid file type '" << ft << "'." << endl;			\
		return err_type::test_format_error;									\
	}

#define assert_correct_drtree_type(t)										\
	if (t != "arborescence" and t != "anti_arborescence" and t != "none") {	\
		cerr << ERROR << endl;												\
		cerr << "    Invalid drtree type '" << t << "'." << endl;			\
		return err_type::test_format_error;									\
	}

#define assert_correct_normalise(str)												\
	if (norm != "true" and norm != "false") {										\
		cerr << ERROR << endl;														\
		cerr << "    Invalid value for boolean in add_edge command." << endl;		\
		cerr << "    Received '" << norm << "'. Valid values: true/false." << endl;	\
		return err_type::test_format_error;											\
	}

#define assert_exists_variable(var)											\
	if (not exists_variable(var)) {											\
		cerr << ERROR << endl;												\
		cerr << "    Variable graph '" << var << "' not declared." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_correct_graph_type(type, TYPES)								\
	if (not in_collection(type, TYPES)) {									\
		cerr << ERROR << endl;												\
		cerr << "    Graph type '" << type << "' is incorrect." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_equal_types(var1, var2)																	\
	if (graph_type(var1) != graph_type(var2)) {															\
		cerr << ERROR << endl;																			\
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


// macros for ROOTED trees
#define if_ffunction_rooted_trees(v, FUNC)							\
	if (graph_type(v) == URTREE)		{ FUNC(urtreevars[v]); }	\
	else if (graph_type(v) == DRTREE)	{ FUNC(drtreevars[v]); }

#define if_mfunction_rooted_trees(v, FUNC)							\
	if (graph_type(v) == URTREE)		{ urtreevars[v].FUNC; }		\
	else if (graph_type(v) == DRTREE)	{ drtreevars[v].FUNC; }

#define ffunction_rooted_trees(v, FUNC)									\
	[&]() {																\
		if (graph_type(v) == URTREE)	{ return FUNC(urtreevars[v]); }	\
		return FUNC(drtreevars[v]);										\
	}()

#define mfunction_rooted_trees(v, FUNC)									\
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


#define WRONG_TYPE_EXT(g, T, my_type)										\
	cerr << ERROR << endl;													\
	cerr << "    Graph '" << g << "' is not " << T << "." << endl;			\
	cerr << "    Graph '" << g << "' is " << my_type << "." << endl;

#define WRONG_TYPE(g, T) WRONG_TYPE_EXT(g, T, graph_type(g))


#define drtree_type drtree::rooted_directed_tree_type
#define drtree_type_to_string(t)					\
	[](drtree_type __t) -> std::string {			\
		switch (__t) {								\
			case drtree_type::arborescence:			\
				return "arborescence";				\
			case drtree_type::anti_arborescence:	\
				return "anti_arborescence";			\
			default:								\
				return "none";						\
		}											\
	}(t)

template<class G>
inline bool equal_graphs(const G& g1, const G& g2) {
	if (g1.n_nodes() != g2.n_nodes()) { return false; }
	uint32_t n = g1.n_nodes();
	for (uint32_t i = 0; i < n; ++i) {
		const neighbourhood& n1 = g1.get_neighbours(i);
		const neighbourhood& n2 = g2.get_neighbours(i);
		if (n1.size() != n2.size()) { return false; }
		set<node> sn1(n1.begin(), n1.end());
		set<node> sn2(n2.begin(), n2.end());
		if (n1.size() != sn1.size()) { return false; }
		if (n2.size() != sn2.size()) { return false; }
		if (sn1 != sn2) { return false; }
	}
	if (g1.is_normalised() != g2.is_normalised()) { return false; }
	return true;
}

inline vector<edge> enumerate_edges_brute_force(const graph& g) {
	const uint32_t n = g.n_nodes();
	set<edge> E;
	for (node s = 0; s < n; ++s) {
	for (auto t : g.get_neighbours(s)) {
		edge st(s,t);
		if (g.is_undirected()) {
			if (s > t) { std::swap(st.first, st.second); }
		}
		E.insert(st);
	}}
	return vector<edge>(E.begin(), E.end());
}

inline bool share_vertices(const edge_pair& st_uv) {
	const edge& st = st_uv.first;
	const edge& uv = st_uv.second;
	const node s = st.first;
	const node t = st.second;
	const node u = uv.first;
	const node v = uv.second;
	return s == u or s == v or t == u or t == v;
}

inline vector<edge_pair> enumerate_Q_brute_force(const graph& g) {
	const uint32_t n = g.n_nodes();
	set<edge_pair> Q;
	for (node s = 0; s < n; ++s) {
	for (node t : g.get_neighbours(s)) {

		for (node u = s + 1; u < n; ++u) {
		for (node v : g.get_neighbours(u)) {

			// s != u and t != u
			if (s == v or s == u) { continue; }
			if (t == v or t == u) { continue; }

			// an undirected edge should not be sorted
			edge st(s,t);
			edge uv(u,v);
			if (g.is_undirected()) {
				if (s > t) { std::swap(st.first, st.second); }
				if (u > v) { std::swap(uv.first, uv.second); }
			}
			// the pair should be sorted
			if (st > uv) { std::swap(st, uv); }

			// no common endpoints
			Q.insert( edge_pair(st, uv) );
		}}
	}}
	return vector<edge_pair>(Q.begin(), Q.end());
}

namespace exe_tests {

err_type process_assert(
	map<string, ugraph>& ugraphvars,
	map<string, dgraph>& dgraphvars,
	map<string, utree>& utreevars,
	map<string, dtree>& dtreevars,
	map<string, urtree>& urtreevars,
	map<string, drtree>& drtreevars,
	map<string, string>& gtypes,
	ifstream& fin
)
{
	string g1, g2;
	uint32_t n;
	node u,v;
	string assert_what;

	fin >> assert_what;
	if (assert_what == "equal_graphs") {
		fin >> g1 >> g2;
		assert_exists_variable(g1)
		assert_exists_variable(g2)
		assert_equal_types(g1, g2)
		if (not are_graphs_equal(g1, g2)) {
			cerr << ERROR << endl;
			cerr << "    In 'equal_graphs' assertion:" << endl;
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are not equal." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of " << g2 << ":" << endl;
			output_graph(g2)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_equal_graphs") {
		fin >> g1 >> g2;
		assert_exists_variable(g1)
		assert_exists_variable(g2)
		assert_equal_types(g1, g2)
		if (are_graphs_equal(g1, g2)) {
			cerr << ERROR << endl;
			cerr << "    In 'not_equal_graphs' assertion:" << endl;
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are equal." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			cerr << "    Contents of " << g2 << ":" << endl;
			output_graph(g2)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_normalised") {
		fin >> g1;
		assert_exists_variable(g1)
		if (not mfunction(g1, is_normalised())) {
			cerr << ERROR << endl;
			cerr << "    In 'normalised' assertion:" << endl;
			cerr << "    Graph '" << g1 << "' is not normalised." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_normalised") {
		fin >> g1;
		assert_exists_variable(g1)
		if (mfunction(g1, is_normalised())) {
			cerr << ERROR << endl;
			cerr << "    In 'normalised' assertion:" << endl;
			cerr << "    Graph '" << g1 << "' is normalised." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "exists_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not mfunction(g1, has_edge(u, v))) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_exists_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (mfunction(g1, has_edge(u, v))) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' has edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_nodes") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (mfunction(g1, n_nodes()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " nodes." << endl;
			cerr << "    Graph '" << g1 << "' has " << mfunction(g1, n_nodes()) << " nodes." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_edges") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (mfunction(g1, n_edges()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " edges." << endl;
			cerr << "    Graph '" << g1 << "' has " << mfunction(g1, n_edges()) << " edges." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "edges_are") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		vector<edge> v(n);
		for (edge& e : v) { fin >> e.first >> e.second; }
		sort(v.begin(), v.end());

		vector<edge> gv = ffunction(g1, enumerate_edges_brute_force);
		sort(gv.begin(), gv.end());
		if (v != gv) {
			cerr << ERROR << endl;
			cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list." << endl;
			cerr << "    List (" << v.size() << "):" << endl;
			for (auto e : v) {
				cerr << "    " << edge_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					cerr << " <- not in the graph!";
				}
				cerr << endl;
			}
			cerr << "    Graph (" << gv.size() << "):" << endl;
			for (auto e : gv) {
				cerr << "    " << edge_out(e) << endl;
			}
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "elements_Q_are") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		vector<edge_pair> v(n);
		for (edge_pair& e : v) {
			fin >> e.first.first >> e.first.second
				>> e.second.first >> e.second.second;
			assert(not share_vertices(e));
		}
		sort(v.begin(), v.end());

		vector<edge_pair> gv = ffunction(g1, enumerate_Q_brute_force);
		sort(gv.begin(), gv.end());
		if (v != gv) {
			cerr << ERROR << endl;
			cerr << "    The edges in graph '" << g1
				 << "' do not coincide with those in the list." << endl;
			cerr << "    List (" << v.size() << "):" << endl;
			for (auto e : v) {
				cerr << "    " << edge_pair_out(e);
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					cerr << " <- not in the graph!";
				}
				cerr << endl;
			}
			cerr << "    Graph (" << gv.size() << "):" << endl;
			for (auto e : gv) {
				cerr << "    " << edge_pair_out(e) << endl;
			}
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_undirected") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] != UGRAPH) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is not undirected." << endl;
			return err_type::test_exe_error;
		}
		if (not mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not return undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_undirected") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] == UGRAPH) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is undirected." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' returns undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_directed") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] != DGRAPH) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is not directed." << endl;
			return err_type::test_exe_error;
		}
		if (not mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not return directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_directed") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] == DGRAPH) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is directed." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' returns directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "degree") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (mfunction(g1, degree(u)) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have degree " << v << endl;
			cerr << "    The vertex has degree: " << mfunction(g1, degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}

	// DIRECTED GRAPHS
	else if (assert_what == "in_degree") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Assertion 'in_degree' can only be applied to directed graphs." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction_dir_graphs(g1, in_degree(u)) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have in-degree " << v << endl;
			cerr << "    The vertex has in-degree: " << mfunction_dir_graphs(g1, in_degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "out_degree") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Assertion 'out_degree' can only be applied to directed graphs." << endl;
			return err_type::test_exe_error;
		}
		if (mfunction_dir_graphs(g1, out_degree(u)) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have out-degree " << v << endl;
			cerr << "    The vertex has out-degree: " << mfunction_dir_graphs(g1, out_degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}

	// TREES
	else if (assert_what == "can_add_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not in_collection(graph_type(g1), tree_types)) {
			WRONG_TYPE(g1, "a tree (of any type)")
			return err_type::test_exe_error;
		}
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (not can) {
			cerr << ERROR << endl;
			cerr << "    Cannot add edge with vertices " << u << ", " << v << " "
				 << "to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "cant_add_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not in_collection(graph_type(g1), tree_types)) {
			WRONG_TYPE(g1, "a tree (of any type)")
			return err_type::test_exe_error;
		}
		bool can = mfunction_trees(g1, can_add_edge(u,v));
		if (can) {
			cerr << ERROR << endl;
			cerr << "    Edge with vertices " << u << ", " << v << " "
				 << "can be added to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "can_add_edges") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (not in_collection(graph_type(g1), tree_types)) {
			WRONG_TYPE(g1, "a tree (of any type)")
			return err_type::test_exe_error;
		}

		vector<edge> edge_list(n);
		for (edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (not can) {
			cerr << ERROR << endl;
			cerr << "    Cannot add edges" << endl;
			for (auto e : edge_list) {
			cerr << "        " << e.first << ", " << e.second << endl;
			}
			cerr << "    to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "cant_add_edges") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (not in_collection(graph_type(g1), tree_types)) {
			WRONG_TYPE(g1, "a tree (of any type)")
			return err_type::test_exe_error;
		}

		vector<edge> edge_list(n);
		for (edge& e : edge_list) { fin >> e.first >> e.second; }

		bool can = mfunction_trees(g1, can_add_edges(edge_list));
		if (can) {
			cerr << ERROR << endl;
			cerr << "    Cannot add edges" << endl;
			for (auto e : edge_list) {
			cerr << "        " << e.first << ", " << e.second << endl;
			}
			cerr << "    to graph '" << g1 << "'" << endl;
			cout << "    Contents of '" << g1 << "':" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_rooted") {
		fin >> g1;
		assert_exists_variable(g1)
		assert_correct_graph_type(graph_type(g1), tree_types)
		if (not mfunction_trees(g1, is_rooted())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is not rooted." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_not_rooted") {
		fin >> g1;
		assert_exists_variable(g1)
		assert_correct_graph_type(graph_type(g1), tree_types)
		if (mfunction_trees(g1, is_rooted())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is rooted." << endl;
			return err_type::test_exe_error;
		}
	}

	// ROOTED DIRECTED TREES
	else if (assert_what == "is_drtree_type") {
		fin >> g1 >> g2;
		assert_exists_variable(g1)
		assert_correct_drtree_type(g2)
		if (gtypes[g1] != DRTREE) {
			WRONG_TYPE(g1, DRTREE)
			return err_type::test_exe_error;
		}
		drtreevars[g1].find_drtree_type();
		auto type = drtreevars[g1].get_drtree_type();
		if (g2 == "arborescence") {
			if (type != drtree::arborescence) {
				WRONG_TYPE_EXT(g1, "not an arborescence (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "anti_arborescence") {
			if (type != drtree::anti_arborescence) {
				WRONG_TYPE_EXT(g1, "not an anti_arborescence (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "none") {
			if (type != drtree::none) {
				WRONG_TYPE_EXT(g1, "not an none (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
	}
	else if (assert_what == "is_not_drtree_type") {
		fin >> g1 >> g2;
		assert_exists_variable(g1)
		assert_correct_drtree_type(g2)
		if (gtypes[g1] != DRTREE) {
			WRONG_TYPE(g1, DRTREE)
			return err_type::test_exe_error;
		}
		drtreevars[g1].find_drtree_type();
		auto type = drtreevars[g1].get_drtree_type();
		if (g2 == "arborescence") {
			if (type == drtree::arborescence) {
				WRONG_TYPE_EXT(g1, "arborescence (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "anti_arborescence") {
			if (type == drtree::anti_arborescence) {
				WRONG_TYPE_EXT(g1, "anti_arborescence (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
		else if (g2 == "none") {
			if (type == drtree::none) {
				WRONG_TYPE_EXT(g1, "none (drtree_type)", drtree_type_to_string(type))
				return err_type::test_exe_error;
			}
		}
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Invalid assertion '" << assert_what << "'." << endl;
		return err_type::test_format_error;
	}
	return err_type::no_error;
}

template<class G>
void make_disjoint_union(
	const string& v1, const string& v2, const string& v3,
	map<string, G>& vars
)
{
	G g1 = vars[v2];
	g1.disjoint_union(vars[v3]);
	vars[v1] = g1;
}

err_type exe_construction_test(ifstream& fin) {
	map<string, ugraph> ugraphvars;
	map<string, dgraph> dgraphvars;
	map<string, utree> utreevars;
	map<string, dtree> dtreevars;
	map<string, urtree> urtreevars;
	map<string, drtree> drtreevars;

	map<string, string> gtypes;

	string option, assert_what;
	string type, g1, g2, g3, file, file_type, norm;
	uint32_t n_nodes;
	uint32_t u, v;

	while (fin >> option) {
		if (option == "create_graph") {
			fin >> type >> g1 >> n_nodes;
			assert_correct_graph_type(type, all_types)
			gtypes[g1] = type;
			if (type == UGRAPH) {
				ugraphvars[g1] = ugraph(n_nodes);
			}
			else if (type == DGRAPH) {
				dgraphvars[g1] = dgraph(n_nodes);
			}
			else if (type == UTREE) {
				utreevars[g1] = utree(n_nodes);
			}
			else if (type == DTREE) {
				dtreevars[g1] = dtree(n_nodes);
			}
			else if (type == URTREE) {
				urtreevars[g1] = urtree(n_nodes);
			}
			else if (type == DRTREE) {
				drtreevars[g1] = drtree(n_nodes);
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Unhandled type '" << type << "'." << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == "read_graph") {
			fin >> type >> g1 >> file >> file_type >> norm;
			assert_correct_graph_type(type, all_types)
			assert_correct_file_type(file_type)
			assert_correct_normalise(norm)
			gtypes[g1] = type;
			bool io_res = false;
			if (type == DGRAPH) {
				dgraphvars[g1] = dgraph();
				io_res = io::read_edge_list(file, dgraphvars[g1], norm == "true");
			}
			else if (type == UGRAPH) {
				ugraphvars[g1] = ugraph();
				io_res = io::read_edge_list(file, ugraphvars[g1], norm == "true");
			}
			else {
				cerr << ERROR << endl;
				cerr << "    I/O operation not implemented for " << gtypes[g1] << endl;
				return err_type::test_exe_error;
			}
			if (not io_res) {
				cerr << ERROR << endl;
				cerr << "    I/O operation failed when attempting to read file '" << file << "'." << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == "output_graph") {
			fin >> g1;
			assert_exists_variable(g1)
			cout << "-----------------------------" << endl;
			cout << "Contents of graph '" << g1 << "'" << endl;
			output_graph_stdout(g1)
			cout << "-----------------------------" << endl;
		}
		else if (option == "init_graph") {
			fin >> g1 >> n_nodes;
			assert_exists_variable(g1)
			if_mfunction(g1, init(n_nodes))
		}
		else if (option == "assign") {
			fin >> g1 >> g2;
			assert_exists_variable(g2)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == DGRAPH) { dgraphvars[g1] = dgraphvars[g2]; }
			else { ugraphvars[g1] = ugraphvars[g2]; }
		}
		else if (option == "add_edge") {
			fin >> g1 >> u >> v >> norm;
			assert_exists_variable(g1)
			assert_correct_normalise(norm)
			if_mfunction(g1, add_edge(u, v, norm == "true"))
		}
		else if (option == "add_edges") {
			fin >> g1 >> n_nodes;
			vector<edge> v(n_nodes);
			for (edge& e : v) { fin >> e.first >> e.second; }
			fin >> norm;
			assert_exists_variable(g1)
			assert_correct_normalise(norm)
			if_mfunction(g1, add_edges(v, norm == "true"))
		}
		else if (option == "assert") {
			err_type e = process_assert(
				ugraphvars, dgraphvars,
				utreevars, dtreevars,
				urtreevars, drtreevars,
				gtypes, fin
			);
			if (e != err_type::no_error) { return e; }
		}
		else if (option == "normalise") {
			fin >> g1;
			assert_exists_variable(g1)
			if_mfunction(g1, normalise())
		}
		else if (option == "disjoint_union") {
			fin >> g1 >> g2 >> g3;
			assert_exists_variable(g2)
			assert_exists_variable(g3)
			assert_equal_types(g2,g3)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == DGRAPH) {
				make_disjoint_union<dgraph>(g1, g2, g3, dgraphvars);
			}
			else if (graph_type(g2) == UGRAPH) {
				make_disjoint_union<ugraph>(g1, g2, g3, ugraphvars);
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Type of graphs '" << g1 << "' and '" << g2 << "' are not" << endl;
				cerr << "    " << UGRAPH << " or " << DGRAPH << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == "check_edge_iterator") {
			fin >> g1;
			assert_exists_variable(g1)
			vector<edge> iter_edges;
			if (graph_type(g1) == DGRAPH) {
				edge_iterator it(dgraphvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			else {
				edge_iterator it(ugraphvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			// check size
			if (iter_edges.size() != mfunction(g1, n_edges())) {
				cerr << ERROR << endl;
				cerr << "    The amount of edges obtained differs from the amount of edges in the graph." << endl;
				cerr << "    Number of edges obtained: " << iter_edges.size() << endl;
				cerr << "    Number of edges in the graph: " << mfunction(g1, n_edges()) << endl;
				return err_type::test_exe_error;
			}
			sort(iter_edges.begin(), iter_edges.end());
			vector<edge> graph_edges = ffunction(g1, enumerate_edges_brute_force);
			sort(graph_edges.begin(), graph_edges.end());
			if (iter_edges != graph_edges) {
				cerr << ERROR << endl;
				cerr << "    The edges in graph '" << g1
					 << "' do not coincide with those in the list." << endl;
				cerr << "    List (" << iter_edges.size() << "):" << endl;
				for (auto e : iter_edges) {
					cerr << "    " << edge_out(e);
					auto it = find(graph_edges.begin(), graph_edges.end(), e);
					if (it == graph_edges.end()) {
						cerr << " <- not in the graph!";
					}
					cerr << endl;
				}
				cerr << "    Graph (" << graph_edges.size() << "):" << endl;
				for (auto e : graph_edges) {
					cerr << "    " << edge_out(e) << endl;
				}
				cerr << "    Contents:" << endl;
				output_graph(g1)
				return err_type::test_exe_error;
			}
		}
		else if (option == "check_Q_iterator") {
			fin >> g1;
			assert_exists_variable(g1)
			vector<edge_pair> iter_pair_edges;
			if (graph_type(g1) == DGRAPH) {
				const dgraph& dg1 = dgraphvars[g1];
				Q_iterator it(dg1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			else {
				const ugraph& ug1 = ugraphvars[g1];
				Q_iterator it(ug1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			// check size
			if (iter_pair_edges.size() != ffunction(g1, properties::size_Q)) {
				cerr << ERROR << endl;
				cerr << "    The amount of pairs obtained differs from the size of the set Q." << endl;
				cerr << "    Number of pairs obtained: " << iter_pair_edges.size() << endl;
				cerr << "    Size of the set Q: " << ffunction(g1, properties::size_Q) << endl;
				return err_type::test_exe_error;
			}
			sort(iter_pair_edges.begin(), iter_pair_edges.end());
			vector<edge_pair> gpe = ffunction(g1, enumerate_Q_brute_force);
			sort(gpe.begin(), gpe.end());
			if (iter_pair_edges != gpe) {
				cerr << ERROR << endl;
				cerr << "    The pairs in graph '" << g1
					 << "' do not coincide with those in the list." << endl;
				cerr << "    List (" << iter_pair_edges.size() << "):" << endl;
				for (auto e : iter_pair_edges) {
					cerr << "    " << edge_pair_out(e);
					auto it = find(gpe.begin(), gpe.end(), e);
					if (it == gpe.end()) {
						cerr << " <- not in the graph!";
					}
					cerr << endl;
				}
				cerr << "    Graph (" << gpe.size() << "):" << endl;
				for (auto e : gpe) {
					cerr << "    " << edge_pair_out(e) << endl;
				}
				cerr << "    Contents:" << endl;
				output_graph(g1)
				return err_type::test_exe_error;
			}
		}
		else if (option == "to_undirected") {
			fin >> g1 >> g2;
			assert_exists_variable(g2)
			if (graph_type(g2) != DGRAPH) {
				WRONG_TYPE(g2, DGRAPH)
				return err_type::test_exe_error;
			}
			ugraphvars[g1] = dgraphvars[g2].to_undirected();
			gtypes[g1] = UGRAPH;
		}
		else if (option == "to_drtree") {
			fin >> g1 >> g2 >> u;
			assert_exists_variable(g2)
			if (graph_type(g2) != UGRAPH) {
				WRONG_TYPE(g2, UGRAPH)
				cerr << "    Cannot apply 'to_drtree' to a non-undirected graph." << endl;
				return err_type::test_exe_error;
			}
			drtreevars[g1] = drtree(ugraphvars[g2], u);
			gtypes[g1] = DRTREE;
		}
		else if (option == "set_root") {
			fin >> g1 >> u;
			assert_exists_variable(g1)
			assert_correct_graph_type(graph_type(g1), rooted_types)
			if (not mfunction_rooted_trees(g1, is_rooted())) {
				cerr << "ERROR" << endl;
				cerr << "    Graph '" << g1 << "' is not rooted." << endl;
				return err_type::test_exe_error;
			}
			if_mfunction_rooted_trees(g1, set_root(u))
			if (graph_type(g1) == DRTREE) {
				drtreevars[g1].find_drtree_type();
			}
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid option '" << option << "'." << endl;
			return err_type::test_format_error;
		}
	}

	return err_type::no_error;
}

err_type exe_construction(ifstream& fin) {
	string field;
	size_t n;

	fin >> field;
	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}
	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	err_type e = exe_construction_test(fin);
	if (e == err_type::no_error) {
		cout << "Test finished without apparent errors." << endl;
	}
	return e;
}

} // -- namespace exe_tests
