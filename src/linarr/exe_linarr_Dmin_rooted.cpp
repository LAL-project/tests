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
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <set>
using namespace std;

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/graphs/rtree.hpp>
#include <lal/iterators/E_iterator.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/C.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/io/basic_output.hpp>
using namespace lal;
using namespace graphs;
using namespace numeric;
using namespace iterators;
using namespace linarr;

// custom includes
#include "../io_wrapper.hpp"
#include "../definitions.hpp"

inline std::vector<lal::node> ilinarr(const lal::linearrgmnt& linarr) {
	std::vector<lal::node> ilin(linarr.size());
	for (uint32_t p : linarr) {
		ilin[ linarr[p] ] = p;
	}
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

inline bool is_projective(const ftree& ft, node root, const linearrgmnt& arr) {
	const uint32_t C = n_crossings(ft, arr);
	if (C != 0) { return false; }
	E_iterator it(ft);
	while (it.has_next()) {
		it.next();
		const edge e = it.get_edge();
		const node s = e.first;
		const node t = e.second;
		const bool st_cover = arr[s] < arr[root] and arr[root] < arr[t];
		const bool ts_cover = arr[t] < arr[root] and arr[root] < arr[s];
		if (st_cover or ts_cover) {
			return false;
		}
	}
	return true;
}

pair<uint32_t, linearrgmnt> Dmin_projective_bruteforce(const rtree& t) {
	const uint32_t n = t.n_nodes();
	if (n == 1) { return make_pair(0, linearrgmnt(0,0)); }

	const ftree ft = t.to_undirected();
	uint32_t Dmin = numeric_limits<uint32_t>::max();
	linearrgmnt arrMin;
	vector<position> arr(n);
	std::iota(arr.begin(), arr.end(), 0);
	do {
		if (is_projective(ft, t.get_root(), arr)) {
			const uint32_t D = sum_length_edges(t, arr);
			if (Dmin > D) {
				Dmin = D;
				arrMin = arr;
			}
		}
	}
	while (std::next_permutation(arr.begin(), arr.end()));
	return make_pair(Dmin, arrMin);
}

err_type test_Dmin_rtree_Projective(rtree& t) {
	if (not t.rtree_type_valid()) {
		t.find_rtree_type();
	}
	if (t.need_recalc_size_subtrees()) {
		t.recalc_size_subtrees();
	}

	// compute Dmin using the library's algorithm
	pair<uint32_t, linearrgmnt> res = compute_Dmin(t, algorithms_Dmin::Projective);

	// compute Dmin by brute force
	pair<uint32_t, linearrgmnt> res_bf = Dmin_projective_bruteforce(t);

	// compare results
	if (res_bf.first != res.first) {
		cerr << ERROR << endl;
		cerr << "    Values of Dmin do not coincide." << endl;
		cerr << "    Library:" << endl;
		cerr << "        Value: " << res.first << endl;
		cerr << "        Arrangement:     " << res.second << endl;
		cerr << "        Inv Arrangement: " << ilinarr(res.second) << endl;
		cerr << "    Bruteforce:" << endl;
		cerr << "        Value: " << res_bf.first << endl;
		cerr << "        Arrangement:     " << res_bf.second << endl;
		cerr << "        Inv Arrangement: " << ilinarr(res_bf.second) << endl;
		cerr << "    For tree: " << endl;
		cerr << t << endl;
		return err_type::test_exe_error;
	}
	return err_type::no_error;
}

err_type test_projective(ifstream& fin) {
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
	if (n_inputs >= 1) {
		cerr << ERROR << endl;
		cerr << "    Expected at most one input." << endl;
		cerr << "    Instead, '" << n_inputs << "' were found." << endl;
		return err_type::test_format_error;
	}

	string graph_name;
	string graph_format;

	if (n_inputs == 1) {
		fin >> graph_name >> graph_format;
	}

	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	if (n_inputs == 1) {
		rtree tree;
		const err_type r = io_wrapper::read_graph(graph_name, graph_format, tree);
		if (r != err_type::no_error) {
			return r;
		}

		// execute tests for this particular tree
		return test_Dmin_rtree_Projective(tree);
	}

	// read number of nodes
	uint32_t n;
	while (fin >> n) {

		generate::all_ulab_rooted_trees TreeGen(n);
		while (TreeGen.has_next()) {
			TreeGen.next();
			rtree tree = TreeGen.get_tree();

			err_type r = test_Dmin_rtree_Projective(tree);
			if (r != err_type::no_error) {
				return r;
			}
		}

	}

	return err_type::no_error;
}

err_type exe_linarr_Dmin_rooted(const string& alg, ifstream& fin) {
	err_type r = [&]() -> err_type {
		if (alg == "Projective") {
			return test_projective(fin);
		}
		cerr << ERROR << endl;
		cerr << "    Test not implemented for algorithm 'alg'." << endl;
		return err_type::not_implemented;
	}();

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
