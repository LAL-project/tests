/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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

#include "io_wrapper.hpp"

// C++ includes
using namespace std;

// lal includes
#include <lal/io/edge_list.hpp>
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;

namespace exe_tests {
namespace io_wrapper {

template<class G>
err_type __read_graph(const string& file, const string& format, G& g) {

	if (format == "edge-list") {
		auto r = io::read_edge_list<G>(file, true);
		if (not r) {
			cerr << ERROR << endl;
			cerr << "    When attempting to read an edge-list-formatted" << endl;
			cerr << "    graph from file: '" << file << "'." << endl;
			return err_type::io;
		}

		g = *r;
		return err_type::no_error;
	}

	cerr << ERROR << endl;
	cerr << "    Unsupported format of file: '" << format << "'." << endl;
	return err_type::test_format;
}

err_type read_graph(const string& file, const string& format, lal::graphs::undirected_graph& G) {
	return __read_graph(file, format, G);
}
err_type read_graph(const string& file, const string& format, lal::graphs::directed_graph& G) {
	return __read_graph(file, format, G);
}

} // -- namespace io_wrapper
} // -- namespace exe_tests
