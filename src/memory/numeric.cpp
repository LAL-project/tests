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
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "definitions.hpp"

#define return_if(e) if (e != err_type::no_error) { return e; }

namespace exe_tests {

#define check_00(i1, i2)													\
	if (i1.is_initialized()) {												\
		cerr << ERROR << endl;												\
		cerr << "    i1 is initialized, but it should not be." << endl;		\
		return err_type::test_execution;									\
	}																		\
	if (i2.is_initialized()) {												\
		cerr << ERROR << endl;												\
		cerr << "    i2 is initialized, and it should not be." << endl;		\
		return err_type::test_execution;									\
	}

#define check_01(i1, i2, v)													\
	if (i1.is_initialized()) {												\
		cerr << ERROR << endl;												\
		cerr << "    i1 is initialized, but it should not be." << endl;		\
		return err_type::test_execution;									\
	}																		\
	if (not i2.is_initialized()) {											\
		cerr << ERROR << endl;												\
		cerr << "    i2 is not initialized, but it should be." << endl;		\
		return err_type::test_execution;									\
	}																		\
	if (i2 != v) {															\
		cerr << ERROR << endl;												\
		cerr << "    Value of i2 is not " << v << "." << endl;				\
		cerr << "    Value is: " << i2 << endl;								\
		return err_type::test_execution;									\
	}

#define check_11(i1, i2, v1, v2)											\
	if (not i1.is_initialized()) {											\
		cerr << ERROR << endl;												\
		cerr << "    i1 is not initialized, but it should be." << endl;		\
		return err_type::test_execution;									\
	}																		\
	if (not i2.is_initialized()) {											\
		cerr << ERROR << endl;												\
		cerr << "    i2 is not initialized, but it should be." << endl;		\
		return err_type::test_execution;									\
	}																		\
	if (i1 != v1) {															\
		cerr << ERROR << endl;												\
		cerr << "    Value of i1 is not " << v1 << "." << endl;				\
		cerr << "    Value is: " << i1 << endl;								\
		return err_type::test_execution;									\
	}																		\
	if (i2 != v2) {															\
		cerr << ERROR << endl;												\
		cerr << "    Value of i2 is not " << v2 << "." << endl;				\
		cerr << "    Value is: " << i2 << endl;								\
		return err_type::test_execution;									\
	}

#define test_eq(data, good)																\
	if (data != good) {																	\
		cerr << ERROR << endl;															\
		cerr << "    Rational values do not coincide." << endl;							\
		cerr << "    Expected '" << good << "', retrieved '" << data << "'." << endl;	\
		return err_type::test_execution;												\
	}

err_type test_imem_swap() {
	{
	integer i1;
	integer i2;
	i1.swap(i2);
	check_00(i1, i2)
	}
	{
	integer i1;
	integer i2;
	i2.swap(i1);
	check_00(i1, i2)
	}

	{
	integer i1 = 50;
	integer i2;
	i1.swap(i2);
	check_01(i1, i2, 50)
	}
	{
	integer i1 = 50;
	integer i2;
	i2.swap(i1);
	check_01(i1, i2, 50)
	}

	{
	integer i1;
	integer i2 = 50;
	i1.swap(i2);
	check_01(i2, i1, 50)
	}
	{
	integer i1;
	integer i2 = 50;
	i2.swap(i1);
	check_01(i2, i1, 50)
	}

	{
	integer i1 = 50;
	integer i2 = 2;
	i1.swap(i2);
	check_11(i1, i2, 2, 50)
	}
	{
	integer i1 = 50;
	integer i2 = 2;
	i2.swap(i1);
	check_11(i1, i2, 2, 50)
	}

	return err_type::no_error;
}

err_type test_imem_move() {
	// move constructor
	{
	integer i1 = 50;
	integer i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}

	// move operator
	{
	integer i1 = 50;
	integer i2;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}

	{
	integer i1 = 50;
	integer i2 = 200;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}

	{
	vector<integer> v;
	v.push_back(integer());
	integer i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}

	{
	vector<integer> v;
	v.push_back(integer(5678));
	integer i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}
	{
	vector<integer> v;
	integer i1;
	v.push_back(std::move(i1));
	check_00(i1, v[0])
	}

	{
	vector<integer> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	i1 = std::move(v[0]);
	check_01(v[0], i1, 1234)
	}

	return err_type::no_error;
}

err_type test_memory_integer() {
	return_if(test_imem_swap())
	return_if(test_imem_move())

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

err_type test_rmem_swap() {
	{
	rational i1;
	rational i2;
	i1.swap(i2);
	check_00(i1, i2)
	}
	{
	rational i1;
	rational i2;
	i2.swap(i1);
	check_00(i1, i2)
	}

	{
	rational i1 = 50;
	rational i2;
	i1.swap(i2);
	check_01(i1, i2, 50)
	}
	{
	rational i1 = 50;
	rational i2;
	i2.swap(i1);
	check_01(i1, i2, 50)
	}

	{
	rational i1;
	rational i2 = 50;
	i1.swap(i2);
	check_01(i2, i1, 50)
	}
	{
	rational i1;
	rational i2 = 50;
	i2.swap(i1);
	check_01(i2, i1, 50)
	}

	{
	rational i1 = 50;
	rational i2 = 2;
	i1.swap(i2);
	check_11(i1, i2, 2, 50)
	}
	{
	rational i1 = 50;
	rational i2 = 2;
	i2.swap(i1);
	check_11(i1, i2, 2, 50)
	}

	return err_type::no_error;
}

err_type test_rmem_move() {
	// move constructor
	{
	rational i1 = 50;
	rational i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	integer i1 = 50;
	rational i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	vector<rational> v;
	rational i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}
	{
	vector<rational> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}

	// move operator
	{
	rational i1 = 50;
	rational i2;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	rational i1 = 50;
	rational i2 = 200;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	integer i1 = 50;
	rational i2;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}
	{
	integer i1 = 50;
	rational i2 = 200;
	i2 = std::move(i1);
	check_01(i1, i2, 50)
	}

	{
	vector<rational> v;
	v.push_back(rational());
	integer i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}
	{
	vector<rational> v;
	v.push_back(rational());
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}

	{
	vector<rational> v;
	v.push_back(integer(5678));
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}
	{
	vector<rational> v;
	v.push_back(rational(5678));
	rational i1 = 1234;
	v[0] = std::move(i1);
	check_01(i1, v[0], 1234)
	}

	{
	vector<rational> v;
	rational i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}
	{
	vector<rational> v;
	integer i1 = 1234;
	v.push_back(std::move(i1));
	check_01(i1, v[0], 1234)
	}
	{
	vector<rational> v;
	integer i1;
	v.push_back(std::move(i1));
	check_00(i1, v[0])
	}

	{
	vector<rational> v;
	rational i1 = 1234;
	v.push_back(std::move(i1));
	i1 = std::move(v[0]);
	check_01(v[0], i1, 1234)
	}
	{
	vector<rational> v;
	rational i1;
	v.push_back(std::move(i1));
	i1 = std::move(v[0]);
	check_00(v[0], i1)
	}

	return err_type::no_error;
}

err_type test_memory_rational() {
	return_if(test_rmem_swap())
	return_if(test_rmem_move())

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
