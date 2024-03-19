/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
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

// C++ includes
#include <iostream>
#include <fstream>
#include <vector>

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/properties/branchless_path_find.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace properties {

typedef lal::properties::branchless_path bp;

template <class tree_t>
err_type execute_program(std::ifstream& fin) noexcept {
	tree_t t;

	std::string command;
	while (fin >> command) {

		if (command == "init") {
			uint64_t n;
			fin >> n;
			t.init(n);
		}
		else if (command == "clear") {
			t.clear();
		}
		else if (command == "add_edge") {
			lal::node u, v;
			fin >> u >> v;
			t.add_edge(u,v);
		}
		else if (command == "print_tree") {
			std::cout << "-------------\n";
			std::cout << "Tree:\n";
			std::cout << t << '\n';
			std::cout << "-------------\n";
		}
		else if (command == "print_paths") {
			const std::vector<bp> bps = lal::properties::find_all_branchless_paths(t);
			std::cout << "Total paths: " << bps.size() << '\n';
			for (std::size_t i = 0; i < bps.size(); ++i) {
				const bp& p = bps[i];
				const lal::node h1 = p.get_h1();
				const lal::node h2 = p.get_h2();
				std::cout << i << ")\n";
				std::cout << "    Number of vertices:   " << p.get_num_nodes() << '\n';
				std::cout << "    Number of edges:      " << p.get_num_edges() << '\n';
				if (h1 >= t.get_num_nodes()) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Path does not have its first endpoint.\n";
					return err_type::test_execution;
				}
				std::cout << "    First endpoint:       " << h1 << " -- degree: " << t.get_degree(h1) << '\n';
				if (h2 >= t.get_num_nodes()) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Path does not have its second endpoint.\n";
					return err_type::test_execution;
				}
				std::cout << "    Second endpoint:      " << h2 << " -- degree: " << t.get_degree(h2) << '\n';
				if (h1 == h2) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Singularity!\n";
					std::cerr << "    h1 is equal to h2!\n";
					return err_type::test_execution;
				}
				std::cout << "    Lowest lexicographic: " << p.get_lowest_lexicographic() << '\n';
				if (p.get_num_nodes() <= 2) {
					if (p.get_lowest_lexicographic() < t.get_num_nodes()) {
						std::cerr << ERROR << '\n';
						std::cerr << "    Path has a lower lexicographic internal vertex but it should not have any.\n";
						return err_type::test_execution;
					}
				}
				else {
					if (p.get_lowest_lexicographic() >= t.get_num_nodes()) {
						std::cerr << ERROR << '\n';
						std::cerr << "    Path does not have a lower lexicographic internal vertex but it should have one.\n";
						return err_type::test_execution;
					}
				}
				/*
				std::cout << "    Is antenna:           " << p.is_antenna(t) << '\n';
				if (p.is_antenna(t)) {
					if (t.get_degree(h1) != 1 and t.get_degree(h2) != 1) {
						std::cerr << ERROR << '\n';
						std::cerr << "    Path is said to be an antenna but non of its endpoints is a leaf.\n";
						return err_type::test_execution;
					}
				}
				*/
				std::cout << "    Vertex sequence:     ";
				for (lal::node u : p.get_vertex_sequence()) {
					std::cout << ' ' << u;

					if (u != h1 and u != h2) {
						if (t.get_degree(u) != 2) {
							std::cerr << ERROR << '\n';
							std::cerr << "    Degree of internal path vertex '" << u << "' is not 2.\n";
							std::cerr << "    Degree: " << t.get_degree(u) << ".\n";
							return err_type::test_execution;
						}
					}
				}
				std::cout << '\n';
			}
		}

	}

	return err_type::no_error;
}

err_type exe_properties_branchless_paths(std::ifstream& fin) noexcept {
	std::string mode;
	fin >> mode;
	err_type err;

	if (mode == "free") {
		err = execute_program<lal::graphs::free_tree>(fin);
	}
	else if (mode == "rooted") {
		err = execute_program<lal::graphs::rooted_tree>(fin);
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "    Incorrect mode '" << mode << "'.\n";
		return err_type::test_format;
	}

	if (err != err_type::no_error) {
		return err;
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
