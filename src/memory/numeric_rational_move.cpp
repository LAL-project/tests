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

#include "memory/numeric.hpp"

// C++ includes
#include <iostream>
#include <vector>

// lal includes
#include <lal/numeric/rational_output.hpp>

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/numeric_functions.hpp"

namespace tests {
namespace memory {

err_type test_rational_move() noexcept {
	// move constructor
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}
	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}
	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2(std::move(i1));
	check_0_1v(i1, i2, 50);
	}
	{
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	std::vector<lal::numeric::rational> v;
	for (std::size_t i = 0; i < 1000; ++i) {
		lal::numeric::rational r1 = 1234;
		v.push_back(std::move(r1));
		check_0_1v(r1, v[i], 1234);
	}
	}
	{
	std::vector<lal::numeric::rational> v;
	lal::numeric::integer i1 = 1234;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	std::vector<lal::numeric::rational> v;
	for (std::size_t i = 0; i < 1000; ++i) {
		lal::numeric::integer i1 = 1234;
		v.push_back(std::move(i1));
		check_0_1v(i1, v[i], 1234);
	}
	}
	{
	lal::numeric::integer i1 = 12;
	lal::numeric::integer i2 = 23;
	lal::numeric::rational r(std::move(i1),std::move(i2));
	check_00_1v(i1, i2, r, lal::numeric::rational("12/23"));
	}
	{
	lal::numeric::rational r1 = integer_from_function();
	check_1v(r1, lal::numeric::rational(std::string(iff_value)));
	}
	{
	std::vector<lal::numeric::rational> v;
	for (std::size_t i = 0; i < 1000; ++i) {
		v.push_back(rational_from_function());
		check_1v(v[i], lal::numeric::rational(std::string(iff_value)));
	}
	}
	{
	std::vector<lal::numeric::rational> v;
	for (std::size_t i = 0; i < 1000; ++i) {
		v.push_back(std::string(iff_value));
		check_1v(v[i], lal::numeric::rational(std::string(iff_value)));
	}
	}

	// move operator
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2;
	check_1v_1(i1, 50, i2);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = 200;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = std::move(i2);
	check_0_1v(i2, i1, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = 200;
	check_1v_1v(i1, 50, i2, 200);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	i1 = 3;
	check_1v_1v(i1, 3, i2, 50);
	i2 = std::move(i1);
	check_0_1v(i1, i2, 3);
	}

	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2 = 200;
	i2 = std::move(i1);
	check_0_1v(i1, i2, 50);
	}

	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational());
	check_1v(v[0], 0);
	lal::numeric::integer i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational());
	check_1v(v[0], 0);
	lal::numeric::rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::integer());
	check_1v(v[0], 0);
	lal::numeric::rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::integer(5678));
	check_1v(v[0], 5678)
	lal::numeric::rational i1 = 1234;
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational(5678));
	check_1v(v[0], 5678);
	lal::numeric::rational i1 = 1234;
	check_1v_1v(i1, 1234, v[0], 5678);
	v[0] = std::move(i1);
	check_0_1v(i1, v[0], 1234);
	}

	{
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	lal::numeric::integer i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	}
	{
	lal::numeric::integer i1;
	std::vector<lal::numeric::rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 0);
	}

	{
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(std::move(i1));
	check_0_1v(i1, v[0], 1234);
	i1 = std::move(v[0]);
	check_1v_0(i1, 1234, v[0]);
	}
	{
	std::vector<lal::numeric::rational> v;
	lal::numeric::rational i1;
	v.push_back(std::move(i1));
	i1 = std::move(v[0]);
	check_1v_0(i1, 0, v[0]);
	}
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
