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

// C++ includes
#include <iostream>
#include <cassert>
#include <fstream>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/iterators/Q_iterator.hpp>
#include <lal/numeric/rational_output.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/linarr/C.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace iterators;
using namespace numeric;
using namespace properties;
using namespace linarr;

// common includes
#include "common/io_wrapper.hpp"
#include "common/definitions.hpp"
#include "common/time.hpp"

#define to_int32(x) static_cast<int32_t>(x)
#define to_uint32(x) static_cast<uint32_t>(x)

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

uint32_t alpha(uint32_t n, uint32_t d1, uint32_t d2) {
	uint32_t c = 0;
	for (uint32_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }
		for (uint32_t s2 = 1; s2 <= n; ++s2) {
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

uint32_t beta(uint32_t n, uint32_t d1, uint32_t d2) {
	uint32_t c = 0;
	for (uint32_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }
		for (uint32_t s2 = 1; s2 <= n; ++s2) {
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
rational E_2Cd_brute_force(GRAPH& g, const linear_arrangement& pi) {
	rational Ec2(0);
	const uint32_t n = g.get_num_nodes();

	for (iterators::Q_iterator<GRAPH> q_it(g); not q_it.end(); q_it.next()) {
		const edge_pair st_uv = q_it.get_edge_pair();
		const edge st = st_uv.first;
		const edge uv = st_uv.second;
		const node s = st.first;
		const node t = st.second;
		const node u = uv.first;
		const node v = uv.second;

		uint32_t al;
		uint32_t be;

		const uint32_t len_st = to_uint32(std::abs(to_int32(pi[s]) - to_int32(pi[t])));
		const uint32_t len_uv = to_uint32(std::abs(to_int32(pi[u]) - to_int32(pi[v])));
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

		Ec2 += rational(to_int32(al), be);
	}

	return Ec2;
}

err_type exe_linarr_approx_Exp_C(const input_list& inputs, ifstream& fin) {
	set<string> allowed_procs({"E_2[C|d]"});

	if (inputs.size() != 1) {
		cerr << ERROR << endl;
		cerr << "    Only one input file si allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	undirected_graph uG;
	directed_graph dG;
	{
	const string graph_name = inputs[0].first;
	const string graph_format = inputs[0].second;
	err_type r;
	r = io_wrapper::read_graph(graph_name, graph_format, uG);
	if (r != err_type::no_error) { return r; }
	r = io_wrapper::read_graph(graph_name, graph_format, dG);
	if (r != err_type::no_error) { return r; }
	}

	string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		cerr << ERROR << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		return err_type::test_format;
	}

	// linear arrangement
	const uint32_t n = uG.get_num_nodes();
	vector<node> T(n);
	linear_arrangement pi(n);

	// amount of linear arrangements
	uint32_t n_linarrs;
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (node u = 0; u < uG.get_num_nodes(); ++u) {
			fin >> T[u];
			pi[ T[u] ] = u;
		}

		// compute value using library and compare it with brute force method
		const rational ap_lib_u = predicted_num_crossings_rational(uG, pi);
		const rational ap_lib_d = predicted_num_crossings_rational(dG, pi);
		if (ap_lib_d != ap_lib_u) {
			cerr << ERROR << endl;
			cerr << "    Library's values for directed and undirected graphs" << endl;
			cerr << "    do not coincide." << endl;
			cerr << "        ap_lib_u= " << ap_lib_u << endl;
			cerr << "        ap_lib_d= " << ap_lib_d << endl;
			cerr << "    For (inverse) linear arrangement: [" << T[0];
			for (size_t _i = 1; _i < T.size(); ++_i) {
				cerr << ", " << T[_i];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}

		const rational ap_bf_u = E_2Cd_brute_force(uG, pi);
		const rational ap_bf_d = E_2Cd_brute_force(dG, pi);
		if (ap_lib_d != ap_lib_u) {
			cerr << ERROR << endl;
			cerr << "    Brute force values for directed and undirected graphs" << endl;
			cerr << "    do not coincide." << endl;
			cerr << "        ap_bf_u= " << ap_bf_u << endl;
			cerr << "        ap_bf_d= " << ap_bf_d << endl;
			cerr << "    For (inverse) linear arrangement: [" << T[0];
			for (size_t _i = 1; _i < T.size(); ++_i) {
				cerr << ", " << T[_i];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}

		if (ap_lib_u != ap_bf_u) {
			cerr << ERROR << endl;
			cerr << "    The value of E_2[C|d] using the library is not equal to the" << endl;
			cerr << "    brute force value." << endl;
			cerr << "        Library's value: " << ap_lib_u << endl;
			cerr << "        Brute force's value: " << ap_bf_u << endl;
			cerr << "    For (inverse) linear arrangement: [" << T[0];
			for (size_t _i = 1; _i < T.size(); ++_i) {
				cerr << ", " << T[_i];
			}
			cerr << "]" << endl;
			return err_type::test_execution;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace common
} // -- namespace tests
