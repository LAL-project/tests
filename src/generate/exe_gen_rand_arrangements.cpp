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
#include <set>
using namespace std;

// lal includes
#include <lal/graphs/output.hpp>
#include <lal/generate/rand_arrangements.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/linarr/C.hpp>
#include <lal/iterators/E_iterator.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace iterators;

// custom includes
#include "../definitions.hpp"

template<class Tree>
bool is_root_covered(const Tree& rT, const linearrgmnt& pi) {
	const node R = rT.get_root();
	bool covered = false;

	E_iterator e_it(rT);
	uint32_t n_edges = 0;

	while (e_it.has_next() and not covered) {
		e_it.next();
		++n_edges;
		const edge e = e_it.get_edge();

		const node s = e.first;
		const node t = e.second;
		covered =
			((pi[s] < pi[R]) and (pi[R] < pi[t])) or
			((pi[t] < pi[R]) and (pi[R] < pi[s]));
	}

	return covered;
}

bool is_linarr_projective(const urtree& t, const linearrgmnt& arr) {
	uint32_t C = lal::linarr::n_crossings(t, arr);
	if (C != 0) { return false; }
	return not is_root_covered(t, arr);
}
bool is_linarr_projective(const drtree& t, const linearrgmnt& arr) {
	uint32_t C = lal::linarr::n_crossings(t.to_undirected(), arr);
	if (C != 0) { return false; }
	return not is_root_covered(t, arr);
}

template<class Tree>
uint32_t is_rand_proj_arr_correct(const Tree& t, const linearrgmnt& arr) {
	set<position> setpos;
	for (node u = 0; u < t.n_nodes(); ++u) {
		setpos.insert(arr[u]);
	}

	if (setpos.size() != t.n_nodes()) {
		// there are collisions in vertices' positions.
		return 0x00000001;
	}

	if (not is_linarr_projective(t, arr)) {
		// the arrangement is not projective;
		return 0x00000002;
	}

	// no error
	return 0x00000000;
}

vector<node> invlinarr(const linearrgmnt& arr) {
	vector<node> ilin(arr.size());
	for (uint32_t p : arr) { ilin[ arr[p] ] = p; }
	return ilin;
}

namespace std {
template<class T>
ostream& operator<< (ostream& os, const vector<T>& v) {
	if (v.size() == 0) { return os; }
	os << v[0];
	for (size_t i = 1; i < v.size(); ++i) {
		os << " " << v[i];
	}
	return os;
}
}

namespace exe_tests {

#define check_errors(tree_type, T, err, arr)											\
	cerr << ERROR << endl;																\
	cerr << "    Generation of random arrangement for " << tree_type << ":" << endl;	\
	cerr << T << endl;																	\
	cerr << "    Failed with code: " << err << endl;									\
	cerr << "    Arrangement:" << endl;													\
	cerr << "    " << arr << endl;														\
	cerr << "    Inverse linear arrangement:" << endl;									\
	cerr << "    " << invlinarr(arr) << endl;

err_type test_rand_projective_arrangements(ifstream& fin) {
	uint32_t n, ntrees, nit;
	while (fin >> n >> ntrees >> nit) {
		// do 'ntrees' trees of 'n' vertices
		rand_ulab_rooted_trees TreeGen(n);

		uint32_t nt = 0;
		while (nt < ntrees) {
			const urtree urT = TreeGen.make_rand_tree();
			const drtree drT(urT, urT.get_root());

			for (uint32_t it = 0; it < nit; ++it) {
				const linearrgmnt urT_arr = rand_projective_arrgmnt(urT);
				const linearrgmnt drT_arr = rand_projective_arrgmnt(drT);

				// Do some sanity checks.
				const uint32_t urT_err = is_rand_proj_arr_correct(urT, urT_arr);
				if (urT_err != 0) {
					check_errors("urtree", urT, urT_err, urT_arr);
					return err_type::test_exe_error;
				}

				const uint32_t drT_err = is_rand_proj_arr_correct(drT, drT_arr);
				if (drT_err != 0) {
					check_errors("drtree", drT, drT_err, drT_arr);
					return err_type::test_exe_error;
				}
			}

			++nt;
		}
	}
	return err_type::no_error;
}

err_type exe_gen_rand_arrangements(ifstream& fin) {

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
		cerr << "    Expected no inputs at all." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
		return err_type::test_format_error;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	// --- do the tests

	string arr_type;
	fin >> arr_type;

	const set<string> allowed_arr_types({"projective"});
	if (allowed_arr_types.find(arr_type) == allowed_arr_types.end()) {
		cerr << ERROR << endl;
		cerr << "    Unhandled arrangement type '" << arr_type << "'." << endl;
		cerr << "    Allowed values: " << endl;
		for (const string& type : allowed_arr_types) {
		cerr << "    - " << type << endl;
		}
		return err_type::test_format_error;
	}

	const err_type err =
	[&]() -> err_type {
		if (arr_type == "projective") {
			return test_rand_projective_arrangements(fin);
		}
		return err_type::test_format_error;
	}();

	if (err != err_type::no_error) { return err; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
