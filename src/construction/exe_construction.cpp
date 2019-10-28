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
#include <iostream>
#include <map>
#include <set>
using namespace std;

// lal includes
#include <lal/dgraph.hpp>
#include <lal/ugraph.hpp>
using namespace lal;

// custom includes
#include "exe_tests.hpp"

#define MAP_HAS(MAP, KEY) (MAP.find(KEY) != MAP.end())

#define assert_exists_variable(MAP, VAR)									\
	if (not MAP_HAS(MAP, VAR)) {											\
		cerr << ERROR << endl;					\
		cerr << "    Variable graph '" << VAR << "' not declared." << endl;	\
		return err_type::test_exe_error;									\
	}

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

template<class G>
err_type process_assert(map<string, G>& variables, ifstream& fin) {
	string g1, g2;
	uint32_t n;
	node u,v;
	string assert_what;

	fin >> assert_what;
	if (assert_what == "equal_graphs") {
		fin >> g1 >> g2;
		assert_exists_variable(variables, g1)
		assert_exists_variable(variables, g2)
		if (not equal_graphs(variables[g1], variables[g2])) {
			cerr << ERROR << endl;
			cerr << "    In 'equal_graphs' assertion:" << endl;
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are not equal." << endl;
			cerr << "    Contents of graphs:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			cerr << "    g2:" << endl;
			cerr << variables[g2] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_equal_graphs") {
		fin >> g1 >> g2;
		assert_exists_variable(variables, g1)
		assert_exists_variable(variables, g2)
		if (equal_graphs(variables[g1], variables[g2])) {
			cerr << ERROR << endl;
			cerr << "    In 'not_equal_graphs' assertion:" << endl;
			cerr << "    Graphs '" << g1 << "' and '" << g2 << "' are equal." << endl;
			cerr << "    Contents of graphs:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			cerr << "    g2:" << endl;
			cerr << variables[g2] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "normalised") {
		fin >> g1;
		assert_exists_variable(variables, g1)
		if (not variables[g1].is_normalised()) {
			cerr << ERROR << endl;
			cerr << "    In 'normalised' assertion:" << endl;
			cerr << "    Graph '" << g1 << "' is not normalised." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_normalised") {
		fin >> g1;
		assert_exists_variable(variables, g1)
		if (variables[g1].is_normalised()) {
			cerr << ERROR << endl;
			cerr << "    In 'normalised' assertion:" << endl;
			cerr << "    Graph '" << g1 << "' is normalised." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "exists_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(variables, g1)
		if (not variables[g1].has_edge(u, v)) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "not_exists_edge") {
		fin >> g1 >> u >> v;
		assert_exists_variable(variables, g1)
		if (variables[g1].has_edge(u, v)) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' has edge (" << u << ", " << v << ")." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_nodes") {
		fin >> g1 >> n;
		assert_exists_variable(variables, g1)
		if (variables[g1].n_nodes() != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " nodes." << endl;
			cerr << "    Graph '" << g1 << "' has " << variables[g1].n_nodes() << " nodes." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "num_edges") {
		fin >> g1 >> n;
		assert_exists_variable(variables, g1)
		if (variables[g1].n_edges() != n) {
			cerr << ERROR << endl;
			cerr << "    Graph '" << g1 << "' does not have " << n << " edges." << endl;
			cerr << "    Graph '" << g1 << "' has " << variables[g1].n_edges() << " edges." << endl;
			cerr << "    Contents of graph:" << endl;
			cerr << "    g1:" << endl;
			cerr << variables[g1] << endl;
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
err_type exe_construction_test(ifstream& fin) {
	map<string, G> variables;

	string option, assert_what;
	string n1, n2, n3;
	uint32_t n_nodes;
	uint32_t u, v;

	while (fin >> option) {
		if (option == "create_graph") {
			fin >> n1 >> n_nodes;
			variables[n1] = G(n_nodes);
		}
		else if (option == "assign") {
			fin >> n1 >> n2;
			assert_exists_variable(variables, n2)
			variables[n1] = variables[n2];
		}
		else if (option == "add_edge") {
			fin >> n1 >> u >> v >> n2;
			assert_exists_variable(variables, n1)
			if (n2 != "true" and n2 != "false") {
				cerr << ERROR << endl;
				cerr << "    Invalid value for boolean in add_edge command." << endl;
				cerr << "    Received '" << n2 << "'. Valid values: true/false." << endl;
				return err_type::test_exe_error;
			}
			variables[n1].add_edge(u, v, n2 == "true");
		}
		else if (option == "add_edges") {
			fin >> n1 >> n_nodes;
			vector<edge> v(n_nodes);
			for (edge& e : v) { fin >> e.first >> e.second; }
			fin >> n2;

			assert_exists_variable(variables, n1)
			if (n2 != "true" and n2 != "false") {
				cerr << ERROR << endl;
				cerr << "    Invalid value for boolean in add_edge command." << endl;
				cerr << "    Received '" << n2 << "'. Valid values: true/false." << endl;
				return err_type::test_exe_error;
			}
			variables[n1].add_edges(v, n2 == "true");
		}
		else if (option == "assert") {
			err_type e = process_assert(variables, fin);
			if (e != err_type::no_error) { return e; }
		}
		else if (option == "normalise") {
			fin >> n1;
			assert_exists_variable(variables, n1)
			variables[n1].normalise();
		}
		else if (option == "disjoint_union") {
			fin >> n1 >> n2 >> n3;
			assert_exists_variable(variables, n1)
			assert_exists_variable(variables, n2)
			assert_exists_variable(variables, n3)

			G& v1 = variables[n1];
			v1 = variables[n2];
			v1.disjoint_union(variables[n3]);
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid option '" << option << "'." << endl;
			return err_type::test_format_error;
		}
	}

	return err_type::no_error;
}

err_type consume_tokens(ifstream& fin) {
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
	return err_type::no_error;
}

err_type exe_construction_directed(ifstream& fin) {
	err_type e = consume_tokens(fin);
	if (e != err_type::no_error) { return e; }
	e = exe_construction_test<dgraph>(fin);
	if (e != err_type::no_error) {
		cout << "Test finished without apparent errors." << endl;
	}
	return e;
}

err_type exe_construction_undirected(ifstream& fin) {
	err_type e = consume_tokens(fin);
	if (e != err_type::no_error) { return e; }
	e = exe_construction_test<ugraph>(fin);
	if (e != err_type::no_error) {
		cout << "Test finished without apparent errors." << endl;
	}
	return e;
}

} // -- namespace exe_tests
