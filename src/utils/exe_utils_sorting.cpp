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
#include <iostream>
#include <fstream>
#include <random>
#include <set>
using namespace std;

// lal includes
#include <lal/utils/sort_integers.hpp>
using namespace lal;
using namespace utils;

// custom includes
#include "../definitions.hpp"

// size, values in range [0,n)
static inline
vector<uint32_t> random_vector(uint32_t r, uint32_t n) {
	// always use the same seed
	std::mt19937 gen(1234);

	// available values in [0,n)
	vector<uint32_t> av(n);
	iota(av.begin(), av.end(), 0);

	// random vector
	vector<uint32_t> R(r);
	for (size_t i = 0; i < R.size(); ++i) {
		std::uniform_int_distribution<size_t> U(0, av.size() - 1);
		size_t index = U(gen);

		R[i] = av[index];
		auto it = av.begin();
		std::advance(it, index);
		av.erase(it);
	}
	return R;
}


namespace exe_tests {

err_type check_sorting(
	const string& algo, uint32_t r, uint32_t n,
	void (*sort_F)(vector<uint32_t>::iterator begin, vector<uint32_t>::iterator end)
)
{
	vector<uint32_t> R = random_vector(r, n);

	vector<uint32_t> Rc1 = R;
	std::sort(Rc1.begin(), Rc1.end());

	vector<uint32_t> Rc2 = R;
	sort_F(Rc2.begin(), Rc2.end());

	if (Rc1 != Rc2) {
		cerr << ERROR << endl;
		cerr << "    Sorting algorithm '" << algo << "' is not correct." << endl;
		cerr << "    Vector sorted with std::sort:" << endl;
		cerr << "    ";
		for (auto k : Rc1) { cerr << " " << k; }
		cerr << endl;
		cerr << "    Vector sorted with '" << algo << "':" << endl;
		cerr << "    ";
		for (auto k : Rc2) { cerr << " " << k; }
		cerr << endl;
		return err_type::test_exe_error;
	}
	return err_type::test_format_error;
}

err_type exe_utils_sorting(ifstream& fin) {
	const set<string> allowed_options({
		"sort_insertion", "sort_boolean"
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
		if (option == "sort_insertion") {
			uint32_t r, n;
			fin >> r >> n;
			err_type e = check_sorting("insertion", r, n, insertion_sort);
			if (e == err_type::test_exe_error) { return e; }
		}
		else if (option == "sort_boolean") {
			uint32_t r, n;
			fin >> r >> n;
			err_type e = check_sorting("boolean", r, n, sort_1_n_inc);
			if (e == err_type::test_exe_error) { return e; }
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
