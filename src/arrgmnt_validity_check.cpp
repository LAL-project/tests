/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019-2020
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

#include "arrgmnt_validity_check.hpp"

// C++ includes
#include <set>
using namespace std;

// lal includes
#include <lal/iterators/E_iterator.hpp>
#include <lal/linarr/C.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/internal/std_utils.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace iterators;

namespace exe_tests {

// ----------------------------------------------------------------------------

bool is_arrangement(const linear_arrangement& arr) {
	const uint32_t n = static_cast<uint32_t>(arr.size());
	bool all_lt_n = true;
	for (position p : arr) {
		all_lt_n = (p >= n ? false : all_lt_n);
	}

	set<position> setpos;
	for (size_t i = 0; i < arr.size(); ++i) {
		setpos.insert(arr[i]);
	}
	return setpos.size() == arr.size() and all_lt_n;
}

// ----------------------------------------------------------------------------

bool is_root_covered(const rtree& rT, const linear_arrangement& pi) {
	const node R = rT.get_root();
	bool covered = false;

	E_iterator e_it(rT);
	while (e_it.has_next() and not covered) {
		e_it.next();
		const edge e = e_it.get_edge();
		const node s = e.first;
		const node t = e.second;
		covered =
			((pi[s] < pi[R]) and (pi[R] < pi[t])) or
			((pi[t] < pi[R]) and (pi[R] < pi[s]));
	}

	return covered;
}

// ----------------------------------------------------------------------------

template<class T>
bool is_linarr_planar(
	const T& t, const linear_arrangement& arr
)
{
	return lal::linarr::n_crossings(t, arr) == 0;
}

bool is_linarr_projective(
	const rtree& rT, const linear_arrangement& arr
)
{
	return is_linarr_planar(rT, arr) and not is_root_covered(rT, arr);
}

// ----------------------------------------------------------------------------

string is_arrangement_planar(
	const ftree& fT, const linear_arrangement& arr
)
{
	if (not is_arrangement(arr)) {
		return "The arrangement is not a permutation of the vertices.";
	}
	if (not is_linarr_planar(fT, arr)) {
		return "The arrangement is not planar";
	}
	return "No error";
}

string is_arrangement_projective(
	const rtree& rT, const linear_arrangement& arr
)
{
	if (not is_arrangement(arr)) {
		return "The arrangement is not a permutation of the vertices.";
	}
	if (not is_linarr_projective(rT, arr)) {
		return "The arrangement is not projective";
	}
	return "No error";
}

} // -- namespace exe_tests
