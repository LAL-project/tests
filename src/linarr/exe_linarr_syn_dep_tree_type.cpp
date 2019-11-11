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

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// lal includes
#include <lal/graphs/urtree.hpp>
#include <lal/linarr/classification.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"

namespace exe_tests {

err_type exe_linarr_syn_dep_tree_type(ifstream& fin) {
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
	if (n_inputs != 1) {
		cerr << ERROR << endl;
		cerr << "    Expected only one input." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
		return err_type::test_format_error;
	}

	string graph_name;
	string graph_format;
	fin >> graph_name >> graph_format;

	urtree tree;
	err_type r = io_wrapper::read_graph(graph_name, graph_format, tree);
	if (r != err_type::no_error) {
		return r;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	// linear arrangement
	const uint32_t n = tree.n_nodes();
	vector<node> T(n), pi(n);
	node root;

	// amount of linear arrangements
	size_t n_linarrs;
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read root
		fin >> root;
		// read linear arrangement
		for (uint32_t u = 0; u < tree.n_nodes(); ++u) {
			fin >> T[u];
			pi[ T[u] ] = u;
		}

		tree.set_root(root);
		linarr::get_syn_dep_tree_type(tree, pi);
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
