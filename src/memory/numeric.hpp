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

#pragma once

// C++ includes
#include <iostream>
#include <string>

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>

// custom includes
#include "definitions.hpp"

#define check_0(i)													\
if (i.is_initialized()) {											\
	cerr << ERROR << endl;											\
	cerr << "    i is initialized, but it should not be." << endl;	\
	return err_type::test_execution;								\
}

#define check_1(i)													\
if (not i.is_initialized()) {										\
	cerr << ERROR << endl;											\
	cerr << "    i is not initialized, but it should be." << endl;	\
	return err_type::test_execution;								\
}

#define check_1v(i, v)										\
check_1(i);													\
if (i != v) {												\
	cerr << ERROR << endl;									\
	cerr << "    Value of i is not '" << v << "'." << endl;	\
	cerr << "    Value is          '" << i << "'." << endl;	\
	return err_type::test_execution;						\
}

#define check_00(i1, i2)	\
check_0(i1);				\
check_0(i2);

#define check_0_1v(i1, i2, v)	\
check_0(i1);					\
check_1v(i2, v);

#define check_1v_0(i1, v, i2)	\
check_1v(i1, v);				\
check_0(i2);

#define check_1v_1(i1, v, i2)	\
check_1v(i1, v);				\
check_1(i2);

#define check_00_1v(i1, i2, i3, v)	\
	check_0(i1);					\
	check_0(i2);					\
	check_1v(i3, v);

#define check_1v_1v(i1, v1, i2, v2)	\
	check_1v(i1, v1);				\
	check_1v(i2, v2);

#define check_1v_1v_1v(i1, v1, i2, v2, i3, v3)	\
	check_1v(i1, v1);							\
	check_1v(i2, v2);							\
	check_1v(i3, v3);

#define check_11_eq(i1, i2)												\
check_1(i1);															\
check_1(i2);															\
if (i1 != i2) {															\
	cerr << ERROR << endl;												\
	cerr << "    i1 is different from i2. They should not be." << endl;	\
	cerr << "    i1=: " << i1 << endl;									\
	cerr << "    i2=: " << i2 << endl;									\
	return err_type::test_execution;									\
}																		\

#define check_eq(i1, i2)												\
if (i1 != i2) {															\
	cerr << ERROR << endl;												\
	cerr << "    i1 is different from i2. They should not be." << endl;	\
	cerr << "    i1=: " << i1 << endl;									\
	cerr << "    i2=: " << i2 << endl;									\
	return err_type::test_execution;									\
}																		\

#define test_eq(data, good)															\
if (data != good) {																	\
	cerr << ERROR << endl;															\
	cerr << "    Rational values do not coincide." << endl;							\
	cerr << "    Expected '" << good << "', retrieved '" << data << "'." << endl;	\
	return err_type::test_execution;												\
}

#define iff_value "1000000000000000000000000000000000000000000"
#define rff_value "100000000000000000000000000000/89898988293482848912384898234892839299219919111"

namespace exe_tests {

inline lal::numeric::integer integer_from_function() {
	return lal::numeric::integer(std::string(iff_value));
}

inline lal::numeric::rational rational_from_function() {
	return lal::numeric::rational(std::string(iff_value));
}

err_type test_integer_copy();
err_type test_integer_swap();
err_type test_integer_move();

err_type test_rational_copy();
err_type test_rational_swap();
err_type test_rational_move();

} // -- namespace exe_tests
