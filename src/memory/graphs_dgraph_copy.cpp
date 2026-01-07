/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2026
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#include "graph_utils.hpp"

namespace tests {
namespace memory {

// NOTE: blank lines left intentionally. DO NOT REMOVE or the tests will
// fail with error (because the number lines of the new execution will not
// match the number lines in the base output).
//
//
//

void test_copy_directed_graph() noexcept
{
	begin_function;

	// copy constructor
	{
		begin_case;
		lal::graphs::directed_graph g1(10);
		output_graph("g1", g1);

		std::cout << "Copy constructor: 'g2 <- g1'\n";
		lal::graphs::directed_graph g2 = g1;
		output_graph("g1", g1);
		output_graph("g2", g2)
	}

	{
		begin_case;
		lal::graphs::directed_graph g1(6);
		g1.add_edges(std::vector<lal::edge>{
			lal::edge(0, 1),
			lal::edge(0, 2),
			lal::edge(1, 2),
			lal::edge(1, 5),
			lal::edge(2, 4)
		});
		output_graph("g1", g1);

		std::cout << "Copy constructor: 'g2 <- g1'\n";
		lal::graphs::directed_graph g2 = g1;
		output_graph("g1", g1);
		output_graph("g2", g2)
	}

	{
		begin_case;
		lal::graphs::directed_graph g1(6);
		g1.add_edges(std::vector<lal::edge>{
			lal::edge(0, 1),
			lal::edge(0, 2),
			lal::edge(1, 2),
			lal::edge(1, 5),
			lal::edge(2, 4)
		});
		output_graph("g1", g1);

		std::cout << "Copy constructor: 'v[0] <- g1'\n";
		std::vector<lal::graphs::directed_graph> v;
		v.push_back(g1);
		output_graph("g1", g1);
		output_graph("v[0]", v[0]);
	}

	// copy operator
	{
		begin_case;
		lal::graphs::directed_graph g1(10);
		output_graph("g1", g1);

		std::cout << "Copy operator: 'g2 <- g1'\n";
		lal::graphs::directed_graph g2;
		g2 = g1;
		output_graph("g1", g1);
		output_graph("g2", g2);
	}

	{
		begin_case;
		lal::graphs::directed_graph g1(6);
		g1.add_edges(std::vector<lal::edge>{
			lal::edge(0, 1),
			lal::edge(0, 2),
			lal::edge(1, 2),
			lal::edge(1, 5),
			lal::edge(2, 4)
		});
		output_graph("g1", g1);

		std::cout << "Copy operator: 'g2 <- g1'\n";
		lal::graphs::directed_graph g2;
		g2 = g1;
		output_graph("g1", g1);
		output_graph("g2", g2);
	}

	{
		begin_case;
		lal::graphs::directed_graph g1(6);
		g1.add_edges(std::vector<lal::edge>{
			lal::edge(0, 1),
			lal::edge(0, 2),
			lal::edge(1, 2),
			lal::edge(1, 5),
			lal::edge(2, 4)
		});
		output_graph("g1", g1);

		std::cout << "Copy operator: 'v[0] <- g1'\n";
		std::vector<lal::graphs::directed_graph> v;
		v.push_back(lal::graphs::directed_graph());
		v[0] = g1;
		output_graph("g1", g1);
		output_graph("v[0]", v[0]);
	}
}

} // namespace memory
} // namespace tests
