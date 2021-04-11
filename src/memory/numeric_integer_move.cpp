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
#include <iostream>
#include <vector>
using namespace std;

// lal includes
#include <lal/numeric/integer_output.hpp>
#include <lal/numeric/rational.hpp>
using namespace lal;
using namespace numeric;

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/numeric_functions.hpp"

namespace tests {
namespace memory {

err_type test_integer_move() {
	// move constructor
	{
	integer i1 = 50;
	integer i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}
	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<integer> v;
	for (size_t i = 0; i < 1000; ++i) {
		integer i1 = 1234;
		v.push_back(std::move(i1));
		check_0_1v(i1, v[i], 1234);
	}
	}
	{
	rational k = string("23/45");
	integer i = k.to_integer();
	check_1v(i, 0);
	k = string("109/45");
	i = k.to_integer();
	check_1v(i, 2);
	}
	{
	integer i1 = integer_from_function();
	check_1v(i1, string(iff_value));
	}
	{
	vector<integer> v;
	for (size_t i = 0; i < 1000; ++i) {
		v.push_back(integer_from_function());
		check_1v(v[i], string(iff_value));
	}
	}
	{
	vector<integer> v;
	for (size_t i = 0; i < 1000; ++i) {
		v.push_back(string(iff_value));
		check_1v(v[i], string(iff_value));
	}
	}
	{
	integer i = 1234;
	vector<integer> v;
	v.emplace_back(std::move(i));
	check_1v(v[0], 1234);
	}
	{
	vector<integer> v;
	v.emplace_back(integer_from_function());
	check_1v(v[0], string(iff_value));
	}
	{
	vector<integer> v;
	v.push_back(integer_from_function());
	check_1v(v[0], string(iff_value));
	}

	// move operator
	{
	integer i1 = 50;
	integer i2;
	check_1v_1v(i1, 50, i2, 0);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	integer i1 = 50;
	integer i2 = 200;
	check_1v_1v(i1, 50, i2, 200);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	integer i1 = 50;
	integer i2 = 200;
	check_1v_1v(i1, 50, i2, 200);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = 3;
	check_1v_1v(i1, 3, i2, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 3);
	}

	{
	vector<integer> v;
	v.push_back(integer());
	integer i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	vector<integer> v;
	v.push_back(integer(5678));
	integer i1 = 1234;
	check_1v_1v(i1, 1234, v[0], 5678);
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	vector<integer> v;
	integer i1;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 0);
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	i1 = std::move(v[0]);
	check_1v_0(i1, 1234, v[0]);
	}

	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
