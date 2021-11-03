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

// C++ includes
#include <iostream>
#include <iomanip>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/generate/rand_lab_free_trees.hpp>
#include <lal/linarr/C.hpp>
#include <lal/graphs/output.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace linarr {

#define begin_function											\
	std::cout << "+++++++++++++++++++++++++++++++++\n";			\
	std::cout << "function: " << __PRETTY_FUNCTION__ << '\n';

#define begin_case												\
	std::cout << "-----------------------------------\n";		\
	std::cout << "case starting at line: " << __LINE__ << '\n';

void shift_left(lal::linear_arrangement& arr) noexcept {
	std::cout
		<< std::setw(30) << "Original arrangement: "
		<< arr << '\n';
	for (std::size_t i = 0; i < arr.size(); ++i) {
		arr.shift_left();
		std::cout
			<< std::setw(30) << "left shift (" + std::to_string(i) + "): "
			<< arr << '\n';
	}
}

void shift_right(lal::linear_arrangement& arr) noexcept {
	std::cout
		<< std::setw(30) << "Original arrangement: "
		<< arr << '\n';
	for (std::size_t i = 0; i < arr.size(); ++i) {
		arr.shift_right();
		std::cout
			<< std::setw(30) << "right shift (" + std::to_string(i) + "): "
			<< arr << '\n';
	}
}

void case_1() noexcept {
	begin_function;

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_left(arr);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_right(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_left(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(1);
	shift_right(arr);
	shift_right(arr);
	}
}

void case_2() noexcept {
	begin_function;

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_left(arr);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_right(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_left(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(2);
	shift_right(arr);
	shift_right(arr);
	}
}

void case_3() noexcept {
	begin_function;

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_left(arr);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_right(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_left(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(3);
	shift_right(arr);
	shift_right(arr);
	}
}

void case_4() noexcept {
	begin_function;

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_left(arr);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_right(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_left(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr = lal::linear_arrangement::identity(10);
	shift_right(arr);
	shift_right(arr);
	}
}

void case_5() noexcept {
	begin_function;

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_left(arr);
	shift_right(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_right(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_left(arr);
	shift_left(arr);
	}

	{
	begin_case;
	lal::linear_arrangement arr({1,2,6,0,5,3,8,4,9,7});
	shift_right(arr);
	shift_right(arr);
	}
}

void __case_6(std::size_t n, const std::vector<std::pair<lal::node_t,lal::node_t>>& v)
noexcept
{
	lal::linear_arrangement arr(n);
	arr.identity();

	std::cout << "    " << arr << '\n';
	for (const auto& p : v) {
		std::cout << "Swapping vertices: " << p.first << " " << p.second << '\n';
		arr.swap(p.first, p.second);
		std::cout << "    " << arr << '\n';
	}
}

void case_6() noexcept {
	begin_function;

	begin_case;
	__case_6(5, {{0,1}, {0,2}, {0,3}, {0,4}});

	begin_case;
	__case_6(5, {{1,0}, {2,3}, {0,4}, {1,3}});

	begin_case;
	__case_6(5, {{1,0}, {2,3}, {0,4}, {1,3}});
}

err_type case_7() {
	begin_function;

	lal::generate::rand_lab_free_trees Gen;
	for (uint64_t n = 10; n < 210; n += 10) {
		begin_case;
		std::cout << "n= " << n << '\n';

		Gen.init(n, 1234);
		for (std::size_t i = 0; i < 100; ++i) {
			const auto t = Gen.get_tree();

			lal::linear_arrangement arr = lal::linear_arrangement::identity(n);
			const auto C = lal::linarr::num_crossings(t, arr);

			for (std::size_t j = 0; j < n; ++j) {
				arr.shift_left();
				auto C2 = lal::linarr::num_crossings(t, arr);

				if (C != C2) {
					std::cerr << ERROR << '\n';
					std::cerr << "    Number of crossings differ.\n";
					std::cerr << "    C(identity)= " << C << '\n';
					std::cerr << "    C(shifted)= " << C2 << '\n';
					std::cerr << "   " << arr << '\n';
					std::cerr << "    For tree:\n";
					std::cerr << t << '\n';
					return err_type::test_execution;
				}
			}
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_linear_arrangement
(const input_list& inputs, std::ifstream&)
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	// set of shifts

	case_1();
	case_2();
	case_3();
	case_4();
	case_5();

	// set of manual manipulations

	case_6();

	// ensuring that the number of crossings stays the same

	{
	auto r = case_7();
	if (r != err_type::no_error) { return r; }
	}

	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
