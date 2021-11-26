/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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
#include <iostream>
#include <fstream>
#include <random>
#include <set>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/output.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/degrees.hpp>
#include <lal/properties/Q.hpp>
#include <lal/properties/mean_hierarchical_distance.hpp>
#include <lal/detail/graphs/is_tree.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"

namespace tests {
namespace properties {

lal::graphs::directed_graph make_rand_dgraph(const lal::graphs::undirected_graph& g) {
	// coin flips
	std::default_random_engine gen(1234);
	std::uniform_int_distribution<int> U(0,1);

	// directed graph
	lal::graphs::directed_graph dg(g.get_num_nodes());
	std::vector<lal::edge> es = g.get_edges();

	const std::size_t m = es.size();
	for (std::size_t i = 0; i < m; ++i) {
		lal::edge& e = es[i];

		// add lal::edge in one orientation or the other
		const int c1 = U(gen);
		if (c1 == 1) {
			std::swap(e.first, e.second);
		}

		// add (or not) the opposite lal::edge
		const int c2 = U(gen);
		if (c2 == 1) {
			lal::edge flip = e;
			std::swap(flip.first, flip.second);
			es.push_back(flip);
		}
	}

	// construct graph and finish
	dg.add_edges(es);
	return dg;
}

template<class G>
void enum_E(const G& g) {
	std::size_t total = 0;

	std::cout << "Elements of E:\n";

	for (lal::iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const auto [u,v] = e_it.get_edge();
		std::cout << "(" << u << "," << v << ")\n";
		++total;
	}
	std::cout << "Total number of elements: " << total << '\n';
}

template<class G>
void enum_Q(const G& g) {
	std::size_t total = 0;

	std::cout << "Elements of Q:\n";

	for (lal::iterators::Q_iterator q_it(g); not q_it.end(); q_it.next()) {
		const lal::edge_pair& ep = q_it.get_edge_pair();
		const lal::edge& e1 = ep.first;
		const lal::edge& e2 = ep.second;
		std::cout << "("
			 << "(" << e1.first << ", " << e1.second << "), "
			 << "(" << e2.first << ", " << e2.second << ")"
			 << ")"
			 << '\n';

		++total;
	}
	std::cout << "Total number of elements: " << total << '\n';
}

template<class G>
void Q_size(const G& g) {
	const lal::numeric::integer Q = lal::properties::num_pairs_independent_edges_integer(g);
	std::cout << "size of Q: " << Q << '\n';

	const uint64_t dQ = lal::properties::num_pairs_independent_edges(g);
	std::cout << "size of Q: " << dQ << '\n';
}

//

void mmt_deg(const lal::graphs::undirected_graph& g, uint64_t p) {
	std::cout << "(udir) <k^" << p << ">= "
		 << lal::properties::moment_degree_rational(g, p)
		 << '\n';
}

void mmt_deg(const lal::graphs::directed_graph& g, uint64_t p) {
	std::cout << "( dir) <k^" << p << ">= "
		 << lal::properties::moment_degree_rational(g, p)
		 << '\n';
}

//

err_type mmt_in_deg(const lal::graphs::undirected_graph& uG, const lal::graphs::directed_graph& dG, uint64_t p) {
	std::cout << "(graph) <k_out^" << p << ">= "
		 << lal::properties::moment_in_degree_rational(dG, p)
		 << '\n';

	const bool is_tree_dG = lal::detail::is_graph_a_tree(dG);
	const bool is_tree_uG = lal::detail::is_graph_a_tree(uG);

	if (is_tree_dG != is_tree_uG) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Error in deciding whether a graph is a tree or not.\n";
		std::cerr << "    It has been determined that an undirected graph uG\n";
		std::cerr << "    " << (is_tree_uG ? "is" : "is not") << " a tree, however\n";
		std::cerr << "    it has been determined that its oriented version dG\n";
		std::cerr << "    " << (is_tree_dG ? "is" : "is not") << " a tree, hence\n";
		std::cerr << "    a contradiction.\n";
		std::cerr << "    Is undirected a tree? " << is_tree_uG << '\n';
		std::cerr << "    Is   directed a tree? " << is_tree_dG << '\n';
		std::cerr << "    Undirected graph:\n";
		std::cerr << uG << '\n';
		std::cerr << "    Directed graph:\n";
		std::cerr << dG << '\n';
		return err_type::test_execution;
	}

	if (is_tree_dG) {
		const auto n = uG.get_num_nodes();
		for (lal::node r = 0; r < n; ++r) {
			const lal::graphs::rooted_tree T(uG, r);
			const auto kin = lal::properties::moment_in_degree_rational(T, p);

			std::cout << "(rtree at " << r << ") <k_in^" << p << ">= " << kin << '\n';
			if (kin != lal::numeric::rational(n-1, n)) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Moment of in-degree is not equal to the expected value.\n";
				std::cerr << "    Expected: " << lal::numeric::rational(n-1, n) << '\n';
				std::cerr << "    Received: " << kin << '\n';
				return err_type::test_execution;
			}

		}
	}
	return err_type::no_error;
}

//

err_type mmt_out_deg(const lal::graphs::undirected_graph& uG, const lal::graphs::directed_graph& dG, uint64_t p) {
	std::cout << "(graph) <k_out^" << p << ">= "
		 << lal::properties::moment_out_degree_rational(dG, p)
		 << '\n';

	const bool is_tree_dG = lal::detail::is_graph_a_tree(dG);
	const bool is_tree_uG = lal::detail::is_graph_a_tree(uG);

	if (is_tree_dG != is_tree_uG) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Error in deciding whether a graph is a tree or not.\n";
		std::cerr << "    It has been determined that an undirected graph uG\n";
		std::cerr << "    " << (is_tree_uG ? "is" : "is not") << " a tree, however\n";
		std::cerr << "    it has been determined that its oriented version dG\n";
		std::cerr << "    " << (is_tree_dG ? "is" : "is not") << " a tree, hence\n";
		std::cerr << "    a contradiction.\n";
		std::cerr << "    Is undirected a tree? " << is_tree_uG << '\n';
		std::cerr << "    Is   directed a tree? " << is_tree_dG << '\n';
		std::cerr << "    Undirected graph:\n";
		std::cerr << uG << '\n';
		std::cerr << "    Directed graph:\n";
		std::cerr << dG << '\n';
		return err_type::test_execution;
	}

	if (is_tree_dG) {
		for (lal::node r = 0; r < uG.get_num_nodes(); ++r) {
			const lal::graphs::rooted_tree T(uG, r);

			std::cout << "(rtree at " << r << ") <k_out^" << p << ">= "
				 << lal::properties::moment_out_degree_rational(T, p)
				 << '\n';
		}
	}
	return err_type::no_error;
}

//

void hubiness_coefficient(const lal::graphs::undirected_graph& g) {
	if (not lal::detail::is_graph_a_tree(g)) { return; }
	if (g.get_num_nodes() > 3) {
		lal::numeric::rational h = lal::properties::hubiness_rational(g);
		std::cout << "hubiness= " << h << '\n';
	}
}

void MHD(const lal::graphs::undirected_graph& g, lal::node r) {
	if (not lal::detail::is_graph_a_tree(g)) { return; }
	const lal::graphs::free_tree t(g);
	const lal::graphs::rooted_tree R(t, r);
	const lal::numeric::rational mhd = lal::properties::mean_hierarchical_distance_rational(R);
	std::cout << "Mean_Hierarchical_Distance(" << r << ")= " << mhd << '\n';
}

err_type exe_properties_general(const input_list& inputs, std::ifstream& fin) {
	const std::set<std::string> allowed_instructions({
		"enumerate_E", "enumerate_E_rand_dir",
		"enumerate_Q", "enumerate_Q_rand_dir",
		"Q_size", "Q_size_rand_dir",
		"mmt_deg", "mmt_in_deg", "mmt_out_deg",
		"hubiness_coefficient", "Mean_Hierarchical_Distance"
	});

	std::string ins;
	std::string all_instructions = "";
	while (fin >> ins) {
		all_instructions += ins + " ";
	}

	// The input file has been parsed completely.
	// It is time to execute the instructions for each graph.

	lal::graphs::undirected_graph uG;
	lal::graphs::directed_graph dG;
	for (std::size_t i = 0; i < inputs.size(); ++i) {

		err_type r;

		r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, uG);
		if (r != err_type::no_error) { return r; }

		r = io_wrapper::read_graph(inputs[i].first, inputs[i].second, dG);
		if (r != err_type::no_error) { return r; }

		// this graph may have edges of the form
		// (u,v)  ..  (v,u)
		const lal::graphs::directed_graph rdG = make_rand_dgraph(uG);

		std::stringstream ss(all_instructions);
		while (ss >> ins) {
			if (ins == "enumerate_E") {
				enum_E(uG);
			}
			else if (ins == "enumerate_E_rand_dir") {
				std::cout << rdG << '\n';
				enum_E(rdG);
			}
			else if (ins == "enumerate_Q") {
				enum_Q(uG);
			}
			else if (ins == "enumerate_Q_rand_dir") {
				std::cout << rdG << '\n';
				enum_Q(rdG);
			}
			else if (ins == "Q_size") {
				Q_size(uG);
			}
			else if (ins == "Q_size_rand_dir") {
				std::cout << rdG << '\n';
				Q_size(rdG);
			}
			else if (ins == "mmt_deg") {
				uint64_t p;
				ss >> p;
				mmt_deg(uG, p);
				mmt_deg(rdG, p);
			}
			else if (ins == "mmt_in_deg") {
				uint64_t p;
				ss >> p;
				r = mmt_in_deg(uG, dG, p);
				if (r != err_type::no_error) { return r; }
			}
			else if (ins == "mmt_out_deg") {
				uint64_t p;
				ss >> p;
				r = mmt_out_deg(uG, dG, p);
				if (r != err_type::no_error) { return r; }
			}
			else if (ins == "hubiness_coefficient") {
				hubiness_coefficient(uG);
			}
			else if (ins == "Mean_Hierarchical_Distance") {
				lal::node root;
				ss >> root;
				MHD(uG, root);
			}
			else {
				std::cerr << ERROR << '\n';
				std::cerr << "    Invalid instruction.\n";
				std::cerr << "    Allowed instructions:\n";
				for (const std::string& s : allowed_instructions) {
					std::cout << "        " << s << '\n';
				}
				std::cerr << "    Instead, '" << ins << "' was found.\n";
				return err_type::test_format;
			}
		}
	}

	return err_type::no_error;
}

} // -- namespace properties
} // -- namespace tests
