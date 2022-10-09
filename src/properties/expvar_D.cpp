/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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
#include <functional>
#include <iostream>
#include <fstream>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/D_rla.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace properties {

namespace ExpVar_D {

void output_ExpVar_D_formula(const lal::graphs::undirected_graph& g) noexcept {
	const lal::numeric::rational Vr = lal::properties::var_sum_edge_lengths_rational(g);
	const lal::numeric::rational E1r = lal::properties::exp_sum_edge_lengths_rational(g);
	const lal::numeric::rational E2r = Vr + E1r*E1r;
	std::cout << E1r << "\t" << E2r << "\t" << Vr << "\t\n";
}

} // -- namespace ExpVar_D

err_type exe_properties_ExpVar_D(const input_list& inputs, std::ifstream& fin)
noexcept
{
	std::string proc;
	fin >> proc;

	if (proc != "formula") {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Received '" << proc << "'.\n";
		return err_type::test_format;
	}

	// read the graph type so that we can call
	// the appropriate function for this graph
	// type's variance.
	std::string graph_type;
	fin >> graph_type;

	lal::graphs::undirected_graph G;
	for (std::size_t i = 0; i < inputs.size(); ++i) {
		err_type r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, G);
		if (r != err_type::no_error) {
			return r;
		}
		if (proc == "formula") {
			ExpVar_D::output_ExpVar_D_formula(G);
		}
	}
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
