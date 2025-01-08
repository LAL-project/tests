/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
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

#include "memory/numeric.hpp"

// C++ includes
#include <iostream>
#include <vector>

// lal includes
#include <lal/numeric/rational_output.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace memory {

err_type test_rational_copy() noexcept {
	// copy constructor
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = i1;
	check_eq(i1, i2);
	}
	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2 = i1;
	check_eq(i2, i1);
	}
	{
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(i1);
	check_eq(i1, v[0]);
	}
	{
	std::vector<lal::numeric::rational> v;
	lal::numeric::integer i1 = 1234;
	v.push_back(i1);
	check_eq(v[0], i1);
	}
	{
	lal::numeric::integer i1 = 12;
	lal::numeric::integer i2 = 23;
	lal::numeric::rational r(i1,i2);
	check_1v_1v_1v(i1,12, i2,23, r, lal::numeric::rational("12/23"));
	}

	// copy operator
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = 200;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2;
	i2 = i1;
	check_eq(i2, i1);
	}
	{
	lal::numeric::integer i1 = 50;
	lal::numeric::rational i2 = 200;
	i2 = i1;
	check_eq(i2, i1);
	}

	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational());
	check_1v(v[0], 0);
	lal::numeric::integer i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational());
	check_1v(v[0], 0);
	lal::numeric::rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}

	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::integer());
	check_1v(v[0], 0);
	lal::numeric::rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(i1, v[0]);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::integer(5678));
	check_1v(v[0], 5678);
	lal::numeric::rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational(5678));
	check_1v(v[0], 5678);
	lal::numeric::rational i1 = 1234;
	check_1v(i1, 1234);
	v[0] = i1;
	check_eq(v[0], i1);
	}
	{
	std::vector<lal::numeric::rational> v;
	v.push_back(lal::numeric::rational(5678));
	check_1v(v[0], 5678);
	lal::numeric::rational i1 = 1234;
	check_1v(i1, 1234);
	i1 = v[0];
	check_eq(v[0], i1);
	}

	{
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(i1);
	check_eq(v[0], i1);
	}
	{
	lal::numeric::integer i1;
	std::vector<lal::numeric::rational> v;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	{
	lal::numeric::rational i1;
	std::vector<lal::numeric::rational> v;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	{
	lal::numeric::rational i2 = 4567;
	lal::numeric::rational i1 = 1234;
	std::vector<lal::numeric::rational> v;
	v.push_back(i1);
	v.push_back(i2);
	check_eq(v[0], i1);
	check_eq(v[1], i2);
	}
	{
	std::vector<lal::numeric::rational> v;
	lal::numeric::rational i1;
	v.push_back(i1);
	check_1v_1v(v[0], 0, i1, 0);
	}
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
