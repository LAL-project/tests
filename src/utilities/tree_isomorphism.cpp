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
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/utilities/tree_isomorphism.hpp>
using namespace lal;
using namespace graphs;
using namespace utilities;

// custom includes
#include "definitions.hpp"

#define ARBOR rooted_tree::rooted_tree_type::arborescence
#define ANTIARBOR rooted_tree::rooted_tree_type::anti_arborescence

namespace exe_tests {

template<class T>
void read_free(ifstream& fin, T& t) {
	vector<edge> edges(t.n_nodes() - 1);
	for (auto& e : edges) {
		fin >> e.first >> e.second;
	}
}

template<class T>
void read_rooted(ifstream& fin, T& t) {
	read_free(fin, t);
	node r;
	fin >> r;
	t.set_root(r);
}

err_type free_isomorphism_test(ifstream& fin) {
	uint32_t n;
	while (fin >> n) {
		free_tree t1(n), t2(n);
		read_free(fin, t1);
		read_free(fin, t2);

		cout << "Are isomorphic? "
			 << std::boolalpha
			 << utilities::are_trees_isomorphic(t1,t2)
			 << endl;
	}
	return err_type::no_error;
}

err_type rooted_isomorphism_test(ifstream& fin) {
	uint32_t n;
	while (fin >> n) {
		rooted_tree t1(n), t2(n);
		read_rooted(fin, t1);
		read_rooted(fin, t2);

		const bool has_t1 = t1.find_rooted_tree_type();
		const bool has_t2 = t2.find_rooted_tree_type();

		// ensure correct format of trees
		if (not has_t1) {
			cerr << ERROR << endl;
			cerr << "    Rooted tree" << endl;
			cerr << t1 << endl;
			cerr << "    is neither an arborescence or an anti-arborescence" << endl;
			return err_type::test_format_error;
		}
		if (not has_t2) {
			cerr << ERROR << endl;
			cerr << "    Rooted tree" << endl;
			cerr << t2 << endl;
			cerr << "    is neither an arborescence or an anti-arborescence" << endl;
			return err_type::test_format_error;
		}

		cout << "Are isomorphic? "
			 << std::boolalpha
			 << utilities::are_trees_isomorphic(t1,t2)
			 << endl;
	}
	return err_type::no_error;
}

err_type exe_utilities_tree_isomorphism(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	string tree_type;
	fin >> tree_type;
	if (tree_type != "free" and tree_type != "rooted") {
		cerr << ERROR << endl;
		cerr << "    Invalid tree type '" << tree_type << "'." << endl;
		cerr << "    Must be one of: free/rooted" << endl;
		return err_type::test_format_error;
	}

	const auto r = (
		tree_type == "free" ?
		free_isomorphism_test(fin) :
		rooted_isomorphism_test(fin)
	);

	if (r != err_type::no_error) {
		TEST_GOODBYE
		return err_type::no_error;
	}
	return r;
}

} // -- namespace exe_tests
