/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019
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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

// C++ includes
#include <iostream>
#include <cmath>
#include <set>
using namespace std;

// lal includes
#include <lal/ugraph.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/properties/C_rla.hpp>
#include <lal/linarr/C.hpp>
using namespace lal;
using namespace lal::numeric;
using namespace lal::properties;
using namespace lal::linarr;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"
#include "../test_utils.hpp"
#include "../time.hpp"
using namespace test_utils;

namespace exe_tests {

int64_t alpha(int64_t n, int64_t d1, int64_t d2) {
	int64_t c = 0;
	for (int64_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }

		for (int64_t s2 = 1; s2 <= n; ++s2) {
			if (s2 + d2 > n) { continue; }

			// no common endpoints (there are redundant checks, I know)
			if (s1 == s2 or s1 + d1 == s2 or s1 + d1 == s2 + d2) { continue; }
			if (s2 == s1 or s2 + d2 == s1 or s2 + d2 == s1 + d1) { continue; }

			bool cross1 = s1 < s2 and s2 < s1 + d1 and s1 + d1 < s2 + d2;
			bool cross2 = s2 < s1 and s1 < s2 + d2 and s2 + d2 < s1 + d1;
			c += cross1 or cross2;
		}
	}
	return c;
}

int64_t beta(int64_t n, int64_t d1, int64_t d2) {
	int64_t c = 0;
	for (int64_t s1 = 1; s1 <= n; ++s1) {
		if (s1 + d1 > n) { continue; }

		for (int64_t s2 = 1; s2 <= n; ++s2) {
			if (s2 + d2 > n) { continue; }

			// no common endpoints (there are redundant checks, I know)
			if (s1 == s2 or s1 + d1 == s2 or s1 + d1 == s2 + d2) { continue; }
			if (s2 == s1 or s2 + d2 == s1 or s2 + d2 == s1 + d1) { continue; }

			++c;
		}
	}
	return c;
}

rational E_2Cd_brute_force(ugraph& g, const vector<node>& T) {
	rational Ec2(0);
	const uint32_t n = g.n_nodes();

	// actual linear arrangement (following notation used in the thesis):
	// pi[u] = p <-> node u is at position p
	int64_t *pi = static_cast<int64_t *>( malloc(n*sizeof(int64_t)) );
	for (uint32_t i = 0; i < n; ++i) {
		pi[ T[i] ] = i;
	}

	for (node s = 0; s < n; ++s) {
		const neighbourhood& Nu = g.get_neighbours(s);
		for (node t : Nu) {
			if (s > t) { continue; }
			// unique edge {s,t}

			for (node u = s + 1; u < n; ++u) {
				const neighbourhood& Nw = g.get_neighbours(u);
				for (node v : Nw) {
					if (u > v) { continue; }
					// unique edge {u,v}

					// s != u and t != u
					if (s == v or s == u) { continue; }
					if (t == v or t == u) { continue; }

					int64_t len_st = std::abs(pi[s] - pi[t]);
					int64_t len_uv = std::abs(pi[u] - pi[v]);

					int64_t al;
					uint64_t be;
					if (len_st <= len_uv) {
						al = alpha(n, len_st, len_uv);
						be = static_cast<uint64_t>(beta(n, len_st, len_uv));
					}
					else {
						al = alpha(n, len_uv, len_st);
						be = static_cast<uint64_t>(beta(n, len_uv, len_st));
					}

					Ec2 += rational(al, be);
				}
			}
		}
	}

	free(pi);
	return Ec2;
}

err_type exe_properties_approx_Exp_C(ifstream& fin) {
	set<string> allowed_procs({"E_2[C|d]"});

	cout.setf(ios::fixed);
	cout.precision(4);

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR("exe_properties_approx_exp_C.cpp", "exe_properties_approx_Exp_C") << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_linarrs;
	fin >> n_linarrs;
	if (n_linarrs != 1) {
		cerr << ERROR("exe_properties_approx_exp_C.cpp", "exe_properties_approx_Exp_C") << endl;
		cerr << "    Expected only one input." << endl;
		cerr << "    Instead, '" << n_linarrs << "' were found." << endl;
		return err_type::test_format_error;
	}

	string graph_name;
	string graph_format;
	fin >> graph_name >> graph_format;

	ugraph G;
	err_type r = io_wrapper::read_graph(graph_name, graph_format, G);
	if (r != err_type::no_error) {
		return r;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR("exe_properties_approx_exp_C.cpp", "exe_properties_approx_Exp_C") << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string proc;
	fin >> proc;

	if (allowed_procs.find(proc) == allowed_procs.end()) {
		cerr << ERROR("exe_properties_approx_exp_C.cpp", "exe_properties_approx_Exp_C") << endl;
		cerr << "    Wrong value for procedure type." << endl;
		cerr << "    Procedure '" << proc << "' was found." << endl;
		return err_type::test_format_error;
	}

	// linear arrangement
	uint32_t n = G.n_nodes();
	vector<node> arr(n);

	// amount of linear arrangements
	fin >> n_linarrs;

	for (size_t i = 0; i < n_linarrs; ++i) {
		// read linear arrangement
		for (uint32_t u = 0; u < G.n_nodes(); ++u) {
			fin >> arr[u];
		}

		// compute value using library and compare it with brute force method
		rational ap_lib = approximate_C_2_rational(G, arr);
		rational ap_bf = E_2Cd_brute_force(G, arr);

		if (ap_lib != ap_bf) {
			cerr << ERROR("exe_properties_approx_exp_C.cpp", "exe_properties_approx_Exp_C") << endl;
			cerr << "    The value of E_2[C|d] using the library is not equal to the." << endl;
			cerr << "    brute force value." << endl;
			cerr << "    Library's value: " << ap_lib << endl;
			cerr << "    Brute force's value: " << ap_bf << endl;
		}
	}

	cout << "Test finished without apparent errors." << endl;
	return err_type::no_error;
}

} // -- namespace exe_tests
