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
#include <functional>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/internal/sorting/insertion_sort.hpp>
#include <lal/internal/sorting/bit_sort.hpp>
#include <lal/internal/sorting/counting_sort.hpp>
using namespace lal;

// custom includes
#include "definitions.hpp"

typedef uint32_t Ui;
typedef vector<uint32_t>::iterator Ui_it;

typedef tuple<Ui> t1;
typedef vector<t1> t1_vec;
typedef t1_vec::iterator t1_vec_it;

typedef tuple<Ui,Ui> t2;
typedef vector<t2> t2_vec;
typedef t2_vec::iterator t2_vec_it;

typedef tuple<Ui,Ui,Ui> t3;
typedef vector<t3> t3_vec;
typedef t3_vec::iterator t3_vec_it;

// -----------------------------------------------------------------------------

// size, values in range [0,n)
static inline vector<Ui> random_vector_unique(Ui s, Ui n) {
	// always use the same seed
	std::mt19937 gen(1234);

	// available values in [0,n)
	vector<Ui> av(n);
	iota(av.begin(), av.end(), 0);
	size_t max_idx = av.size() - 1;

	// random vector
	vector<Ui> R(s);

	for (size_t i = 0; i < R.size(); ++i) {
		std::uniform_int_distribution<size_t> U(0, max_idx);
		size_t index = U(gen);

		R[i] = av[index];
		std::swap(av[index], av[max_idx]);
		--max_idx;
	}
	return R;
}

// -----------------------------------------------------------------------------

static inline vector<Ui> random_vector_multiple(Ui s, Ui n) {
	// always use the same seed
	std::mt19937 gen(1234);

	std::uniform_int_distribution<Ui> U(0, n);
	// random vector
	vector<Ui> R(s);

	for (size_t i = 0; i < R.size(); ++i) { R[i] = U(gen); }
	return R;
}

// -----------------------------------------------------------------------------

namespace std {
template<class T> ostream& operator <<(ostream& os, const tuple<T>& t) {
	os << std::get<0>(t);
	return os;
}
template<class T> ostream& operator <<(ostream& os, const tuple<T,T>& t) {
	os << std::get<0>(t) << " "
	   << std::get<1>(t);
	return os;
}
template<class T> ostream& operator <<(ostream& os, const tuple<T,T,T>& t) {
	os << std::get<0>(t) << " "
	   << std::get<1>(t) << " "
	   << std::get<2>(t);
	return os;
}
template<class T> ostream& operator <<(ostream& os, const tuple<T,T,T,T>& t) {
	os << std::get<0>(t) << " "
	   << std::get<1>(t) << " "
	   << std::get<2>(t) << " "
	   << std::get<3>(t);
	return os;
}
template<class T> ostream& operator <<(ostream& os, const tuple<T,T,T,T,T>& t) {
	os << std::get<0>(t) << " "
	   << std::get<1>(t) << " "
	   << std::get<2>(t) << " "
	   << std::get<3>(t) << " "
	   << std::get<4>(t);
	return os;
}
}

// -----------------------------------------------------------------------------

namespace exe_tests {

// Sort two vectors and compare them.
// - algo: name of the sorting algorithm (for debugging purposes)
// - v1, v2: the same vectors.
//     v1 is sorted by "std::sort"
//     v2 is sorted by our algorithm
// - sort_F: our sorting algorithm
// - incr: should the sorting be done in increasing order?
template<class T, class It>
err_type __check_sorting
(
	const string& algo, vector<T> v1, vector<T> v2,
	function<void (It begin, It end)> sort_F,
	bool incr
)
{
	// sort with C++'s standard library's algorithm
	std::sort(v1.begin(), v1.end());
	if (not incr) {
		std::reverse(v1.begin(), v1.end());
	}

	// sort with our algorithm
	sort_F(v2.begin(), v2.end());

	if (v1 != v2) {
		cerr << ERROR << endl;
		cerr << "    Sorting algorithm '" << algo << "' is not correct." << endl;
		cerr << "    Vector sorted with std::sort:" << endl;
		cerr << "    ";
		for (auto k : v1) { cerr << " " << k; }
		cerr << endl;
		cerr << "    Vector sorted with '" << algo << "':" << endl;
		cerr << "    ";
		for (auto k : v2) { cerr << " " << k; }
		cerr << endl;
		return err_type::test_exe_error;
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
err_type check_sorting(
	const string& algo, Ui s, Ui n,
	function<void (Ui_it begin, Ui_it end)> sort_F,
	bool incr
)
{
	const vector<Ui> R = random_vector_unique(s, n);
	return __check_sorting(algo, R, R, sort_F, incr);
}

// -----------------------------------------------------------------------------

// check sorting of vectors of tuples
err_type check_counting_sort(
	const bool incr, Ui k, Ui s, Ui n
)
{
	auto key1 = [](const t1& t) -> size_t { return std::get<0>(t); };
	auto key2 = [](const t2& t) -> size_t { return std::get<0>(t); };
	auto key3 = [](const t3& t) -> size_t { return std::get<0>(t); };

	auto sort1 =
	[&](t1_vec_it begin, t1_vec_it end) -> void {
		internal::counting_sort<t1_vec_it, t1>(begin, end, n, key1, incr);
	};
	auto sort2 =
	[&](t2_vec_it begin, t2_vec_it end) -> void {
		internal::counting_sort<t2_vec_it, t2>(begin, end, n, key2, incr);
	};
	auto sort3 =
	[&](t3_vec_it begin, t3_vec_it end) -> void {
		internal::counting_sort<t3_vec_it, t3>(begin, end, n, key3, incr);
	};

	err_type E = err_type::no_error;

	if (k == 1) {
		// vector of tuples
		t1_vec R(s);
		// fill vector of tuples
		const vector<Ui> r = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			std::get<0>(R[i]) = r[i];
		}
		// check sorting algorithm
		E = __check_sorting<t1,t1_vec_it>("counting_sort", R,R, sort1, incr);
	}
	else if (k == 2) {
		// vector of tuples
		t2_vec R(s);
		// fill vector of tuples
		const vector<Ui> r1 = random_vector_multiple(s, n);
		const vector<Ui> r2 = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			std::get<0>(R[i]) = r1[i];
			std::get<1>(R[i]) = r2[i];
		}
		// check sorting algorithm
		E = __check_sorting<t2,t2_vec_it>("counting_sort", R,R, sort2, incr);
	}
	else if (k == 3) {
		// vector of tuples
		t3_vec R(s);
		// fill vector of tuples
		const vector<Ui> r1 = random_vector_multiple(s, n);
		const vector<Ui> r2 = random_vector_multiple(s, n);
		const vector<Ui> r3 = random_vector_multiple(s, n);
		for (Ui i = 0; i < s; ++i) {
			std::get<0>(R[i]) = r1[i];
			std::get<1>(R[i]) = r2[i];
			std::get<2>(R[i]) = r3[i];
		}
		// check sorting algorithm
		E = __check_sorting<t3,t3_vec_it>("counting_sort", R,R, sort3, incr);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Size of tuple '" << k << "' not captured." << endl;
		return err_type::test_format_error;
	}
	return E;
}

err_type exe_rand_sorting(const string& option, ifstream& fin) {
	if (option == "insertion_sort_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void {
			internal::insertion_sort(begin, end);
		};
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("insertion", s, n, this_sort, true);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	/* BIT SORT */

	// --- without memory

	if (option == "bit_sort_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void {
			internal::bit_sort(begin, end);
		};
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("bit", s, n, this_sort, true);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	// --- with memory

	if (option == "bit_sort_mem_rand") {
		Ui R, s, n;
		fin >> R >> s >> n;
		// bit array
		vector<char> seen(n, 0);
		auto bsm = [&](Ui_it begin, Ui_it end) -> void {
			internal::bit_sort_mem(begin, end, &seen[0]);
		};
		// execute test
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("bit_memory", s, n, bsm, true);
			// check for two errors
			if (find(seen.begin(), seen.end(), 1) != seen.end()) {
				cerr << ERROR << endl;
				cerr << "    Memory array 'seen' contains true values." << endl;
				return err_type::test_exe_error;
			}
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	/* COUNTING SORT */

	if (option == "counting_sort_rand") {
		string order;
		fin >> order;
		if (order != "increasing" and order != "decreasing") {
			cerr << ERROR << endl;
			cerr << "    Order string '" << order << "' is not valid." << endl;
			return err_type::test_format_error;
		}
		const bool incr = order == "increasing";

		Ui k, R, s, n;
		fin >> k >> R >> s >> n;

		// execute test
		for (Ui rep = 0; rep < R; ++rep) {
			const err_type e = check_counting_sort(incr, k, s, n);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	if (option == "counting_sort_nrand") {
		string order;
		fin >> order;
		if (order != "increasing" and order != "decreasing") {
			cerr << ERROR << endl;
			cerr << "    Order string '" << order << "' is not valid." << endl;
			return err_type::test_format_error;
		}
		const bool incr = order == "increasing";

		uint32_t n;
		fin >> n;

		uint32_t Max = 0;
		vector<uint32_t> values(n);
		for (auto& v : values) {
			fin >> v;
			Max = std::max(Max, v);
		}

		auto key1 = [](const uint32_t& t) -> size_t { return t; };
		auto sort1 =
		[&](Ui_it begin, Ui_it end) -> void {
			internal::counting_sort<Ui_it, uint32_t>(begin, end, Max, key1, incr);
		};
		return __check_sorting<uint32_t,Ui_it>(
			"counting_sort", values,values, sort1, incr
		);
	}

	cerr << ERROR << endl;
	cerr << "    Option '" << option << "' not valid." << endl;
	return err_type::test_format_error;
}

err_type exe_internal_sorting(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_options({
		"insertion_sort_rand",
		"bit_sort_rand", "bit_sort_mem_rand",
		"counting_sort_rand", "counting_sort_nrand"
	});

	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	string option;
	while (fin >> option) {
		if (allowed_options.find(option) != allowed_options.end()) {
			const err_type e = exe_rand_sorting(option, fin);
			if (e != err_type::no_error) { return e; }
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid option '" << option << "'." << endl;
			cerr << "    Valid options are:" << endl;
			for (const string& s : allowed_options) {
				cerr << "        " << s << endl;
			}
			return err_type::test_format_error;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
