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

#pragma once

// C++ includes
#include <ostream>
#include <vector>

template <typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) noexcept {
	if (v.size() == 0) { return os; }
	os << v[0];
	for (std::size_t i = 1ull; i < v.size(); ++i) {
		os << ", " << v[i];
	}
	return os;
}

template <typename T>
std::istream& operator>> (std::istream& is, const std::vector<T>& v) noexcept {
	for (std::size_t i = 0ull; i < v.size(); ++i) {
		is >> v[i];
	}
	return is;
}

template <typename T, typename U>
std::ostream& operator<< (std::ostream& os, const std::pair<T,U>& p) noexcept {
	os << "(" << p.first << "," << p.second << ")";
	return os;
}

// lal includes
#include <lal/linear_arrangement.hpp>

inline
std::ostream& operator<< (std::ostream& os, const lal::linear_arrangement& arr)
noexcept
{
	if (arr.size() > 0) {
		os << "(" << arr[lal::node_t{0ull}];
		for (lal::node_t u = 1ull; u < arr.size(); ++u) {
			os << ", " << arr[u];
		}
		os << ") (" << arr[lal::position_t{0ull}];
		for (lal::position_t p = 1ull; p < arr.size(); ++p) {
			os << ", " << arr[p];
		}
		os << ')';
	}
	return os;
}
