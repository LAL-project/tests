/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
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

// C++ includes
#include <iostream>
#include <fstream>
#include <map>

// lal includes
#include <lal/detail/data_array.hpp>
using namespace lal::detail;

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/data_array_utils.hpp"

#define MOVE
#define COPY

// LINE LEFT BLANK INTENTIONALLY
namespace tests {
namespace memory {

namespace lal_data_array {

#if defined MOVE
void test_move_constructor() noexcept {
	begin_function;

	{
	begin_case;
	data_array<int> A(0);
	output_array(A);
	data_array<int> B(std::move(A));
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(0, 333);
	output_array(A);
	data_array<int> B(std::move(A));
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(5, 100);
	output_array(A);
	data_array<int> B(std::move(A));
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(10, 1);
	output_array(A);
	data_array<int> B(std::move(A));
	output_array(A);
	output_array(B);
	}
}

void test_move_assignment() noexcept {
	begin_function;
	data_array<int> TO(0);

	{
	begin_case;
	data_array<int> A(0);
	output_array(A);
	TO = std::move(A);
	output_array(A);
	output_array(TO);

	A = std::move(TO);
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(0, 333);
	output_array(A);
	TO = std::move(A);
	output_array(A);
	output_array(TO);

	A = std::move(TO);
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(5, 100);
	output_array(A);
	TO = std::move(A);
	output_array(A);
	output_array(TO);

	A = std::move(TO);
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(10, 1);
	output_array(A);
	TO = std::move(A);
	output_array(A);
	output_array(TO);

	A = std::move(TO);
	output_array(A);
	output_array(TO);
	}
}
#endif

#if defined COPY
void test_copy_constructor() noexcept {
	begin_function;

	{
	begin_case;
	data_array<int> A(0);
	output_array(A);
	data_array<int> B(A);
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(0, 333);
	output_array(A);
	data_array<int> B(A);
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(5, 100);
	output_array(A);
	data_array<int> B(A);
	output_array(A);
	output_array(B);
	}

	{
	begin_case;
	data_array<int> A(10, 1);
	output_array(A);
	data_array<int> B(A);
	output_array(A);
	output_array(B);
	}
}

class class_with_array {
public:
	void init() {
		A.resize(10, 5);
	}

	data_array<int> A;
};

void test_copy_assignment() noexcept {
	begin_function;
	data_array<int> TO(0);

	{
	begin_case;
	data_array<int> A(0);
	output_array(A);
	TO = A;
	output_array(A);
	output_array(TO);

	A = TO;
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(0, 333);
	output_array(A);
	TO = A;
	output_array(A);
	output_array(TO);

	A = TO;
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(5, 100);
	output_array(A);
	TO = A;
	output_array(A);
	output_array(TO);

	A = TO;
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A(10, 1);
	output_array(A);
	TO = A;
	output_array(A);
	output_array(TO);

	A = TO;
	output_array(A);
	output_array(TO);
	}

	{
	begin_case;
	data_array<int> A;
	output_array(A);
	A = data_array<int>(10, 5);
	output_array(A);
	}

	{
	begin_case;
	data_array<int> A;
	output_array(A);
	A.resize(10, 5);
	output_array(A);
	}

	{
	begin_case;
	class_with_array cwa;
	output_array(cwa.A);
	cwa.init();
	output_array(cwa.A);
	}
}
#endif

template <bool clear>
void test_resize_fill() noexcept {
	begin_function;

	data_array<int> d;

	begin_case;
	d.resize(1, 1);
	output_array(d);

	if constexpr (clear) { d.clear(); }

	begin_case;
	d.resize(2, 2);
	output_array(d);

	if constexpr (clear) { d.clear(); }

	begin_case;
	d.resize(10, 10);
	output_array(d);

	if constexpr (clear) { d.clear(); }

	begin_case;
	d.resize(10, 10);
	output_array(d);

	if constexpr (clear) { d.clear(); }

	begin_case;
	d.resize(20);
	d.fill(20);
	output_array(d);

	if constexpr (clear) { d.clear(); }
}

}

err_type exe_memory_detail_data_array(std::ifstream&) noexcept {

#if defined MOVE
	lal_data_array::test_move_constructor();
	lal_data_array::test_move_assignment();
#endif

#if defined COPY
	lal_data_array::test_copy_constructor();
	lal_data_array::test_copy_assignment();
#endif

	lal_data_array::test_resize_fill<false>();
	lal_data_array::test_resize_fill<true>();

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
