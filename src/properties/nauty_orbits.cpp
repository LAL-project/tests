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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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

#if defined DEBUG
#include <cassert>
#endif

// C++ includes
#include <vector>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/array.hpp>

// nauty includes
#include <nauty/gtools.h>
#include <nauty/nautinv.h>
#include <nauty/schreier.h>
#include <nauty/traces.h>

namespace tests {
namespace properties {
namespace nauty_lib {

std::vector<std::vector<lal::node>> compute_vertex_orbits
(const lal::graphs::free_tree& t)
noexcept
{
	graph *g = NULL;
	std::size_t g_sz = 0;

	graph *canong = NULL;
	std::size_t canong_sz = 0;

	setword *workspace = NULL;
	std::size_t workspace_sz = 0;

	int *lab = NULL;
	std::size_t lab_sz = 0;

	int *ptn = NULL;
	std::size_t ptn_sz = 0;

	int *orbits = NULL;
	std::size_t orbits_sz = 0;

	int *perm = NULL;
	std::size_t perm_sz = 0;

	set *active = NULL;
	std::size_t active_sz = 0;

	int n = static_cast<int>(t.get_num_nodes());
	int m = SETWORDSNEEDED(n);

	// allocate nauty's graph

	DYNALLOC1(int,lab,lab_sz,n,"dreadnaut");
	DYNALLOC1(int,ptn,ptn_sz,n,"dreadnaut");
	DYNALLOC1(int,orbits,orbits_sz,n,"dreadnaut");
	DYNALLOC1(int,perm,perm_sz,n,"dreadnaut");
	DYNALLOC1(set,active,active_sz,m,"dreadnaut");

	// initialise nauty's graph

	DYNALLOC2(graph,g,g_sz,n,m,"dreadnaut");
	int v;
	graph *gv;
	for (v = 0, gv = g; v < n; ++v, gv += m) {
		EMPTYSET(gv,m);
	}

	// construct graph

	for (lal::node u = 0; u < t.get_num_nodes(); ++u) {
		for (lal::node k : t.get_neighbors(u)) {
			ADDELEMENT(GRAPHROW(g,u,m), k);
		}
	}

	const int worksize = 60;

	// this macro declares a variable
	DEFAULTOPTIONS_GRAPH(options);

	const int options_schreier = 10;

	options.defaultptn = TRUE;

	options.schreier = (options_schreier > 0);
	options.invarproc = NULL;
	options.maxinvarlevel = 0;
	schreier_fails(options_schreier);

	options.usernodeproc = NULL;
	options.userautomproc = NULL;
	options.userlevelproc = NULL;
	options.userrefproc = NULL;
	options.usercanonproc = NULL;

	if (options.getcanon) {
		DYNALLOC2(graph,canong,canong_sz,n,m,"dreadnaut");
	}
	DYNALLOC1(setword,workspace,workspace_sz,2*m*worksize,"dreadnaut");

	options.writeautoms = FALSE;
	options.writemarkers = FALSE;

	// call nauty
	statsblk stats;
	nauty(
		g,lab,ptn,NULL,orbits,&options,&stats,workspace,
		2*m*worksize,m,n,canong
	);

	if (stats.errstatus) {
		// something went wrong
#if defined DEBUG
		assert(false);
#endif
		return {};
	}

	// initialise memory to calculate orbits
	std::vector<int> workperm(n);
	for (int i = n; --i >= 0;) {
		workperm[i] = 0;
	}

	int i, j;
	for (i = n; --i >= 0;) {
		if ((j = orbits[i]) < i) {
			workperm[i] = workperm[j];
			workperm[j] = i;
		}
	}

	// calculate orbits
	std::vector<std::vector<lal::node>> output_orbits;
	for (i = 0; i < n; ++i) {
		if (orbits[i] == i) {
			output_orbits.emplace_back();
			j = i;
			do {
				output_orbits.back().push_back(j);
				j = workperm[j];
			}
			while (j > 0);
		}
	}

	DYNFREE(g,g_sz);
	DYNFREE(lab,lab_sz);
	DYNFREE(ptn,ptn_sz);
	DYNFREE(orbits,orbits_sz);
	DYNFREE(perm,perm_sz);
	DYNFREE(active,active_sz);
	DYNFREE(workspace,workspace_sz);

	naugraph_freedyn();
	nausparse_freedyn();
	nautil_freedyn();
	nautinv_freedyn();
	naututil_freedyn();
	nauty_freedyn();
	schreier_freedyn();

	return output_orbits;
}

std::vector<std::vector<lal::node>> compute_vertex_orbits
(const lal::graphs::rooted_tree& t)
noexcept
{
	return compute_vertex_orbits(t.to_free_tree());
}

// -----------------------------------------------------------------------------

std::vector<std::vector<lal::edge>> compute_edge_orbits(
	const lal::graphs::free_tree& t,
	const std::vector<lal::edge>& edges,
	const std::vector<std::size_t>& vertex_orbits
)
noexcept
{
	lal::detail::array<char> edge_used(t.get_num_nodes() - 1, 0);
	std::vector<std::vector<lal::edge>> edge_orbits;
	edge_orbits.reserve(t.get_num_nodes() - 1);

	for (std::size_t ei = 0; ei < edges.size(); ++ei) {
		if (edge_used[ei] == 1) { continue; }
		const auto [u,v] = edges[ei];
		edge_orbits.push_back({{u,v}});

		for (std::size_t ej = ei + 1; ej < edges.size(); ++ej) {
			if (edge_used[ej] == 1) { continue; }
			const auto [x,y] = edges[ej];

			const bool same_ux = vertex_orbits[u] == vertex_orbits[x];
			const bool same_uy = vertex_orbits[u] == vertex_orbits[y];
			const bool same_vx = vertex_orbits[v] == vertex_orbits[x];
			const bool same_vy = vertex_orbits[v] == vertex_orbits[y];
			if ((same_ux or same_uy) and (same_vx or same_vy)) {
				edge_used[ej] = 1;
				edge_orbits.back().push_back({x,y});
			}
		}
	}

	return edge_orbits;
}

std::vector<std::vector<lal::edge>> compute_edge_orbits(
	const lal::graphs::rooted_tree& t,
	const std::vector<lal::edge>& edges,
	const std::vector<std::size_t>& vertex_orbits
)
noexcept
{
	return compute_edge_orbits(t.to_free_tree(), edges, vertex_orbits);
}

} // -- namespace nauty_lib
} // -- namespace properties
} // -- namespace tests
