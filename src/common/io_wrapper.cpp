/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#include <iostream>

// lal includes
#include <lal/io/edge_list.hpp>
#include <lal/io/head_vector.hpp>
#include <lal/graphs/output.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace io_wrapper {

template <class graph_t>
err_type __read_graph(const std::string& file, const std::string& format, graph_t& g, bool norm)
noexcept
{

	if (format == "edge_list" or format == "edge-list") {
		auto r = lal::io::read_edge_list<graph_t>(file, norm, false);
		if (not r) {
			std::cerr << ERROR << '\n';
			std::cerr << "    When attempting to read an lal::edge-list-formatted\n";
			std::cerr << "    graph from file: '" << file << "'.\n";
			return err_type::io;
		}

		g = std::move(*r);
		return err_type::no_error;
	}

	if (format == "head_vector") {
		if constexpr (
			std::is_base_of_v<lal::graphs::rooted_tree, graph_t> or
			std::is_base_of_v<lal::graphs::free_tree, graph_t>
		)
		{
			auto r = lal::io::read_head_vector<graph_t>(file, norm, false);
			if (not r) {
				std::cerr << ERROR << '\n';
				std::cerr << "    When attempting to read an lal::edge-list-formatted\n";
				std::cerr << "    graph from file: '" << file << "'.\n";
				return err_type::io;
			}

			g = std::move(*r);
			return err_type::no_error;
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Type of graph is not allowed when reading a head vector file.\n";
			return err_type::test_format;
		}
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Unsupported file format: '" << format << "'.\n";
	return err_type::test_format;
}

err_type read_graph
(const std::string& file, const std::string& format, lal::graphs::undirected_graph& G, bool norm)
noexcept
{
	return __read_graph(file, format, G, norm);
}
err_type read_graph
(const std::string& file, const std::string& format, lal::graphs::directed_graph& G, bool norm)
noexcept
{
	return __read_graph(file, format, G, norm);
}

err_type read_graph
(const std::string& file, const std::string& format, lal::graphs::free_tree& G, bool norm)
noexcept
{
	return __read_graph(file, format, G, norm);
}

err_type read_graph
(const std::string& file, const std::string& format, lal::graphs::rooted_tree& G, bool norm)
noexcept
{
	return __read_graph(file, format, G, norm);
}

} // -- namespace io_wrapper
} // -- namespace tests
