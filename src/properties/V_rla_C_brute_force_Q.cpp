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

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/iterators/E_iterator.hpp>
using namespace lal;
using namespace iterators;
using namespace numeric;
using namespace graphs;

typedef uint32_t bigint;

namespace tests {
namespace properties {

inline void compute_data_gen_graphs_Q
(
	const undirected_graph& g, const vector<edge_pair>& Q,
	bigint& Qs, bigint& Kg,
	bigint& n_paths_4, bigint& n_cycles_4, bigint& graphlet,
	bigint& n_paths_5, bigint& pair_C3_L2,
	bigint& Phi_1, bigint& Phi_2,
	bigint& Lambda_1, bigint& Lambda_2
)
{
	// adjacency matrix
	vector<vector<bool> > A(g.get_num_nodes(), vector<bool>(g.get_num_nodes(), false));
	E_iterator it(g);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		A[e.first][e.second] = true;
		A[e.second][e.first] = true;
	}

	for (const edge_pair& ep : Q) {
		const node s = ep.first.first;
		const node t = ep.first.second;
		const node u = ep.second.first;
		const node v = ep.second.second;

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

		const neighbourhood& ns = g.get_neighbours(s);
		for (const node& ws : ns) {
			if (ws == t or ws == u or ws == v) { continue; }
			n_paths_5 += A[u][ws] + A[v][ws];
			pair_C3_L2 += A[t][ws];
		}
		const neighbourhood& nt = g.get_neighbours(t);
		for (const node& wt : nt) {
			if (wt == s or wt == u or wt == v) { continue; }
			n_paths_5 += A[u][wt] + A[v][wt];
		}
		const neighbourhood& nu = g.get_neighbours(u);
		for (const node& wu : nu) {
			if (wu == s or wu == t or wu == v) { continue; }
			pair_C3_L2 += A[v][wu];
		}
	}

	Qs = static_cast<uint32_t>(Q.size());
	n_cycles_4 /= 2;
}

rational variance_C_rational_Q(const undirected_graph& g, const vector<edge_pair>& Q) {
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
	// triangle and edge in the graph.
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

	integer J(0);

	// V[C]
	rational V(0);
	J.set_ui((m + 2)*Qs);			V += rational(2,45)*J;
	J.set_ui((2*m + 7)*n_paths_4);	V -= rational(1,180)*J;
	J.set_ui(n_paths_5);			V -= rational(1,180)*J;
	J.set_ui(Kg);					V += rational(1,90)*J;
	J.set_ui(n_cycles_4);			V -= rational(3,45)*J;
	J.set_ui(Lambda_1);				V -= rational(1,60)*J;
	J.set_ui(Lambda_2);				V += rational(1,180)*J;
	J.set_ui(Phi_2);				V += rational(1,180)*J;
	J.set_ui(Phi_1);				V -= rational(1,90)*J;
	J.set_ui(graphlet);				V += rational(1,30)*J;
	J.set_ui(pair_C3_L2);			V += rational(1,90)*J;
	return V;
}

} // -- namespace properties
} // -- namespace tests
