/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
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
#include <lal/graphs/rooted_directed_tree.hpp>
#include <lal/iterators/edge_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/io/edge_list.hpp>
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

static const vector<string> dir_undir_types(
	{"directed", "undirected"}
);
static const vector<string> all_types(
	{"directed", "undirected", "rooted_directed_tree"}
);

/* ASSERTIONS */

#define assert_correct_file_type(ft)										\
	if (ft != "edge_list") {												\
		cerr << ERROR << endl;												\
		cerr << "    Invalid file type '" << ft << "'." << endl;			\
		return err_type::test_format_error;									\
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

#define assert_correct_normalise(str)												\
	if (norm != "true" and norm != "false") {										\
		cerr << ERROR << endl;														\
		cerr << "    Invalid value for boolean in add_edge command." << endl;		\
		cerr << "    Received '" << norm << "'. Valid values: true/false." << endl;	\
		return err_type::test_exe_error;											\
	}

/* USEFUL FUNCTIONS */

#define are_graphs_equal(var1, var2)																\
	[&]() -> bool {																					\
		if (graph_type(var1) == "directed") { return equal_graphs(dvars[var1], dvars[var2]); }		\
		if (graph_type(var1) == "undirected") { return equal_graphs(uvars[var1], uvars[var2]); }	\
		return equal_graphs(rdtvars[var1], rdtvars[var2]);											\
	}()

#define apply_if_ffunction(var, FUNC)									\
	if (graph_type(var) == "directed") { FUNC(dvars[var]); }			\
	else if (graph_type(var) == "undirected") { FUNC(uvars[var]); }		\
	else { FUNC(rdtvars[var]); }

#define apply_if_mfunction(var, FUNC)									\
	if (graph_type(var) == "directed") { dvars[var].FUNC; }				\
	else if (graph_type(var) == "undirected") { uvars[var].FUNC; }		\
	else { rdtvars[var].FUNC; }

#define apply_ffunction(var, FUNC)											\
	[&]() {																	\
		if (graph_type(var) == "directed") { return FUNC(dvars[var]); }		\
		if (graph_type(var) == "undirected") { return FUNC(uvars[var]); }	\
		return FUNC(rdtvars[var]);											\
	}()

#define apply_mfunction(var, FUNC)											\
	[&]() {																	\
		if (graph_type(var) == "directed") { return dvars[var].FUNC; }		\
		if (graph_type(var) == "undirected") { return uvars[var].FUNC; }	\
		return rdtvars[var].FUNC;											\
	}()

#define output_graph(var)														\
	if (graph_type(var) == "directed") { cerr << dvars[var] << endl; }			\
	else if (graph_type(var) == "undirected") { cerr << uvars[var] << endl; }	\
	else { cerr << rdtvars[var] << endl; }

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
	map<string, dgraph>& dvars,
	map<string, ugraph>& uvars,
	map<string, rooted_directed_tree>& rdtvars,
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
	else if (assert_what == "normalised") {
		fin >> g1;
		assert_exists_variable(g1)
		if (not apply_mfunction(g1, is_normalised())) {
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
		if (apply_mfunction(g1, is_normalised())) {
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
		if (not apply_mfunction(g1, has_edge(u, v))) {
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
		if (apply_mfunction(g1, has_edge(u, v))) {
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
		if (apply_mfunction(g1, n_nodes()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " nodes." << endl;
			cerr << "    Graph '" << g1 << "' has " << apply_mfunction(g1, n_nodes()) << " nodes." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_edges") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (apply_mfunction(g1, n_edges()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " edges." << endl;
			cerr << "    Graph '" << g1 << "' has " << apply_mfunction(g1, n_edges()) << " edges." << endl;
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

		vector<edge> gv = apply_ffunction(g1, enumerate_edges_brute_force);
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

		vector<edge_pair> gv = apply_ffunction(g1, enumerate_Q_brute_force);
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
	else if (assert_what == "directed") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] != "directed") {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is not directed." << endl;
			return err_type::test_exe_error;
		}
		if (not apply_mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not return directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_directed") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] == "directed") {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is directed." << endl;
			return err_type::test_exe_error;
		}
		if (apply_mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' returns directed." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "undirected") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] != "undirected") {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is not undirected." << endl;
			return err_type::test_exe_error;
		}
		if (not apply_mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not return undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_undirected") {
		fin >> g1;
		assert_exists_variable(g1)
		if (gtypes[g1] == "undirected") {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' is undirected." << endl;
			return err_type::test_exe_error;
		}
		if (apply_mfunction(g1, is_undirected())) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' returns undirected." << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "degree") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (apply_mfunction(g1, degree(u)) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have (out) degree " << v << endl;
			cerr << "    The vertex has (out) degree: " << apply_mfunction(g1, degree(u)) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "in_degree") {
		fin >> g1 >> u >> v;
		assert_exists_variable(g1)
		if (not apply_mfunction(g1, is_directed())) {
			cerr << ERROR << endl;
			cerr << "    Assertion 'in_degree' can only be applied to directed graphs." << endl;
			return err_type::test_exe_error;
		}
		if (graph_type(g1) == "directed" and dvars[g1].in_degree(u) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have in-degree " << v << endl;
			cerr << "    The vertex has in-degree: " << dvars[g1].in_degree(u) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
		if (graph_type(g1) == "rooted_directed_tree" and rdtvars[g1].in_degree(u) != v) {
			cerr << ERROR << endl;
			cerr << "    The vertex '" << u << "' of graph '"
				 << g1 << "' does not have in-degree " << v << endl;
			cerr << "    The vertex has in-degree: " << rdtvars[g1].in_degree(u) << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
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
	map<string, dgraph> dvars;
	map<string, ugraph> uvars;
	map<string, rooted_directed_tree> rdtvars;
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
			if (type == "directed") {
				dvars[g1] = dgraph(n_nodes);
			}
			else if (type == "undirected") {
				uvars[g1] = ugraph(n_nodes);
			}
			else {
				rdtvars[g1] = rooted_directed_tree(n_nodes);
			}
		}
		else if (option == "read_graph") {
			fin >> type >> g1 >> file >> file_type >> norm;
			assert_correct_graph_type(type, all_types)
			assert_correct_file_type(file_type)
			assert_correct_normalise(norm)
			gtypes[g1] = type;
			bool io_res = false;
			if (type == "directed") {
				dvars[g1] = dgraph();
				io_res = io::read_edge_list(file, dvars[g1], norm == "true");
			}
			else if (type == "undirected") {
				uvars[g1] = ugraph();
				io_res = io::read_edge_list(file, uvars[g1], norm == "true");
			}
			else if (type == "rooted_directed_trees") {
				cerr << ERROR << endl;
				cerr << "    I/O operation not implemented for rooted directed trees" << endl;
				return err_type::test_exe_error;
			}
			if (not io_res) {
				cerr << ERROR << endl;
				cerr << "    I/O operation failed when attempting to read file '" << file << "'." << endl;
				return err_type::test_exe_error;
			}
		}
		else if (option == "init_graph") {
			fin >> g1 >> n_nodes;
			assert_exists_variable(g1)
			apply_if_mfunction(g1, init(n_nodes))
		}
		else if (option == "assign") {
			fin >> g1 >> g2;
			assert_exists_variable(g2)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == "directed") { dvars[g1] = dvars[g2]; }
			else { uvars[g1] = uvars[g2]; }
		}
		else if (option == "add_edge") {
			fin >> g1 >> u >> v >> norm;
			assert_exists_variable(g1)
			assert_correct_normalise(norm)
			apply_if_mfunction(g1, add_edge(u, v, norm == "true"))
		}
		else if (option == "add_edges") {
			fin >> g1 >> n_nodes;
			vector<edge> v(n_nodes);
			for (edge& e : v) { fin >> e.first >> e.second; }
			fin >> norm;
			assert_exists_variable(g1)
			assert_correct_normalise(norm)
			apply_if_mfunction(g1, add_edges(v, norm == "true"))
		}
		else if (option == "assert") {
			err_type e = process_assert(dvars, uvars, rdtvars, gtypes, fin);
			if (e != err_type::no_error) { return e; }
		}
		else if (option == "normalise") {
			fin >> g1;
			assert_exists_variable(g1)
			apply_if_mfunction(g1, normalise())
		}
		else if (option == "disjoint_union") {
			fin >> g1 >> g2 >> g3;
			assert_exists_variable(g2)
			assert_exists_variable(g3)
			assert_equal_types(g2,g3)
			gtypes[g1] = graph_type(g2);
			if (graph_type(g2) == "directed") {
				make_disjoint_union<dgraph>(g1, g2, g3, dvars);
			}
			else if (graph_type(g2) == "undirected") {
				make_disjoint_union<ugraph>(g1, g2, g3, uvars);
			}
		}
		else if (option == "check_edge_iterator") {
			fin >> g1;
			assert_exists_variable(g1)
			vector<edge> iter_edges;
			if (graph_type(g1) == "directed") {
				edge_iterator it(dvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			else {
				edge_iterator it(uvars[g1]);
				while (it.has_next()) {
					it.next();
					iter_edges.push_back(it.get_edge());
				}
			}
			sort(iter_edges.begin(), iter_edges.end());
			vector<edge> graph_edges = apply_ffunction(g1, enumerate_edges_brute_force);
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
			if (graph_type(g1) == "directed") {
				const dgraph& dg1 = dvars[g1];
				Q_iterator it(dg1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			else {
				const ugraph& ug1 = uvars[g1];
				Q_iterator it(ug1);
				while (it.has_next()) {
					it.next();
					iter_pair_edges.push_back(it.get_pair());
					assert(not share_vertices(it.get_pair()));
				}
			}
			sort(iter_pair_edges.begin(), iter_pair_edges.end());
			vector<edge_pair> gpe = apply_ffunction(g1, enumerate_Q_brute_force);
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
			if (graph_type(g2) != "directed") {
				cerr << ERROR << endl;
				cerr << "    Graph '" << g2 << "' is not directed." << endl;
				cerr << "    Graph '" << g2 << "' is " << graph_type(g2) << "." << endl;
				return err_type::test_exe_error;
			}
			uvars[g1] = dvars[g2].to_undirected();
			gtypes[g1] = "undirected";
		}
		else if (option == "to_rooted_directed") {
			fin >> g1 >> g2 >> u;
			assert_exists_variable(g2)
			if (graph_type(g2) != "undirected") {
				cerr << ERROR << endl;
				cerr << "    Cannot apply 'to_root_directed' to a non-undirected graph." << endl;
				cerr << "    Graph '" << g2 << "' is not undirected." << endl;
				cerr << "    Graph '" << g2 << "' is " << graph_type(g2) << "." << endl;
				return err_type::test_exe_error;
			}
			rdtvars[g1] = rooted_directed_tree(uvars[g2], u);
			gtypes[g1] = "rooted_directed_tree";
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
