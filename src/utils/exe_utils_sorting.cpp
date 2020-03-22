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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
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
#include <lal/utils/sorting/insertion_sort.hpp>
#include <lal/utils/sorting/bit_sort.hpp>
#include <lal/utils/sorting/counting_sort.hpp>
using namespace lal;

// custom includes
#include "../definitions.hpp"

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

// sort two vectors and compare them
template<class T, class It>
err_type __check_sorting
(
	const string& algo, vector<T> v1, vector<T> v2,
	function<void (It begin, It end)> sort_F
)
{
	std::sort(v1.begin(), v1.end());
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

// check sorting of uni-dimensional vectors
err_type check_sorting(
	const string& algo, Ui s, Ui n,
	function<void (Ui_it begin, Ui_it end)> sort_F
)
{
	const vector<Ui> R = random_vector_unique(s, n);
	return __check_sorting(algo, R, R, sort_F);
}

// -----------------------------------------------------------------------------

// check sorting of vectors of tuples
err_type check_counting_sort(const string& algo, Ui k, Ui s, Ui n) {

	auto key1 = [](const t1& t) -> size_t { return std::get<0>(t); };
	auto key2 = [](const t2& t) -> size_t { return std::get<0>(t); };
	auto key3 = [](const t3& t) -> size_t { return std::get<0>(t); };

	auto sort1 =
	[&](t1_vec_it begin, t1_vec_it end) -> void {
		utils::counting_sort<t1_vec_it, t1>(begin, end, n+1, key1);
	};
	auto sort2 =
	[&](t2_vec_it begin, t2_vec_it end) -> void {
		utils::counting_sort<t2_vec_it, t2>(begin, end, n+1, key2);
	};
	auto sort3 =
	[&](t3_vec_it begin, t3_vec_it end) -> void {
		utils::counting_sort<t3_vec_it, t3>(begin, end, n+1, key3);
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
		E = __check_sorting<t1,t1_vec_it>(algo, R,R, sort1);
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
		E = __check_sorting<t2,t2_vec_it>(algo, R,R, sort2);
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
		E = __check_sorting<t3,t3_vec_it>(algo, R,R, sort3);
	}
	else {
		cerr << ERROR << endl;
		cerr << "    Size of tuple '" << k << "' not captured." << endl;
		return err_type::test_format_error;
	}
	return E;
}

err_type exe_rand_sorting(const string& option, ifstream& fin) {
	if (option == "rand_insertion_sort") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void {
			utils::insertion_sort(begin, end);
		};
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("insertion", s, n, this_sort);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	if (option == "rand_bit_sort") {
		Ui R, s, n;
		fin >> R >> s >> n;
		auto this_sort = [&](Ui_it begin, Ui_it end) -> void {
			utils::bit_sort(begin, end);
		};
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("bit", s, n, this_sort);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	if (option == "rand_bit_sort_mem") {
		Ui R, s, n;
		fin >> R >> s >> n;
		// bit array
		vector<char> seen(n, 0);
		auto bsm = [&](Ui_it begin, Ui_it end) -> void {
			utils::bit_sort_mem(begin, end, &seen[0]);
		};
		// execute test
		for (Ui k = 0; k < R; ++k) {
			err_type e = check_sorting("bit_memory", s, n, bsm);
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

	if (option == "rand_counting_sort") {
		Ui k, R, s, n;
		fin >> k >> R >> s >> n;

		// execute test
		for (Ui rep = 0; rep < R; ++rep) {
			err_type e = check_counting_sort("counting_sort", k, s, n);
			if (e != err_type::no_error) { return e; }
		}
		return err_type::no_error;
	}

	cerr << ERROR << endl;
	cerr << "    Option '" << option << "' not valid." << endl;
	return err_type::test_format_error;
}

err_type exe_utils_sorting(ifstream& fin) {
	const set<string> allowed_options({
		"rand_insertion_sort", "rand_bit_sort",
		"rand_bit_sort_mem", "rand_counting_sort"
	});

	string field;
	fin >> field;

	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	size_t n_inputs;
	fin >> n_inputs;
	if (n_inputs != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n_inputs << " were specified." << endl;
		return err_type::test_format_error;
	}

	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	string option;
	while (fin >> option) {
		if (option.substr(0, 4) == "rand") {
			err_type e = exe_rand_sorting(option, fin);
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
