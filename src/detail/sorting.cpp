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

// C++ includes
#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <random>
#include <set>

// lal includes
#include <lal/detail/sorting/insertion_sort.hpp>
#include <lal/detail/sorting/bit_sort.hpp>
#include <lal/detail/sorting/sorting_types.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

// common includes
#include "common/definitions.hpp"

typedef uint64_t Ui;
typedef std::vector<uint64_t>::iterator Ui_it;

typedef std::tuple<Ui> t1;
typedef std::vector<t1> t1_vec;
typedef t1_vec::iterator t1_vec_it;

typedef std::tuple<Ui, Ui> t2;
typedef std::vector<t2> t2_vec;
typedef t2_vec::iterator t2_t;

typedef std::tuple<Ui, Ui, Ui> t3;
typedef std::vector<t3> t3_vec;
typedef t3_vec::iterator t3_vec_it;

// -----------------------------------------------------------------------------

// size, values in range [0,n)
static inline std::vector<Ui> random_vector_unique(Ui s, Ui n) noexcept
{
	// always use the same seed
	std::mt19937 gen(1234);

	// available values in [0,n)
	std::vector<Ui> av(n);
	iota(av.begin(), av.end(), 0);
	std::size_t max_idx = av.size() - 1;

	// random vector
	std::vector<Ui> R(s);

	for (std::size_t i = 0; i < R.size(); ++i) {
		std::uniform_int_distribution<std::size_t> U(0, max_idx);
		std::size_t index = U(gen);

		R[i] = av[index];
		std::swap(av[index], av[max_idx]);
		--max_idx;
	}
	return R;
}

// -----------------------------------------------------------------------------

static inline std::vector<Ui> random_vector_multiple(Ui s, Ui n) noexcept
{
	// always use the same seed
	std::mt19937 gen(1234);
	std::uniform_int_distribution<Ui> U(0, n);
	// random vector
	std::vector<Ui> R(s);
	for (std::size_t i = 0; i < R.size(); ++i) {
		R[i] = U(gen);
	}
	return R;
}

// -----------------------------------------------------------------------------

template <std::size_t idx = 0>
struct struct_output {

	template <typename V1, typename... Params>
	static constexpr void
	output(const std::tuple<V1, Params...>& t, std::ostream& out) noexcept
	{
		out << " " << std::get<idx>(t);

		if constexpr (idx < sizeof...(Params)) {
			struct_output<idx + 1>::output(t, out);
		}
	}

	static constexpr void output(const Ui& t, std::ostream& out) noexcept
	{
		out << " " << t;
	}
};

template <std::size_t idx = 0>
struct struct_assign {

	template <typename Tuple, typename V1, typename... Params>
	static void assign(Tuple& t, const V1& v1, const Params&...params) noexcept
	{
		std::get<idx>(t) = v1;

		if constexpr (idx < std::tuple_size_v<Tuple> - 1) {
			struct_assign<idx + 1>::assign(t, params...);
		}
	}

	template <typename Tuple, typename V1, typename... Params>
	static void assign(Tuple& t, V1&& v1, Params&&...params) noexcept
	{
		std::get<idx>(t) = std::move(v1);

		if constexpr (idx < std::tuple_size_v<Tuple> - 1) {
			struct_assign<idx + 1>::assign(t, params...);
		}
	}
};

// -----------------------------------------------------------------------------

namespace tests {
namespace detail {

// Sort two vectors and compare them.
// - algo: name of the sorting algorithm (for debugging purposes)
// - v1, v2: the same vectors.
//     v1 is sorted by "std::sort"
//     v2 is sorted by our algorithm
// - sort_F: our sorting algorithm
// - incr: should the sorting be done in increasing order?
template <class T, typename Callable>
err_type _check_sorting(
	const std::string& algo,
	std::vector<T> v1,
	const Callable& sort_F,
	bool incr
) noexcept
{
	// sort with our algorithm
	sort_F(v1.begin(), v1.end());

	bool is_sorted;
	if (incr) {
		is_sorted = std::is_sorted(v1.begin(), v1.end());
	}
	else {
		std::reverse(v1.begin(), v1.end());
		is_sorted = std::is_sorted(v1.begin(), v1.end());
	}

	if (not is_sorted) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Sorting algorithm '" << algo << "' is not correct.\n";
		std::cerr << "    Vector sorted with '" << algo << "':\n";
		std::cerr << "    ";
		for (auto k : v1) {
			struct_output<>::output(k, std::cerr);
		}
		std::cerr << '\n';

		std::sort(v1.begin(), v1.end());
		std::cerr << "    Vector sorted with std::sort:\n";
		std::cerr << "    ";
		for (auto k : v1) {
			struct_output<>::output(k, std::cerr);
		}
		std::cerr << '\n';
		return err_type::test_execution;
	}
	return err_type::no_error;
}

// -----------------------------------------------------------------------------

// Check sorting of uni-dimensional vectors.
// - algo: name of the sorting algorithm (for debugging purposes)
// - s: size of the vector
// - n: upper bound of the values in the vector
// - sort_F: our sorting algorithm
// - incr: should the sorting be done in increasing order?
template <typename Callable>
err_type check_sorting(
	const std::string& algo, Ui s, Ui n, const Callable& sort_F, bool incr
) noexcept
{
	const std::vector<Ui> R = random_vector_unique(s, n);
	return _check_sorting(algo, R, sort_F, incr);
}

// -----------------------------------------------------------------------------

template <typename It, typename T, typename Callable>
void here_counting_sort(
	It begin, It end, const std::size_t n, const Callable& key, bool incr
) noexcept
{
	const std::size_t size =
		static_cast<std::size_t>(std::distance(begin, end));
	if (incr) {
		lal::detail::sorting::
			counting_sort<T, lal::detail::sorting::sort_type::non_decreasing>(
				begin, end, n, size, key
			);
	}
	else {
		lal::detail::sorting::
			counting_sort<T, lal::detail::sorting::sort_type::non_increasing>(
				begin, end, n, size, key
			);
	}
}

// check sorting of vectors of tuples
err_type check_counting_sort(const bool incr, Ui k, Ui s, Ui n) noexcept
{
	const auto key1 = [](const t1& t) -> std::size_t
	{
		return std::get<0>(t);
	};
	const auto key2 = [](const t2& t) -> std::size_t
	{
		return std::get<0>(t);
	};
	const auto key3 = [](const t3& t) -> std::size_t
	{
		return std::get<0>(t);
	};

	const auto sort1 = [&](t1_vec_it begin, t1_vec_it end) -> void
	{
		here_counting_sort<t1_vec_it, t1>(begin, end, n, key1, incr);
	};
	const auto sort2 = [&](t2_t begin, t2_t end) -> void
	{
		here_counting_sort<t2_t, t2>(begin, end, n, key2, incr);
	};
	const auto sort3 = [&](t3_vec_it begin, t3_vec_it end) -> void
	{
		here_counting_sort<t3_vec_it, t3>(begin, end, n, key3, incr);
	};

	if (k == 1) {
		// vector of tuples
		t1_vec R(s);
		// fill vector of tuples
		const std::vector<Ui> r = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			struct_assign<>::assign(R[i], r[i]);
		}
		// check sorting algorithm
		return _check_sorting<t1>("counting_sort", R, sort1, incr);
	}

	if (k == 2) {
		// vector of tuples
		t2_vec R(s);
		// fill vector of tuples
		const std::vector<Ui> r1 = random_vector_multiple(s, n);
		const std::vector<Ui> r2 = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			struct_assign<>::assign(R[i], r1[i], r2[i]);
		}
		// check sorting algorithm
		return _check_sorting<t2>("counting_sort", R, sort2, incr);
	}

	if (k == 3) {
		// vector of tuples
		t3_vec R(s);
		// fill vector of tuples
		const std::vector<Ui> r1 = random_vector_multiple(s, n);
		const std::vector<Ui> r2 = random_vector_multiple(s, n);
		const std::vector<Ui> r3 = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			struct_assign<>::assign(R[i], r1[i], r2[i], r3[i]);
		}
		// check sorting algorithm
		return _check_sorting<t3>("counting_sort", R, sort3, incr);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Size of tuple '" << k << "' not captured.\n";
	return err_type::test_format;
}

err_type
exe_rand_sorting(const std::string& option, std::ifstream& fin) noexcept
{
	if (option == "insertion_sort_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void
		{
			lal::detail::sorting::insertion_sort(begin, end);
		};
		for (Ui k = 0; k < R; ++k) {
			const err_type e =
				check_sorting("insertion", s, n, this_sort, true);
			if (e != err_type::no_error) {
				return e;
			}
		}
		return err_type::no_error;
	}

	/* BIT SORT */

	// --- without memory

	if (option == "bit_sort_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void
		{
			lal::detail::sorting::bit_sort<Ui>(
				begin, end, static_cast<std::size_t>(std::distance(begin, end))
			);
		};
		for (Ui k = 0; k < R; ++k) {
			const err_type e = check_sorting("bit", s, n, this_sort, true);
			if (e != err_type::no_error) {
				return e;
			}
		}
		return err_type::no_error;
	}

	// --- with memory

	if (option == "bit_sort_mem_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		// bit array
		std::vector<char> seen(n, 0);
		auto bsm = [&](Ui_it begin, Ui_it end) -> void
		{
			lal::detail::sorting::bit_sort_mem<Ui>(
				begin,
				end,
				static_cast<std::size_t>(std::distance(begin, end)),
				&seen[0]
			);
		};
		// execute test
		for (Ui k = 0; k < R; ++k) {
			const err_type e = check_sorting("bit_memory", s, n, bsm, true);
			// check for two errors
			if (find(seen.begin(), seen.end(), 1) != seen.end()) {
				std::cerr << ERROR << '\n';
				std::cerr << "    Memory array 'seen' contains true values.\n";
				return err_type::test_execution;
			}
			if (e != err_type::no_error) {
				return e;
			}
		}
		return err_type::no_error;
	}

	/* COUNTING SORT */

	if (option == "counting_sort_rand") {
		std::string order;
		fin >> order;
		if (order != "increasing" and order != "decreasing") {
			std::cerr << ERROR << '\n';
			std::cerr << "    Order std::string '" << order
					  << "' is not valid.\n";
			return err_type::test_format;
		}
		const bool incr = order == "increasing";

		Ui k, R, s, n;
		fin >> k >> R >> s >> n;

		// execute test
		for (Ui rep = 0; rep < R; ++rep) {
			const err_type e = check_counting_sort(incr, k, s, n);
			if (e != err_type::no_error) {
				return e;
			}
		}
		return err_type::no_error;
	}

	if (option == "counting_sort_nrand") {
		std::string order;
		fin >> order;
		if (order != "increasing" and order != "decreasing") {
			std::cerr << ERROR << '\n';
			std::cerr << "    Order std::string '" << order
					  << "' is not valid.\n";
			return err_type::test_format;
		}
		const bool incr = order == "increasing";

		uint64_t n;
		fin >> n;

		uint64_t Max = 0;
		std::vector<uint64_t> values(n);
		for (auto& v : values) {
			fin >> v;
			Max = std::max(Max, v);
		}

		auto key1 = [](const uint64_t& t) -> std::size_t
		{
			return t;
		};
		auto sort1 = [&](Ui_it begin, Ui_it end) -> void
		{
			here_counting_sort<Ui_it, uint64_t>(begin, end, Max, key1, incr);
		};
		return _check_sorting<uint64_t>("counting_sort", values, sort1, incr);
	}

	std::cerr << ERROR << '\n';
	std::cerr << "    Option '" << option << "' not valid.\n";
	return err_type::test_format;
}

err_type exe_detail_sorting(std::ifstream& fin) noexcept
{
	const std::set<std::string> allowed_options(
		{"insertion_sort_rand",
		 "bit_sort_rand",
		 "bit_sort_mem_rand",
		 "counting_sort_rand",
		 "counting_sort_nrand"}
	);

	std::string option;
	while (fin >> option) {
		if (allowed_options.find(option) != allowed_options.end()) {
			const err_type e = exe_rand_sorting(option, fin);
			if (e != err_type::no_error) {
				return e;
			}
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Invalid option '" << option << "'.\n";
			std::cerr << "    Valid options are:\n";
			for (const std::string& s : allowed_options) {
				std::cerr << "        " << s << '\n';
			}
			return err_type::test_format;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace detail
} // namespace tests
