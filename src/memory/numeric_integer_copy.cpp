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

#include "memory/numeric.hpp"

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/numeric/integer_output.hpp>
using namespace lal;
using namespace numeric;

namespace exe_tests {

err_type test_integer_copy() {
	// copy constructor
	{
	integer i1 = 50;
	integer i2 = i1;
	check_eq(i1, i2);
	}
	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(i1);
	check_eq(i1, v[0]);
	}
	{
	integer i = 1234;
	vector<integer> v;
	v.emplace_back(i);
	check_1v(v[0], 1234);
	}
	{
	integer i = 1234;
	vector<integer> v;
	v.push_back(i);
	check_1v(v[0], 1234);
	}

	// copy operator
	{
	integer i1 = 50;
	integer i2;
	i2 = i1;
	check_eq(i1, i2);
	}

	{
	integer i1 = 50;
	integer i2 = 200;
	i2 = i1;
	check_eq(i1, i2);
	}

	{
	vector<integer> v;
	v.push_back(integer());
	integer i1 = 1234;
	v[0] = i1;
	check_eq(i1, v[0]);
	}

	{
	vector<integer> v;
	v.push_back(integer(5678));
	integer i1 = 1234;
	v[0] = i1;
	check_eq(i1, v[0]);
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(i1);
	check_eq(i1, v[0]);
	}
	{
	vector<integer> v;
	integer i1;
	v.push_back(i1);
	check_1v_1v(i1, 0, v[0], 0);
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(i1);
	check_eq(v[0], i1);
	}

	return err_type::no_error;
}

} // -- namespace exe_tests
