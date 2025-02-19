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

#pragma once

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>

namespace tests {
namespace numeric {

template <typename numeric_t, typename integral_t>
concept numeric_add = requires(const numeric_t& n, const integral_t& i) {
	{ n + i } -> std::same_as<numeric_t>;
	{ i + n } -> std::same_as<numeric_t>;
	{ n + n } -> std::same_as<numeric_t>;
};

template <
	typename numeric_t,
	typename integral_t,
	typename result_t = numeric_t>
concept numeric_subtract = requires(const numeric_t& n, const integral_t& i) {
	{ n - i } -> std::same_as<result_t>;
	{ i - n } -> std::same_as<result_t>;
	{ n - n } -> std::same_as<result_t>;
	{ -n } -> std::same_as<result_t>;
};

template <typename numeric_t, typename integral_t>
concept numeric_multiply = requires(const numeric_t& n, const integral_t& i) {
	{ n *i } -> std::same_as<numeric_t>;
	{ i *n } -> std::same_as<numeric_t>;
	{ n *n } -> std::same_as<numeric_t>;
};

template <typename integral_t>
concept integer_divide =
	requires(const lal::numeric::integer& n, const integral_t& i) {
		{ n / i } -> std::same_as<lal::numeric::integer>;
		{ n / n } -> std::same_as<lal::numeric::integer>;
	};

template <typename integral_t>
concept rational_divide =
	requires(const lal::numeric::rational& r, const integral_t& i) {
		{ i / r } -> std::same_as<lal::numeric::rational>;
		{ r / i } -> std::same_as<lal::numeric::rational>;
		{ r / r } -> std::same_as<lal::numeric::rational>;
	};

template <typename numeric_t, typename integral_t>
concept numeric_comparable = requires(const numeric_t& n, const integral_t& i) {
	{ n == i } -> std::same_as<bool>;
	{ i == n } -> std::same_as<bool>;
	{ n != i } -> std::same_as<bool>;
	{ i != n } -> std::same_as<bool>;
	{ n >= i } -> std::same_as<bool>;
	{ i >= n } -> std::same_as<bool>;
	{ n > i } -> std::same_as<bool>;
	{ i > n } -> std::same_as<bool>;
	{ n <= i } -> std::same_as<bool>;
	{ i <= n } -> std::same_as<bool>;
	{ n < i } -> std::same_as<bool>;
	{ i < n } -> std::same_as<bool>;
};

// INTEGER

static_assert(numeric_add<lal::numeric::integer, char>);
static_assert(numeric_add<lal::numeric::integer, unsigned char>);
static_assert(numeric_add<lal::numeric::integer, int8_t>);
static_assert(numeric_add<lal::numeric::integer, uint8_t>);
static_assert(numeric_add<lal::numeric::integer, int16_t>);
static_assert(numeric_add<lal::numeric::integer, uint16_t>);
static_assert(numeric_add<lal::numeric::integer, int32_t>);
static_assert(numeric_add<lal::numeric::integer, uint32_t>);
static_assert(numeric_add<lal::numeric::integer, int64_t>);
static_assert(numeric_add<lal::numeric::integer, uint64_t>);
static_assert(numeric_add<lal::numeric::integer, lal::numeric::integer>);

static_assert(numeric_subtract<lal::numeric::integer, char>);
static_assert(numeric_subtract<lal::numeric::integer, unsigned char>);
static_assert(numeric_subtract<lal::numeric::integer, int8_t>);
static_assert(numeric_subtract<lal::numeric::integer, uint8_t>);
static_assert(numeric_subtract<lal::numeric::integer, int16_t>);
static_assert(numeric_subtract<lal::numeric::integer, uint16_t>);
static_assert(numeric_subtract<lal::numeric::integer, int32_t>);
static_assert(numeric_subtract<lal::numeric::integer, uint32_t>);
static_assert(numeric_subtract<lal::numeric::integer, int64_t>);
static_assert(numeric_subtract<lal::numeric::integer, uint64_t>);
static_assert(numeric_subtract<lal::numeric::integer, lal::numeric::integer>);

static_assert(numeric_multiply<lal::numeric::integer, char>);
static_assert(numeric_multiply<lal::numeric::integer, unsigned char>);
static_assert(numeric_multiply<lal::numeric::integer, int8_t>);
static_assert(numeric_multiply<lal::numeric::integer, uint8_t>);
static_assert(numeric_multiply<lal::numeric::integer, int16_t>);
static_assert(numeric_multiply<lal::numeric::integer, uint16_t>);
static_assert(numeric_multiply<lal::numeric::integer, int32_t>);
static_assert(numeric_multiply<lal::numeric::integer, uint32_t>);
static_assert(numeric_multiply<lal::numeric::integer, int64_t>);
static_assert(numeric_multiply<lal::numeric::integer, uint64_t>);
static_assert(numeric_multiply<lal::numeric::integer, lal::numeric::integer>);

static_assert(integer_divide<char>);
static_assert(integer_divide<unsigned char>);
static_assert(integer_divide<int8_t>);
static_assert(integer_divide<uint8_t>);
static_assert(integer_divide<int16_t>);
static_assert(integer_divide<uint16_t>);
static_assert(integer_divide<int32_t>);
static_assert(integer_divide<uint32_t>);
static_assert(integer_divide<int64_t>);
static_assert(integer_divide<uint64_t>);
static_assert(integer_divide<lal::numeric::integer>);

static_assert(numeric_comparable<lal::numeric::integer, char>);
static_assert(numeric_comparable<lal::numeric::integer, unsigned char>);
static_assert(numeric_comparable<lal::numeric::integer, int8_t>);
static_assert(numeric_comparable<lal::numeric::integer, uint8_t>);
static_assert(numeric_comparable<lal::numeric::integer, int16_t>);
static_assert(numeric_comparable<lal::numeric::integer, uint16_t>);
static_assert(numeric_comparable<lal::numeric::integer, int32_t>);
static_assert(numeric_comparable<lal::numeric::integer, uint32_t>);
static_assert(numeric_comparable<lal::numeric::integer, int64_t>);
static_assert(numeric_comparable<lal::numeric::integer, uint64_t>);
static_assert(numeric_comparable<lal::numeric::integer, lal::numeric::integer>);

// RATIONAL

static_assert(numeric_add<lal::numeric::rational, char>);
static_assert(numeric_add<lal::numeric::rational, unsigned char>);
static_assert(numeric_add<lal::numeric::rational, int8_t>);
static_assert(numeric_add<lal::numeric::rational, uint8_t>);
static_assert(numeric_add<lal::numeric::rational, int16_t>);
static_assert(numeric_add<lal::numeric::rational, uint16_t>);
static_assert(numeric_add<lal::numeric::rational, int32_t>);
static_assert(numeric_add<lal::numeric::rational, uint32_t>);
static_assert(numeric_add<lal::numeric::rational, int64_t>);
static_assert(numeric_add<lal::numeric::rational, uint64_t>);
static_assert(numeric_add<lal::numeric::rational, lal::numeric::integer>);
static_assert(numeric_add<lal::numeric::rational, lal::numeric::rational>);

static_assert(numeric_subtract<lal::numeric::rational, char>);
static_assert(numeric_subtract<lal::numeric::rational, unsigned char>);
static_assert(numeric_subtract<lal::numeric::rational, int8_t>);
static_assert(numeric_subtract<lal::numeric::rational, uint8_t>);
static_assert(numeric_subtract<lal::numeric::rational, int16_t>);
static_assert(numeric_subtract<lal::numeric::rational, uint16_t>);
static_assert(numeric_subtract<lal::numeric::rational, int32_t>);
static_assert(numeric_subtract<lal::numeric::rational, uint32_t>);
static_assert(numeric_subtract<lal::numeric::rational, int64_t>);
static_assert(numeric_subtract<lal::numeric::rational, uint64_t>);
static_assert(numeric_subtract<lal::numeric::rational, lal::numeric::integer>);
static_assert(numeric_subtract<lal::numeric::rational, lal::numeric::rational>);

static_assert(numeric_multiply<lal::numeric::rational, char>);
static_assert(numeric_multiply<lal::numeric::rational, unsigned char>);
static_assert(numeric_multiply<lal::numeric::rational, int8_t>);
static_assert(numeric_multiply<lal::numeric::rational, uint8_t>);
static_assert(numeric_multiply<lal::numeric::rational, int16_t>);
static_assert(numeric_multiply<lal::numeric::rational, uint16_t>);
static_assert(numeric_multiply<lal::numeric::rational, int32_t>);
static_assert(numeric_multiply<lal::numeric::rational, uint32_t>);
static_assert(numeric_multiply<lal::numeric::rational, int64_t>);
static_assert(numeric_multiply<lal::numeric::rational, uint64_t>);
static_assert(numeric_multiply<lal::numeric::rational, lal::numeric::integer>);
static_assert(numeric_multiply<lal::numeric::rational, lal::numeric::rational>);

static_assert(rational_divide<char>);
static_assert(rational_divide<unsigned char>);
static_assert(rational_divide<int8_t>);
static_assert(rational_divide<uint8_t>);
static_assert(rational_divide<int16_t>);
static_assert(rational_divide<uint16_t>);
static_assert(rational_divide<int32_t>);
static_assert(rational_divide<uint32_t>);
static_assert(rational_divide<int64_t>);
static_assert(rational_divide<uint64_t>);
static_assert(rational_divide<lal::numeric::integer>);
static_assert(rational_divide<lal::numeric::rational>);

static_assert(numeric_comparable<lal::numeric::rational, char>);
static_assert(numeric_comparable<lal::numeric::rational, unsigned char>);
static_assert(numeric_comparable<lal::numeric::rational, int8_t>);
static_assert(numeric_comparable<lal::numeric::rational, uint8_t>);
static_assert(numeric_comparable<lal::numeric::rational, int16_t>);
static_assert(numeric_comparable<lal::numeric::rational, uint16_t>);
static_assert(numeric_comparable<lal::numeric::rational, int32_t>);
static_assert(numeric_comparable<lal::numeric::rational, uint32_t>);
static_assert(numeric_comparable<lal::numeric::rational, int64_t>);
static_assert(numeric_comparable<lal::numeric::rational, uint64_t>);
static_assert(numeric_comparable<
			  lal::numeric::rational,
			  lal::numeric::integer>);
static_assert(numeric_comparable<
			  lal::numeric::rational,
			  lal::numeric::rational>);

} // namespace numeric
} // namespace tests
