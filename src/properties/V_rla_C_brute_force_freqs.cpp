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

#include "C_rla_brute_force_algorithms.hpp"

// C includes
#include <assert.h>
#include <string.h>
#include <omp.h>

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/internal/data_array.hpp>
using namespace lal;
using namespace numeric;
using namespace graphs;

enum class frequency_type : int8_t {
	invalid,
	f00, f01, f021, f022,
	f03, f04, f12, f13, f24
};

namespace exe_tests {

/* UTILITIES */

inline constexpr
int tau(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	return
		static_cast<int>(st == wx or st == yz) +
		static_cast<int>(uv == wx or uv == yz);
}

inline constexpr
int share(const edge& e1, const edge& e2) {
	return
		static_cast<int>(e1.first == e2.first or e1.first == e2.second) +
		static_cast<int>(e1.second == e2.first or e1.second == e2.second);
}

inline constexpr
int phi(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	return share(st, wx) + share(st, yz) + share(uv, wx) + share(uv, yz);
}

inline constexpr
int subtype(const edge& st, const edge& uv, const edge& wx, const edge& yz) {
	const int e1e3 = share(st, wx);
	const int e1e4 = share(st, yz);
	const int e2e3 = share(uv, wx);
	const int e2e4 = share(uv, yz);

	if (e1e3 == 1 and e1e4 == 0 and e2e3 == 0 and e2e4 == 1) {
		return 2;
	}
	if (e1e3 == 0 and e1e4 == 1 and e2e3 == 1 and e2e4 == 0) {
		return 2;
	}
	return 1;
}

inline constexpr
frequency_type edge_pair_type(const edge_pair& ep1, const edge_pair& ep2)
{
	const edge& st = ep1.first;
	const edge& uv = ep1.second;
	const edge& wx = ep2.first;
	const edge& yz = ep2.second;

	const int t = tau(st, uv, wx, yz);
	const int p = phi(st, uv, wx, yz);

	if (t == 2) {
		// the only frequency type with tau=2
		return frequency_type::f24;
	}

	if (t == 1) {
		if (p == 2) {
			return frequency_type::f12;
		}
		if (p == 3) {
			return frequency_type::f13;
		}
		return frequency_type::invalid;
	}

	// tau=0
	if (p == 4) {
		return frequency_type::f04;
	}
	if (p == 3) {
		return frequency_type::f03;
	}
	if (p == 1) {
		return frequency_type::f01;
	}
	if (p == 0) {
		return frequency_type::f00;
	}

	// tau=0, phi=2
	const int s = subtype(st, uv, wx, yz);
	if (s == 1) {
		return frequency_type::f021;
	}
	if (s == 2) {
		// the remaining true frequency
		return frequency_type::f022;
	}

	return frequency_type::invalid;
}

#define classify(ft, F021, F022, F03, F04, F12, F13, F24)	\
	switch (ft) {											\
	case frequency_type::f021: F021 += 1; break;			\
	case frequency_type::f022: F022 += 1; break;			\
	case frequency_type::f03: F03 += 1; break;				\
	case frequency_type::f04: F04 += 1; break;				\
	case frequency_type::f12: F12 += 1; break;				\
	case frequency_type::f13: F13 += 1; break;				\
	case frequency_type::f24: F24 += 1; break;				\
	case frequency_type::invalid:							\
		/* this should never happen */						\
		assert(false);										\
		/* 'break' is added so the compiler doesn't cry */	\
		break;												\
	default:												\
		/* do nothing */									\
		;													\
	}

// ---------------------
// Number of crossings C

rational variance_C_freqs_rational(const undirected_graph& g) {
	// compute set Q(g)
	const auto Q = g.get_Q();
	return variance_C_freqs_Q_rational(Q);
}

rational variance_C_freqs_Q_rational(const vector<edge_pair>& Q) {
	// frequencies f00 and f01 are not measured
	// because they have expectation 0

	static const rational exps[9] =
	{
		rational(0),		// 0, f00:   0
		rational(2, 9),		// 1, f24:   2/9
		rational(1, 18),	// 2, f13:   1/18
		rational(1, 45),	// 3, f12:   1/45
		rational(-1, 9),	// 4, f04:  -1/9
		rational(-1, 36),	// 5, f03:  -1/36
		rational(-1, 90),	// 6, f021: -1/90
		rational(1, 180),	// 7, f022:  1/180
		rational(0)			// 8, f01:   0
	};

	// values of the frequencies
	uint32_t f021(0), f022(0),	f03(0);
	uint32_t f04(0),  f12(0), 	f13(0);
	uint32_t f24(0);

	// for a small enough set Q
	for (const edge_pair& q1 : Q) {
		for (const edge_pair& q2 : Q) {
			frequency_type ft = edge_pair_type(q1, q2);
			classify(ft, f021, f022, f03, f04, f12, f13, f24);
		}
	}

	// value of V_rla[C]
	rational V(0);

	{
	integer J(0);
	//J.init_ui(f00);  V += exps[0]*f00;
	J.set_ui(f24);  V += exps[1]*J;
	J.set_ui(f13);  V += exps[2]*J;
	J.set_ui(f12);  V += exps[3]*J;
	J.set_ui(f04);  V += exps[4]*J;
	J.set_ui(f03);  V += exps[5]*J;
	J.set_ui(f021); V += exps[6]*J;
	J.set_ui(f022); V += exps[7]*J;
	//J.set_ui(f01);  V += exps[8]*f01;
	}

	return V;
}

} // -- namespace exe_tests
