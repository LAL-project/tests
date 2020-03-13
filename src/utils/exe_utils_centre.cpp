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
#include <lal/graphs/utree.hpp>
#include <lal/graphs/dtree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/utils/tree_centre.hpp>
using namespace lal;
using namespace utils;
using namespace graphs;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"

namespace exe_tests {

template<class G>
err_type exe_commands_utils_centre(ifstream& fin) {
	G t;
	uint32_t n;

	string option;
	while (fin >> option) {
		if (command_is_comment(option)) {
			process_comment(fin);
		}
		else if (option == "output") {
			cout << read_output_string(fin) << endl;
		}
		else if (option == "init") {
			fin >> n;
			t.init(n);
		}
		else if (option == "add_edges") {
			fin >> n;
			vector<edge> es(n);
			for (uint32_t i = 0; i < n; ++i) {
				fin >> es[i].first >> es[i].second;
			}
			t.add_edges(es);
		}
		else if (option == "find_centre") {
			node s;
			fin >> s;
			auto centre = utils::retrieve_centre(t, s);
			cout << "centre: " << centre.first;
			if (centre.second < t.n_nodes()) { cout << " " << centre.second; }
			cout << endl;
		}
		else if (option == "centre_is") {
			node s;
			fin >> s;
			auto centre = utils::retrieve_centre(t, s);

			uint32_t centre_size;
			fin >> centre_size;

			if (centre_size == 1) {
				node u;
				fin >> u;

				if (centre.first != u) {
					cerr << ERROR << endl;
					cerr << "    Centre does not coincide." << endl;
					return err_type::test_exe_error;
				}
			}
			else if (centre_size == 2) {
				node u, v;
				fin >> u >> v;
				if (u > v) {
					cerr << ERROR << endl;
					cerr << "    The first vertex must be smaller than the second." << endl;
					return err_type::test_format_error;
				}
				if (u == v) {
					cerr << ERROR << endl;
					cerr << "    The two vertices are the same: " << u << " <-> " << v << "." << endl;
					return err_type::test_format_error;
				}

				if (centre.first != u and centre.second != v) {
					cerr << ERROR << endl;
					cerr << "    Centres do not coincide." << endl;
					return err_type::test_exe_error;
				}
			}
			else {
				cerr << ERROR << endl;
				cerr << "    Centre size has to be either 1 or 2. Instead, received '" << centre_size << "'." << endl;
				return err_type::test_format_error;
			}
		}
		else if (option == "output_graph") {
			cout << t << endl;
		}
		else if (option == "remove_edge") {
			node u,v;
			fin >> u >> v;
			t.remove_edge(u,v);
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid command '" << option << "'." << endl;
			return err_type::test_format_error;
		}
	}
	return err_type::no_error;
}

err_type exe_utils_centre(ifstream& fin) {
	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_inputs;
	fin >> n_inputs;
	if (n_inputs != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n_inputs << " were specified." << endl;
		return err_type::test_format_error;
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
	if (graph_type != "utree" and graph_type != "dtree") {
		cerr << ERROR << endl;
		cerr << "    Expected graph type 'utree' or 'dtree'." << endl;
		cerr << "    Found '" << graph_type << "'" << endl;
		return err_type::test_format_error;
	}

	const err_type e = (
		graph_type == "utree" ?
		exe_commands_utils_centre<utree>(fin) :
		exe_commands_utils_centre<dtree>(fin)
	);

	if (e != err_type::no_error) { return e; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
