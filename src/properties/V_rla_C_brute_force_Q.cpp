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

#include "C_rla_brute_force_algorithms.hpp"

// C++ includes
#include <vector>

// lal includes
#include <lal/iterators/E_iterator.hpp>

typedef uint64_t bigint;

namespace tests {
namespace properties {

inline void compute_data_gen_graphs_Q
(
	const lal::graphs::undirected_graph& g, const std::vector<lal::edge_pair>& Q,
	bigint& Qs, bigint& Kg,
	bigint& n_paths_4, bigint& n_cycles_4, bigint& graphlet,
	bigint& n_paths_5, bigint& pair_C3_L2,
	bigint& Phi_1, bigint& Phi_2,
	bigint& Lambda_1, bigint& Lambda_2
)
{
	// adjacency matrix
	std::vector<std::vector<bool> > A(g.get_num_nodes(), std::vector<bool>(g.get_num_nodes(), false));
	for (lal::iterators::E_iterator e_it(g); not e_it.end(); e_it.next()) {
		const lal::edge& e = e_it.get_edge();
		A[e.first][e.second] = true;
		A[e.second][e.first] = true;
	}

	for (const lal::edge_pair& ep : Q) {
		const lal::node s = ep.first.first;
		const lal::node t = ep.first.second;
		const lal::node u = ep.second.first;
		const lal::node v = ep.second.second;

		const bigint ks = g.get_degree(s);
		const bigint kt = g.get_degree(t);
		const bigint ku = g.get_degree(u);
		const bigint kv = g.get_degree(v);

		n_cycles_4 += A[s][v]*A[u][t] + A[s][u]*A[t][v];

		n_paths_4 += A[s][u] + A[s][v] + A[t][u] + A[t][v];

		Kg += ks + kt + ku + kv;
		Phi_2 += (ks + kt)*(ku + kv);
		Phi_1 += (ks*kt + ku*kv);

		graphlet += (A[t][u] + A[s][v])*(A[t][v] + A[s][u]);

		Lambda_2 +=
			(A[s][u] + A[s][v] + A[t][u] + A[t][v])*
			(ks + kt + ku + kv);

		Lambda_1 +=
			ks*(A[t][u] + A[t][v]) +
			kt*(A[s][u] + A[s][v]) +
			ku*(A[s][v] + A[t][v]) +
			kv*(A[s][u] + A[t][u]);

		const lal::neighbourhood& ns = g.get_neighbours(s);
		for (const lal::node& ws : ns) {
			if (ws == t or ws == u or ws == v) { continue; }
			n_paths_5 += A[u][ws] + A[v][ws];
			pair_C3_L2 += A[t][ws];
		}
		const lal::neighbourhood& nt = g.get_neighbours(t);
		for (const lal::node& wt : nt) {
			if (wt == s or wt == u or wt == v) { continue; }
			n_paths_5 += A[u][wt] + A[v][wt];
		}
		const lal::neighbourhood& nu = g.get_neighbours(u);
		for (const lal::node& wu : nu) {
			if (wu == s or wu == t or wu == v) { continue; }
			pair_C3_L2 += A[v][wu];
		}
	}

	Qs = static_cast<uint64_t>(Q.size());
	n_cycles_4 /= 2;
}

lal::numeric::rational nonLAL_var_num_crossings_rational_Q(const lal::graphs::undirected_graph& g, const std::vector<lal::edge_pair>& Q) {
	const bigint m = g.get_num_edges();

	// ----------------------------
	// compute terms dependent of Q

	// size of set Q
	bigint Qs = 0;

	// n_G(L_4)
	bigint n_paths_4 = 0;
	// n_G(L_5)
	bigint n_paths_5 = 0;
	// n_G(C_4)
	bigint n_cycles_4 = 0;

	// (a_{tu} + a_{sv})(a_{tv} + a_{su})
	bigint graphlet = 0;
	// the amount of pairs of disjoint
	// triangle and lal::edge in the graph.
	bigint pair_C3_L2 = 0;

	// k_s + k_t + k_u + k_v
	bigint Kg = 0;
	// (k_s*k_t + k_u*k_v)
	bigint Phi_1 = 0;
	// (k_s + k_t)(k_u + k_v)
	bigint Phi_2 = 0;

	// k_s*(a_{tu} + a_{tv}) + k_t*(a_{su} + a_{sv})
	//             + k_u*(a_{vs} + a_{vt}) + k_v*(a_{us} + a_{ut})
	bigint Lambda_1 = 0;
	// (a_{su} + a_{tu} + a_{sv} + a_{tv})*(k_s + k_t + k_u + k_v)
	bigint Lambda_2 = 0;

	compute_data_gen_graphs_Q
	(
		g, Q,
		Qs, Kg,
		n_paths_4, n_cycles_4, graphlet,
		n_paths_5, pair_C3_L2,
		Phi_1, Phi_2,
		Lambda_1, Lambda_2
	);

	lal::numeric::integer J(0);

	// V[C]
	lal::numeric::rational V(0);
	J.set_number((m + 2)*Qs);			V += lal::numeric::rational(2,45)*J;
	J.set_number((2*m + 7)*n_paths_4);	V -= lal::numeric::rational(1,180)*J;
	J.set_number(n_paths_5);			V -= lal::numeric::rational(1,180)*J;
	J.set_number(Kg);					V += lal::numeric::rational(1,90)*J;
	J.set_number(n_cycles_4);			V -= lal::numeric::rational(3,45)*J;
	J.set_number(Lambda_1);				V -= lal::numeric::rational(1,60)*J;
	J.set_number(Lambda_2);				V += lal::numeric::rational(1,180)*J;
	J.set_number(Phi_2);				V += lal::numeric::rational(1,180)*J;
	J.set_number(Phi_1);				V -= lal::numeric::rational(1,90)*J;
	J.set_number(graphlet);				V += lal::numeric::rational(1,30)*J;
	J.set_number(pair_C3_L2);			V += lal::numeric::rational(1,90)*J;
	return V;
}

} // -- namespace properties
} // -- namespace tests
