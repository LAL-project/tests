/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2022
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/numeric/rational_output.hpp>

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/numeric_functions.hpp"
#include "memory/numeric.hpp"

#define return_if(e) if (e != err_type::no_error) { return e; }

namespace tests {
namespace memory {

err_type test_memory_integer() noexcept {
	return_if(test_integer_swap())
	return_if(test_integer_move())
	return_if(test_integer_copy())

	{
	std::map<uint64_t, std::pair<lal::numeric::integer, lal::numeric::integer>> hash;
	hash.insert( {3, {lal::numeric::integer("42"), lal::numeric::integer("9999")}} );
	hash.insert( {4, {lal::numeric::integer("40"), lal::numeric::integer("99899")}} );

	const auto& p1 = hash[3];
	test_eq(p1.first, lal::numeric::integer("42"))
	test_eq(p1.second, lal::numeric::integer("9999"))

	const auto& p2 = hash[4];
	test_eq(p2.first, lal::numeric::integer("40"))
	test_eq(p2.second, lal::numeric::integer("99899"))
	}
	return err_type::no_error;
}

err_type test_memory_rational() noexcept {
	return_if(test_rational_swap())
	return_if(test_rational_move())
	return_if(test_rational_copy())

	{
	std::map<uint64_t, std::pair<lal::numeric::rational, lal::numeric::rational>> hash;
	hash.insert( {3, {lal::numeric::rational("42/13"), lal::numeric::rational("9999")}} );
	hash.insert( {4, {lal::numeric::rational("40/13"), lal::numeric::rational("99899")}} );

	const auto& p1 = hash[3];
	test_eq(p1.first, lal::numeric::rational("42/13"))
	test_eq(p1.second, lal::numeric::rational("9999"))

	const auto& p2 = hash[4];
	test_eq(p2.first, lal::numeric::rational("40/13"))
	test_eq(p2.second, lal::numeric::rational("99899"))
	}
	return err_type::no_error;
}

err_type exe_memory_numeric(std::ifstream&) noexcept {

	return_if(test_memory_integer());
	return_if(test_memory_rational());
	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
