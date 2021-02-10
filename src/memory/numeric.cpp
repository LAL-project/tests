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

// C++ includes
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/numeric/rational_output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "definitions.hpp"

#define return_if(e) if (e != err_type::no_error) { return e; }

namespace exe_tests {

#define check_0(i)															\
	if (i.is_initialized()) {												\
		cerr << ERROR << endl;												\
		cerr << "    i is initialized, but it should not be." << endl;		\
		return err_type::test_execution;									\
	}

#define check_1(i)															\
	if (not i.is_initialized()) {											\
		cerr << ERROR << endl;												\
		cerr << "    i is not initialized, but it should be." << endl;		\
		return err_type::test_execution;									\
	}

#define check_1v(i, v)														\
	check_1(i)																\
	if (i != v) {															\
		cerr << ERROR << endl;												\
		cerr << "    Value of i is not '" << v << "'." << endl;				\
		cerr << "    Value is          '" << i << "'." << endl;				\
		return err_type::test_execution;									\
	}

#define check_00(i1, i2)			\
	check_0(i1)						\
	check_0(i2)

#define check_0_1v(i1, i2, v)		\
	check_0(i1)						\
	check_1v(i2, v)

#define check_1v_0(i1, v, i2)		\
	check_1v(i1, v)					\
	check_0(i2)

#define check_1v_1(i1, v, i2)		\
	check_1v(i1, v)					\
	check_1(i2)

#define check_00_1v(i1, i2, i3, v)	\
	check_0(i1)						\
	check_0(i2)						\
	check_1v(i3, v)

#define check_1v_1v(i1, v1, i2, v2)	\
	check_1v(i1, v1)				\
	check_1v(i2, v2)

#define check_1v_1v_1v(i1, v1, i2, v2, i3, v3)	\
	check_1v(i1, v1)							\
	check_1v(i2, v2)							\
	check_1v(i3, v3)

#define check_11_eq(i1, i2)													\
	check_1(i1)																\
	check_1(i2)																\
	if (i1 != i2) {															\
		cerr << ERROR << endl;												\
		cerr << "    i1 is different from i2. They should not be." << endl;	\
		cerr << "    i1=: " << i1 << endl;									\
		cerr << "    i2=: " << i2 << endl;									\
		return err_type::test_execution;									\
	}																		\

#define check_eq(i1, i2)													\
	if (i1 != i2) {															\
		cerr << ERROR << endl;												\
		cerr << "    i1 is different from i2. They should not be." << endl;	\
		cerr << "    i1=: " << i1 << endl;									\
		cerr << "    i2=: " << i2 << endl;									\
		return err_type::test_execution;									\
	}																		\

#define test_eq(data, good)																\
	if (data != good) {																	\
		cerr << ERROR << endl;															\
		cerr << "    Rational values do not coincide." << endl;							\
		cerr << "    Expected '" << good << "', retrieved '" << data << "'." << endl;	\
		return err_type::test_execution;												\
	}

err_type test_integer_swap() {
	{
	integer i1;
	integer i2;
	check_1v_1v(i1, 0, i2, 0);
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 0);
	}
	{
	integer i1;
	integer i2;
	check_1v_1v(i1, 0, i2, 0);
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 0);
	}

	{
	integer i1 = 50;
	integer i2;
	check_1v_1v(i1, 50, i2, 0);
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 50);
	}
	{
	integer i1 = 50;
	integer i2;
	check_1v_1v(i1, 50, i2, 0);
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 50);
	}

	{
	integer i1;
	integer i2 = 50;
	check_1v_1v(i1, 0, i2, 50);
	i1.swap(i2);
	check_1v_1v(i1, 50, i2, 0);
	}
	{
	integer i1;
	integer i2 = 50;
	check_1v_1v(i1, 0, i2, 50);
	i2.swap(i1);
	check_1v_1v(i1, 50, i2, 0);
	}

	{
	integer i1 = 50;
	integer i2 = 2;
	check_1v_1v(i1, 50, i2, 2);
	i1.swap(i2);
	check_1v_1v(i1, 2, i2, 50);
	}
	{
	integer i1 = 50;
	integer i2 = 2;
	check_1v_1v(i1, 50, i2, 2);
	i2.swap(i1);
	check_1v_1v(i1, 2, i2, 50);
	}

	return err_type::no_error;
}

#define iff_value "1000000000000000000000000000000000000000000"
integer integer_from_function() {
	integer k = string(iff_value);
	return k;
}

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
	integer i1 = integer_from_function();
	check_1v(i1, string(iff_value));
	}
	{
	vector<integer> v;
	v.push_back(integer_from_function());
	check_1v(v[0], string(iff_value));
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

err_type test_memory_integer() {
	return_if(test_integer_swap())
	return_if(test_integer_move())
	return_if(test_integer_copy())

	{
	map<uint32_t, pair<integer, integer>> hash;
	hash.insert( make_pair(3, make_pair(integer("42"), integer("9999"))) );
	hash.insert( make_pair(4, make_pair(integer("40"), integer("99899"))) );

	const auto& p1 = hash[3];
	test_eq(p1.first, integer("42"))
	test_eq(p1.second, integer("9999"))

	const auto& p2 = hash[4];
	test_eq(p2.first, integer("40"))
	test_eq(p2.second, integer("99899"))
	}
	return err_type::no_error;
}

err_type test_rational_swap() {
	{
	rational i1;
	rational i2;
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 0);
	}
	{
	rational i1;
	rational i2;
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 0);
	}

	{
	rational i1 = 50;
	rational i2;
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 50);
	}
	{
	rational i1 = 50;
	rational i2;
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 50);
	}

	{
	rational i1;
	rational i2 = 50;
	i1.swap(i2);
	check_1v_1v(i1, 50, i2, 0);
	}
	{
	rational i1;
	rational i2 = 50;
	i2.swap(i1);
	check_1v_1v(i1, 50, i2, 0);
	}

	{
	rational i1 = 50;
	rational i2 = 2;
	i1.swap(i2);
	check_1v_1v(i1, 2, i2, 50);
	}
	{
	rational i1 = 50;
	rational i2 = 2;
	i2.swap(i1);
	check_1v_1v(i1, 2, i2, 50);
	}

	return err_type::no_error;
}

#define rff_value "100000000000000000000000000000/89898988293482848912384898234892839299219919111"
rational rational_from_function() {
	rational r = string(iff_value);
	return r;
}

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

err_type test_memory_rational() {
	return_if(test_rational_swap())
	return_if(test_rational_move())
	return_if(test_rational_copy())

	{
	map<uint32_t, pair<rational, rational>> hash;
	hash.insert( make_pair(3, make_pair(rational("42/13"), rational("9999"))) );
	hash.insert( make_pair(4, make_pair(rational("40/13"), rational("99899"))) );

	const auto& p1 = hash[3];
	test_eq(p1.first, rational("42/13"))
	test_eq(p1.second, rational("9999"))

	const auto& p2 = hash[4];
	test_eq(p2.first, rational("40/13"))
	test_eq(p2.second, rational("99899"))
	}
	return err_type::no_error;
}

err_type exe_memory_numeric(const input_list& inputs, ifstream&) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}
	return_if(test_memory_integer())
	return_if(test_memory_rational())
	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
