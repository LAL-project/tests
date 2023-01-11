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
#include <vector>
#include <set>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace tests {
namespace graphs {

std::vector<lal::edge> enumerate_edges_brute_force(const lal::graphs::directed_graph& g)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::set<lal::edge> E;
	for (lal::node s = 0; s < n; ++s) {
	for (auto t : g.get_out_neighbours(s)) {
		lal::edge st(s,t);
		if (g.is_undirected()) {
			if (s > t) { std::swap(st.first, st.second); }
		}
		E.insert(st);
	}}
	return std::vector<lal::edge>(E.begin(), E.end());
}

std::vector<lal::edge> enumerate_edges_brute_force(const lal::graphs::undirected_graph& g)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::set<lal::edge> E;
	for (lal::node s = 0; s < n; ++s) {
	for (auto t : g.get_neighbours(s)) {
		lal::edge st(s,t);
		if (g.is_undirected()) {
			if (s > t) { std::swap(st.first, st.second); }
		}
		E.insert(st);
	}}
	return std::vector<lal::edge>(E.begin(), E.end());
}

bool share_vertices(const lal::edge_pair& st_uv) noexcept {
	const lal::edge& st = st_uv.first;
	const lal::edge& uv = st_uv.second;
	const lal::node s = st.first;
	const lal::node t = st.second;
	const lal::node u = uv.first;
	const lal::node v = uv.second;
	return s == u or s == v or t == u or t == v;
}

std::vector<lal::edge_pair> enumerate_Q_brute_force(const lal::graphs::undirected_graph& g)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::set<lal::edge_pair> Q;
	for (lal::node s = 0; s < n; ++s) {
	for (lal::node t : g.get_neighbours(s)) {

		for (lal::node u = s + 1; u < n; ++u) {
		for (lal::node v : g.get_neighbours(u)) {

			// s != u and t != u
			if (s == v or s == u) { continue; }
			if (t == v or t == u) { continue; }

			// an undirected lal::edge should not be sorted
			lal::edge st(s,t);
			lal::edge uv(u,v);
			if (g.is_undirected()) {
				if (s > t) { std::swap(st.first, st.second); }
				if (u > v) { std::swap(uv.first, uv.second); }
			}
			// the pair should be sorted
			if (st > uv) { std::swap(st, uv); }

			// no common endpoints
			Q.insert( lal::edge_pair(st, uv) );
		}}
	}}
	return std::vector<lal::edge_pair>(Q.begin(), Q.end());
}

std::vector<lal::edge_pair> enumerate_Q_brute_force(const lal::graphs::directed_graph& g)
noexcept
{
	const uint64_t n = g.get_num_nodes();
	std::set<lal::edge_pair> Q;
	for (lal::node s = 0; s < n; ++s) {
	for (lal::node t : g.get_out_neighbours(s)) {

		for (lal::node u = s + 1; u < n; ++u) {
		for (lal::node v : g.get_out_neighbours(u)) {

			// s != u and t != u
			if (s == v or s == u) { continue; }
			if (t == v or t == u) { continue; }

			// an undirected lal::edge should not be sorted
			lal::edge st(s,t);
			lal::edge uv(u,v);
			if (g.is_undirected()) {
				if (s > t) { std::swap(st.first, st.second); }
				if (u > v) { std::swap(uv.first, uv.second); }
			}
			// the pair should be sorted
			if (st > uv) { std::swap(st, uv); }

			// no common endpoints
			Q.insert( lal::edge_pair(st, uv) );
		}}
	}}
	return std::vector<lal::edge_pair>(Q.begin(), Q.end());
}

} // -- namespace graphs
} // -- namespace tests
