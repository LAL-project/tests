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

err_type test_rational_move() {
	// move constructor
	{
	rational i1 = 50;
	rational i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}
	{
	integer i1 = 50;
	rational i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}
	{
	integer i1 = 50;
	rational i2(std::move(i1));
	check_0_1v(i1, i2, 50);
	}
	{
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<rational> v;
	for (size_t i = 0; i < 1000; ++i) {
		rational r1 = 1234;
		v.push_back(std::move(r1));
		check_0_1v(r1, v[i], 1234);
	}
	}
	{
	vector<rational> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<rational> v;
	for (size_t i = 0; i < 1000; ++i) {
		integer i1 = 1234;
		v.push_back(std::move(i1));
		check_0_1v(i1, v[i], 1234);
	}
	}
	{
	integer i1 = 12;
	integer i2 = 23;
	rational r(std::move(i1),std::move(i2));
	check_00_1v(i1, i2, r, rational("12/23"));
	}
	{
	rational r1 = integer_from_function();
	check_1v(r1, string(iff_value));
	}
	{
	vector<rational> v;
	for (size_t i = 0; i < 1000; ++i) {
		v.push_back(rational_from_function());
		check_1v(v[i], string(iff_value));
	}
	}
	{
	vector<rational> v;
	for (size_t i = 0; i < 1000; ++i) {
		v.push_back(string(iff_value));
		check_1v(v[i], string(iff_value));
	}
	}

	// move operator
	{
	rational i1 = 50;
	rational i2;
	check_1v_1(i1, 50, i2);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	rational i1 = 50;
	rational i2 = 200;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	rational i1 = 50;
	rational i2 = 200;
	check_1v_1v(i1, 50, i2, 200);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = 3;
	check_1v_1v(i1, 3, i2, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 3);
	}

	{
	integer i1 = 50;
	rational i2;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	integer i1 = 50;
	rational i2 = 200;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	vector<rational> v;
	v.push_back(rational());
	check_1v(v[0], 0);
	integer i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<rational> v;
	v.push_back(rational());
	check_1v(v[0], 0);
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	vector<rational> v;
	v.push_back(integer());
	check_1v(v[0], 0);
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<rational> v;
	v.push_back(integer(5678));
	check_1v(v[0], 5678)
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<rational> v;
	v.push_back(rational(5678));
	check_1v(v[0], 5678);
	rational i1 = 1234;
	check_1v_1v(i1, 1234, v[0], 5678);
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	integer i1 = 1234;
	vector<rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	integer i1;
	vector<rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 0);
	}

	{
	rational i1 = 1234;
	vector<rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	i1 = std::move(v[0]);
	check_1v_0(i1, 1234, v[0]);
	}
	{
	vector<rational> v;
	rational i1;
	v.push_back(std::move(i1));
	i1 = std::move(v[0]);
	check_1v_0(i1, 0, v[0]);
	}
	return err_type::no_error;
}

} // -- namespace exe_tests
