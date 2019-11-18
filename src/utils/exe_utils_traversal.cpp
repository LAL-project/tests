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

// C++ includes
#include <iostream>
#include <fstream>
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>
#include <lal/utils/cycles.hpp>
#include <lal/utils/reachability.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace utils;
using namespace graphs;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"

namespace exe_tests {

template<class G>
err_type process_instruction(const G& g, const string& command, ifstream& fin) {
	if (command == "has_cycle") {
		bool has_cycle = utils::has_cycles(g);
		cout << "Graph has cycle? "
			 << (has_cycle ? "yes" : "no") << endl;
		return err_type::no_error;
	}

	if (command == "is_reachable") {
		node u, v;
		fin >> u >> v;
		bool reach = utils::is_node_reachable_from(g, u, v);
		cout << "Is '" << v << "' reachable from '" << u << "'? "
			 << (reach ? "yes" : "no") << endl;
		return err_type::no_error;
	}

	if (command == "output_graph") {
		cout << g << endl;
		return err_type::no_error;
	}

	cerr << ERROR << endl;
	cerr << "    Unrecognised command '" << command << "'" << endl;
	return err_type::test_format_error;
}

template<class G>
err_type test_with_graph(const G& g, ifstream& fin) {
	string command;
	while (fin >> command) {
		err_type r = process_instruction(g, command, fin);
		if (r != err_type::no_error) {
			return r;
		}
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
	err_type r = io_wrapper::read_graph(graph_name, graph_format, g);
	if (r != err_type::no_error) { return r; }
	return test_with_graph(g, fin);
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
