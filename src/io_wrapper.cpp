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

#include "io_wrapper.hpp"

// lal includes
#include <lal/io/edge_list.hpp>
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;

namespace io_wrapper {

template<class G>
err_type __read_graph(const string& file, const string& format, G& g) {

	if (format == "edge-list") {
		bool r = io::read_edge_list(file, g, true);
		if (not r) {
			cerr << ERROR << endl;
			cerr << "    When attempting to read an edge-list-formatted" << endl;
			cerr << "    graph from file: '" << file << "'." << endl;
			return err_type::io_error;
		}

		return err_type::no_error;
	}

	cerr << ERROR << endl;
	cerr << "    Unsupported format of file: '" << format << "'." << endl;
	return err_type::test_format_error;
}

err_type read_graph(const string& file, const string& format, ugraph& G) {
	return __read_graph(file, format, G);
}
err_type read_graph(const string& file, const string& format, dgraph& G) {
	return __read_graph(file, format, G);
}


} // -- namespace io_wrapper
