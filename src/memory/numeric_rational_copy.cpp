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

#include "memory/numeric.hpp"

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/numeric/rational_output.hpp>
using namespace lal;
using namespace numeric;

namespace exe_tests {

err_type test_rational_copy() {
	// copy constructor
	{
	rational i1 = 50;
	rational i2 = i1;
	check_eq(i1, i2);
	}
	{
	integer i1 = 50;
	rational i2 = i1;
	check_eq(i2, i1);
	}
	{
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(i1);
	check_eq(i1, v[0]);
	}
	{
	vector<rational> v;
	integer i1 = 1234;
	v.push_back(i1);
	check_eq(v[0], i1);
	}
	{
	integer i1 = 12;
	integer i2 = 23;
	rational r(i1,i2);
	check_1v_1v_1v(i1,12, i2,23, r,rational("12/23"));
	}

	// copy operator
	{
	rational i1 = 50;
	rational i2;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	rational i1 = 50;
	rational i2 = 200;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	integer i1 = 50;
	rational i2;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	integer i1 = 50;
	rational i2 = 200;
	i2 = i1;
	check_eq(i2, i1);
	}

	{
	vector<rational> v;
	v.push_back(rational());
	check_1v(v[0], 0);
	integer i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	vector<rational> v;
	v.push_back(rational());
	check_1v(v[0], 0);
	rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1)
	}

	{
	vector<rational> v;
	v.push_back(integer());
	check_1v(v[0], 0);
	rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(i1, v[0]);
	}
	{
	vector<rational> v;
	v.push_back(integer(5678));
	check_1v(v[0], 5678);
	rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	vector<rational> v;
	v.push_back(rational(5678));
	check_1v(v[0], 5678);
	rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	vector<rational> v;
	v.push_back(rational(5678));
	check_1v(v[0], 5678);
	rational i1 = 1234;
	check_1v(i1, 1234);
	i1 = v[0];
	check_eq(v[0], i1);
	}

	{
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(i1);
	check_eq(v[0], i1);
	}
	{
	integer i1;
	vector<rational> v;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	{
	rational i1;
	vector<rational> v;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	{
	rational i2 = 4567;
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(i1);
	v.push_back(i2);
	check_eq(v[0], i1);
	check_eq(v[1], i2);
	}
	{
	vector<rational> v;
	rational i1;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	return err_type::no_error;
}

} // -- namespace exe_tests
