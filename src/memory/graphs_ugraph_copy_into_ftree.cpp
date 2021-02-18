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

#include "graphs.hpp"

// C++ includes
using namespace std;

// lal includes
using namespace lal;
using namespace graphs;

namespace exe_tests {

void test_copy_ugraph_into_ftree() {
	begin_function;

	// move constructor
	{
	begin_case;
	undirected_graph g1(1);
	output_graph("g1",g1);

	cout << "Move constructor: 'g2 <- g1'" << endl;
	free_tree g2 = g1;
	output_graph("g1",g1);
	output_free_tree_info("g2",g2);
	}

	{
	begin_case;
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,3), edge(1,5), edge(2,4)});
	output_graph("g1",g1);

	cout << "Move constructor: 'g2 <- g1'" << endl;
	free_tree g2 = g1;
	output_graph("g1",g1);
	output_free_tree_info("g2",g2);
	}

	{
	begin_case;
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,3), edge(1,5), edge(2,4)});
	output_graph("g1",g1);

	cout << "Move constructor: 'v[0] <- g1'" << endl;
	vector<free_tree> v;
	v.push_back(g1);
	output_graph("g1",g1);
	output_free_tree_info("v[0]",v[0]);
	}

	// move operator
	{
	begin_case;
	undirected_graph g1(1);
	output_graph("g1",g1);

	cout << "Move operator: 'g2 <- g1'" << endl;
	free_tree g2;
	g2 = g1;
	output_graph("g1",g1);
	output_free_tree_info("g2",g2);
	}

	{
	begin_case;
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,3), edge(1,5), edge(2,4)});
	output_graph("g1",g1);

	cout << "Move operator: 'g2 <- g1'" << endl;
	free_tree g2;
	g2 = g1;
	output_graph("g1",g1);
	output_free_tree_info("g2",g2);
	}

	{
	begin_case;
	undirected_graph g1(6);
	g1.add_edges(vector<edge>{edge(0,1), edge(0,2), edge(1,3), edge(1,5), edge(2,4)});
	output_graph("g1",g1);

	cout << "Move operator: 'v[0] <- g1'" << endl;
	vector<free_tree> v;
	v.push_back(free_tree());
	v[0] = g1;
	output_graph("g1",g1);
	output_free_tree_info("v[0]",v[0]);
	}
}

} // -- namespace exe_tests
