/********************************************************************
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
#include <iostream>
#include <fstream>
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/utils/graphs/cycles.hpp>
#include <lal/utils/graphs/reachability.hpp>
using namespace lal;
using namespace utils;
using namespace graphs;

// custom includes
#include "io_wrapper.hpp"
#include "definitions.hpp"

namespace exe_tests {

template<class G>
err_type process_common_assertions(const G& g, const string& assert_what, ifstream& fin)
{
	node s, t;
	if (assert_what == "is_reachable") {
		fin >> s >> t;
		const bool is = utils::is_node_reachable_from(g, s, t);
		if (not is) {
			cerr << ERROR << endl;
			cerr << "    Vertex " << t << " is not reachable from " << s << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "is_not_reachable") {
		fin >> s >> t;
		const bool is = utils::is_node_reachable_from(g, s, t);
		if (is) {
			cerr << ERROR << endl;
			cerr << "    Vertex " << t << " is reachable from " << s << endl;
			return err_type::test_exe_error;
		}
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Unknow assertion '" << assert_what << "'." << endl;
		return err_type::test_format_error;
	}
	return err_type::no_error;
}

err_type process_assert(const graphs::dgraph& g, ifstream& fin) {
	string assert_what;
	fin >> assert_what;
	if (assert_what == "has_dir_cycle") {
		const bool has = utils::has_directed_cycles(g);
		if (not has) {
			cerr << ERROR << endl;
			cerr << "    Graph does not have directed cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "hasnt_dir_cycle") {
		const bool has = utils::has_directed_cycles(g);
		if (has) {
			cerr << ERROR << endl;
			cerr << "    Graph does have directed cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "has_undir_cycle") {
		const bool has = utils::has_undirected_cycles(g);
		if (not has) {
			cerr << ERROR << endl;
			cerr << "    Graph does not have undirected cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "hasnt_undir_cycle") {
		const bool has = utils::has_undirected_cycles(g);
		if (has) {
			cerr << ERROR << endl;
			cerr << "    Graph does have undirected cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else {
		return process_common_assertions(g, assert_what, fin);
	}

	return err_type::no_error;
}

err_type process_assert(const graphs::ugraph& g, ifstream& fin) {
	string assert_what;
	fin >> assert_what;
	if (assert_what == "has_undir_cycle") {
		const bool has = utils::has_undirected_cycles(g);
		if (not has) {
			cerr << ERROR << endl;
			cerr << "    Graph does not have undirected cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else if (assert_what == "hasnt_undir_cycle") {
		const bool has = utils::has_undirected_cycles(g);
		if (has) {
			cerr << ERROR << endl;
			cerr << "    Graph does have undirected cycles" << endl;
			return err_type::test_exe_error;
		}
	}
	else {
		return process_common_assertions(g, assert_what, fin);
	}

	return err_type::no_error;
}

template<class G>
err_type process_instruction(const G& g, const string& command, ifstream& fin) {
	if (command == "output_graph") {
		cout << g << endl;
	}
	else if (command == "is_reachable") {

	}
	else if (command == "assert") {
		auto e = process_assert(g, fin);
		if (e != err_type::no_error) { return e; }
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Unrecognised command '" << command << "'" << endl;
		return err_type::test_format_error;
	}

	return err_type::no_error;
}

template<class G>
err_type test_without_graph(ifstream& fin) {
	G g;
	string command;
	while (fin >> command) {
		if (command == "add_edge") {
			node u,v;
			fin >> u >> v;
			g.add_edge(u,v);
		}
		else if (command == "init") {
			uint32_t n;
			fin >> n;
			g.init(n);
		}
		else {
			err_type r = process_instruction(g, command, fin);
			if (r != err_type::no_error) {
				return r;
			}
		}
	}
	return err_type::no_error;
}

template<class G>
err_type execute_utils_bfs_test(
	const string& graph_name,
	const string& graph_format,
	ifstream& fin
)
{
	G g;
	err_type r1 = io_wrapper::read_graph(graph_name, graph_format, g);
	if (r1 != err_type::no_error) { return r1; }

	string command;
	while (fin >> command) {
		r1 = process_instruction(g, command, fin);
		if (r1 != err_type::no_error) { return r1; }
	}
	return err_type::no_error;
}

err_type exe_utils_bfs(ifstream& fin) {
	const set<string> allowed_options({
		"sort_insertion", "sort_boolean"
	});

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n;
	fin >> n;
	if (n > 1) {
		cerr << ERROR << endl;
		cerr << "    At most one file is allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}

	string file_name, file_format;
	if (n == 1) {
		fin >> file_name >> file_format;
	}

	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string graph_type;
	fin >> graph_type;
	if (graph_type != "ugraph" and graph_type != "dgraph") {
		cerr << ERROR << endl;
		cerr << "    Incorrect type of graph specified." << endl;
		cerr << "    Recevied: '" << graph_type << "'." << endl;
		return err_type::test_format_error;
	}

	err_type r = err_type::no_error;
	cout << "graph_type: " << graph_type << endl;
	if (n == 1) {
		r = (graph_type == "ugraph" ?
			execute_utils_bfs_test<ugraph>(file_name, file_format, fin) :
			execute_utils_bfs_test<dgraph>(file_name, file_format, fin)
		);
	}
	else {
		r = (graph_type == "ugraph" ?
			test_without_graph<ugraph>(fin) :
			test_without_graph<dgraph>(fin)
		);
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
