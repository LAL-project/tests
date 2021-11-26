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
#include <cassert>
#include <fstream>
#include <cmath>
#include <set>

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/linarr/C.hpp>
#include <lal/io/basic_output.hpp>

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/std_utils.hpp"
#include "common/time.hpp"

#define to_int64(x) static_cast<int64_t>(x)
#define to_uint64(x) static_cast<uint64_t>(x)

namespace tests {
namespace linarr {

template<typename INT>
inline constexpr bool common_endpoints(INT s1, INT d1, INT s2, INT d2) {
	/*if (s1 == s2) { return true; }
	if (s1 == s2 + d2) { return true; }
	if (s1 + d1 == s2) { return true; }
	if (s1 + d1 == s2 + d2) { return true; }*/
	return (s1 == s2) or (s1 == s2 + d2) or (s1 + d1 == s2) or (s1 + d1 == s2 + d2);
}

uint64_t alpha(uint64_t n, uint64_t d1, uint64_t d2) {
	uint64_t c = 0;
	for (uint64_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }
		for (uint64_t s2 = 1; s2 <= n; ++s2) {
			//if (s2 + d2 > n) { continue; }
			//if (common_endpoints(s1,d1, s2,d2)) { continue; }
			const bool cond1 = s2 + d2 > n;
			const bool cond2 = common_endpoints(s1,d1, s2,d2);

			// no common endpoints
			const bool cross1 = s1 < s2 and s2 < s1 + d1 and s1 + d1 < s2 + d2;
			const bool cross2 = s2 < s1 and s1 < s2 + d2 and s2 + d2 < s1 + d1;
			c += (not cond1 and not cond2)*(cross1 or cross2);
		}
	}
	return c;
}

uint64_t beta(uint64_t n, uint64_t d1, uint64_t d2) {
	uint64_t c = 0;
	for (uint64_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }
		for (uint64_t s2 = 1; s2 <= n; ++s2) {
			//if (s2 + d2 > n) { continue; }
			//if (common_endpoints(s1,d1, s2,d2)) { continue; }
			const bool cond1 = s2 + d2 > n;
			const bool cond2 = common_endpoints(s1,d1, s2,d2);

			// no common endpoints
			c += (not cond1 and not cond2);
		}
	}
	return c;
}

template<typename GRAPH>
lal::numeric::rational E_2Cd_brute_force(GRAPH& g, const lal::linear_arrangement& pi) {
	lal::numeric::rational Ec2(0);
	const uint64_t n = g.get_num_nodes();

	for (lal::iterators::Q_iterator<GRAPH> q_it(g); not q_it.end(); q_it.next()) {
		const lal::edge_pair st_uv = q_it.get_edge_pair();
		const lal::edge st = st_uv.first;
		const lal::edge uv = st_uv.second;
		const lal::node_t s = st.first;
		const lal::node_t t = st.second;
		const lal::node_t u = uv.first;
		const lal::node_t v = uv.second;

		uint64_t al;
		uint64_t be;

		const uint64_t len_st =
			to_uint64(std::abs(to_int64(pi[s]) - to_int64(pi[t])));

		const uint64_t len_uv =
			to_uint64(std::abs(to_int64(pi[u]) - to_int64(pi[v])));

		assert(len_st <= n);
		assert(len_uv <= n);

		if (len_st <= len_uv) {
			al = alpha(n, len_st, len_uv);
			be = beta(n, len_st, len_uv);
		}
		else {
			al = alpha(n, len_uv, len_st);
			be = beta(n, len_uv, len_st);
		}

		Ec2 += lal::numeric::rational(to_int64(al), be);
	}

	return Ec2;
}

err_type exe_linarr_approx_Exp_C(const input_list& inputs, std::ifstream& fin) {
	std::set<std::string> allowed_procs({"E_2[C|d]"});

	if (inputs.size() != 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Only one input file si allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	lal::graphs::undirected_graph uG;
	lal::graphs::directed_graph dG;
	{
	const std::string graph_name = inputs[0].first;
	const std::string graph_format = inputs[0].second;
	err_type r;
	r = io_wrapper::read_graph(graph_name, graph_format, uG);
	if (r != err_type::no_error) { return r; }
	r = io_wrapper::read_graph(graph_name, graph_format, dG);
	if (r != err_type::no_error) { return r; }
	}

	std::string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Wrong value for procedure type.\n";
		std::cerr << "    Procedure '" << proc << "' was found.\n";
		return err_type::test_format;
	}

	// linear arrangement
	const uint64_t n = uG.get_num_nodes();
	lal::linear_arrangement pi(n);

	// amount of linear arrangements
	uint64_t n_linarrs;
	fin >> n_linarrs;

	for (std::size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		lal::node u;
		for (lal::position pu = 0; pu < n; ++pu) {
			fin >> u;
			pi.assign(u, pu);
		}

		// compute value using library and compare it with brute force method
		const lal::numeric::rational ap_lib_u = lal::linarr::predicted_num_crossings_rational(uG, pi);
		const lal::numeric::rational ap_lib_d = lal::linarr::predicted_num_crossings_rational(dG, pi);
		if (ap_lib_d != ap_lib_u) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Library's values for directed and undirected graphs\n";
			std::cerr << "    do not coincide.\n";
			std::cerr << "        ap_lib_u= " << ap_lib_u << '\n';
			std::cerr << "        ap_lib_d= " << ap_lib_d << '\n';
			std::cerr << "    For (inverse) linear arrangement: [" << pi.inverse_as_vector() << "]\n";
			return err_type::test_execution;
		}

		const lal::numeric::rational ap_bf_u = E_2Cd_brute_force(uG, pi);
		const lal::numeric::rational ap_bf_d = E_2Cd_brute_force(dG, pi);
		if (ap_lib_d != ap_lib_u) {
			std::cerr << ERROR << '\n';
			std::cerr << "    Brute force values for directed and undirected graphs\n";
			std::cerr << "    do not coincide.\n";
			std::cerr << "        ap_bf_u= " << ap_bf_u << '\n';
			std::cerr << "        ap_bf_d= " << ap_bf_d << '\n';
			std::cerr << "    For (inverse) linear arrangement: [" << pi.inverse_as_vector() << "]\n";
			return err_type::test_execution;
		}

		if (ap_lib_u != ap_bf_u) {
			std::cerr << ERROR << '\n';
			std::cerr << "    The value of E_2[C|d] using the library is not equal to the\n";
			std::cerr << "    brute force value.\n";
			std::cerr << "        Library's value: " << ap_lib_u << '\n';
			std::cerr << "        Brute force's value: " << ap_bf_u << '\n';
			std::cerr << "    For (inverse) linear arrangement: [" << pi.inverse_as_vector() << "]\n";
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace common
} // -- namespace tests
