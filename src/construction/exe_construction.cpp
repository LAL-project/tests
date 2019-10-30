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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// C++ includes
#include <algorithm>
#include <iostream>
#include <memory>
#include <map>
#include <set>
using namespace std;

// lal includes
#include <lal/dgraph.hpp>
#include <lal/ugraph.hpp>
#include <lal/edge_iterator.hpp>
using namespace lal;

// custom includes
#include "exe_tests.hpp"

#define map_has(map, key) (map.find(key) != map.end())

#define exists_variable(var)												\
	map_has(gtypes, var)

#define graph_type(var)														\
	(gtypes[var])

/* ASSERTIONS */

#define assert_exists_variable(var)											\
	if (not exists_variable(var)) {											\
		cerr << ERROR << endl;												\
		cerr << "    Variable graph '" << var << "' not declared." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_correct_type(type)											\
	if (type != "directed" and type != "undirected") {						\
		cerr << ERROR << endl;												\
		cerr << "    Graph type '" << type << "' is incorrect." << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_equal_types(var1, var2)										\
	if (graph_type(var1) != graph_type(var2)) {								\
		cerr << ERROR << endl;												\
		cerr << "    Graphs '" << var1 << "' and '" << var2 << "' are not of the same type." << endl;	\
		cerr << "    Type of graph " << var1 << ": " << graph_type(var1) << endl;	\
		cerr << "    Type of graph " << var2 << ": " << graph_type(var2) << endl;	\
		return err_type::test_exe_error;									\
	}

#define assert_correct_normalise(str)										\
	if (norm != "true" and norm != "false") {								\
		cerr << ERROR << endl;												\
		cerr << "    Invalid value for boolean in add_edge command." << endl;		\
		cerr << "    Received '" << norm << "'. Valid values: true/false." << endl;	\
		return err_type::test_exe_error;									\
	}

/* USEFUL FUNCTIONS */

#define are_graphs_equal(var1, var2)										\
	(graph_type(var1) == "directed" ?										\
		equal_graphs(dvars[var1], dvars[var2]) :							\
		equal_graphs(uvars[var1], uvars[var2])								\
	)

#define apply_function(var, FUNC)											\
	(graph_type(var) == "directed" ? dvars[var].FUNC : uvars[var].FUNC)

#define apply_if_function(var, FUNC)										\
	if (graph_type(var) == "directed") { dvars[var].FUNC; }					\
	else { uvars[var].FUNC;	}

#define output_graph(var)													\
	if (graph_type(var) == "directed") { cerr << dvars[var] << endl; }		\
	else { cerr << uvars[var] << endl; }

namespace exe_tests {

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

err_type process_assert(
	map<string, dgraph>& dvars,
	map<string, ugraph>& uvars,
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
		if (not apply_function(g1, is_normalised())) {
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
		if (apply_function(g1, is_normalised())) {
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
		if (not apply_function(g1, has_edge(u, v))) {
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
		if (apply_function(g1, has_edge(u, v))) {
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
		if (apply_function(g1, n_nodes()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " nodes." << endl;
			cerr << "    Graph '" << g1 << "' has " << apply_function(g1, n_nodes()) << " nodes." << endl;
			cerr << "    Contents of " << g1 << ":" << endl;
			output_graph(g1)
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_edges") {
		fin >> g1 >> n;
		assert_exists_variable(g1)
		if (apply_function(g1, n_edges()) != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " edges." << endl;
			cerr << "    Graph '" << g1 << "' has " << apply_function(g1, n_edges()) << " edges." << endl;
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

		vector<edge> gv = apply_function(g1, edges());
		sort(gv.begin(), gv.end());
		if (v != gv) {
			cerr << ERROR << endl;
			cerr << "    The edges in graph '" << g1 << "' do not coincide with those in the list." << endl;
			cerr << "    List (" << v.size() << "):" << endl;
			for (const edge& e : v) {
				cerr << "    " << e.first << ", " << e.second;
				if (find(gv.begin(), gv.end(), e) == gv.end()) {
					cerr << " <- not in the graph!";
				}
				cerr << endl;
			}
			cerr << "    Graph (" << gv.size() << "):" << endl;
			for (const edge& e : gv) {
				cerr << "    " << e.first << ", " << e.second << endl;
			}
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
	map<string, string> gtypes;

	string option, assert_what;
	string type, g1, g2, g3, norm;
	uint32_t n_nodes;
	uint32_t u, v;

	while (fin >> option) {
		if (option == "create_graph") {
			fin >> type >> g1 >> n_nodes;
			assert_correct_type(type)
			gtypes[g1] = type;
			if (type == "directed") {
				dvars[g1] = dgraph(n_nodes);
			}
			else {
				uvars[g1] = ugraph(n_nodes);
			}
		}
		else if (option == "init_graph") {
			fin >> g1 >> n_nodes;
			assert_exists_variable(g1)
			apply_if_function(g1, init(n_nodes))
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
			apply_if_function(g1, add_edge(u, v, norm == "true"))
		}
		else if (option == "add_edges") {
			fin >> g1 >> n_nodes;
			vector<edge> v(n_nodes);
			for (edge& e : v) { fin >> e.first >> e.second; }
			fin >> norm;
			assert_exists_variable(g1)
			assert_correct_normalise(norm)
			apply_if_function(g1, add_edges(v, norm == "true"))
		}
		else if (option == "assert") {
			err_type e = process_assert(dvars, uvars, gtypes, fin);
			if (e != err_type::no_error) { return e; }
		}
		else if (option == "normalise") {
			fin >> g1;
			assert_exists_variable(g1)
			apply_if_function(g1, normalise())
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
			else {
				make_disjoint_union<ugraph>(g1, g2, g3, uvars);
			}
		}
		else if (option == "check_edge_iterator") {
			fin >> g1;
			assert_exists_variable(g1)
			vector<edge> iter_edges;
			if (graph_type(g1) == "directed") {
				edge_iterator e_it(dvars[g1]);
				while (e_it.has_next()) {
					iter_edges.push_back(e_it.next());
				}
			}
			else {
				edge_iterator e_it(uvars[g1]);
				while (e_it.has_next()) {
					iter_edges.push_back(e_it.next());
				}
			}
			sort(iter_edges.begin(), iter_edges.end());
			vector<edge> graph_edges = apply_function(g1, edges());
			sort(graph_edges.begin(), graph_edges.end());
			if (iter_edges != graph_edges) {
				cerr << ERROR << endl;
				cerr << "    The edges in graph '" << g1 << "' do not coincide with those in the list." << endl;
				cerr << "    List (" << iter_edges.size() << "):" << endl;
				for (const edge& e : iter_edges) {
					cerr << "    " << e.first << ", " << e.second;
					if (find(graph_edges.begin(), graph_edges.end(), e) == graph_edges.end()) {
						cerr << " <- not in the graph!";
					}
					cerr << endl;
				}
				cerr << "    Graph (" << graph_edges.size() << "):" << endl;
				for (const edge& e : graph_edges) {
					cerr << "    " << e.first << ", " << e.second << endl;
				}
				return err_type::test_exe_error;
			}
		}
		else if (option == "to_undirected") {
			fin >> g1 >> g2;
			assert_exists_variable(g2)
			if (graph_type(g2) != "directed") {
				cerr << ERROR << endl;
				cerr << "    Graph '" << g2 << "' is not directed." << endl;
				return err_type::test_exe_error;
			}
			uvars[g1] = dvars[g2].to_undirected();
			gtypes[g1] = "undirected";
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
