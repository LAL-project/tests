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
#include <fstream>
#include <set>
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/linarr/formal_constraints.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;

// common includes
#include "common/definitions.hpp"

// linarr includes
#include "linarr/linarr_brute_force_testing.hpp"

namespace tests {
namespace linarr {

namespace tests_Dmin_planar {

pair<uint64_t,linear_arrangement> Dmin_planar_quadratic(const free_tree& t) {
	uint64_t Dmin_planar = numeric_limits<uint64_t>::max();
	linear_arrangement arr;

	for (node u = 0; u < t.get_num_nodes(); ++u) {
		rooted_tree rt(t, u);
		rt.calculate_size_subtrees();

		const auto Dmin_proj = lal::linarr::min_sum_edge_lengths_projective(rt);
		if (Dmin_planar > Dmin_proj.first) {
			Dmin_planar = Dmin_proj.first;
			arr = Dmin_proj.second;
		}
	}
	return {Dmin_planar, arr};
}

inline
bool check_correctness_arr(
	const free_tree& tree,
	const pair<uint64_t, linear_arrangement>& res
)
{
	const linear_arrangement& arr = res.second;
	/* ensure planarity of arrangement */
	if (not lal::linarr::is_planar(tree, arr)) {
		cerr << ERROR << endl;
		cerr << "    The arrangement produced by the library is not an actual" << endl;
		cerr << "    arrangement or is not planar." << endl;
		cerr << "    Arrangement: " << arr << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	/* ensure that value of D is correct */
	const uint64_t D = sum_edge_lengths(tree, arr);
	if (D != res.first) {
		cerr << ERROR << endl;
		cerr << "    Value of D returned by method is incorrect." << endl;
		cerr << "    Arrangement:     " << res.second << endl;
		cerr << "    Inv Arrangement: " << invlinarr(res.second) << endl;
		cerr << "    Value of D returned: " << res.first << endl;
		cerr << "    Actual value of D:   " << D << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	return true;
}

err_type check_tree(const free_tree& T) {
	const auto Dmin_planar_library = lal::linarr::min_sum_edge_lengths_planar(T);

	const auto Dmin_planar_quadratic =
		tests_Dmin_planar::Dmin_planar_quadratic(T);

	// check correctness of library's result
	const bool correct1 = tests_Dmin_planar::check_correctness_arr(T, Dmin_planar_library);
	if (not correct1) { return err_type::test_execution; }

	// check correctness of quadratic's result
	const bool correct2 = tests_Dmin_planar::check_correctness_arr(T, Dmin_planar_quadratic);
	if (not correct2) { return err_type::test_execution; }

	if (Dmin_planar_quadratic.first != Dmin_planar_library.first) {
		cerr << ERROR << endl;
		cerr << "    The library produced a result that does not correspond to" << endl;
		cerr << "    quadratic algorithm's result." << endl;
		cerr << "    Library: " << Dmin_planar_library.first << endl;
		cerr << "        Arrangement: " << Dmin_planar_library.second << endl;
		cerr << "    Quadratic: " << Dmin_planar_quadratic.first << endl;
		cerr << "        Arrangement: " << Dmin_planar_quadratic.second << endl;
		cerr << "    For tree:" << endl;
		cerr << T << endl;
		return err_type::test_execution;
	}
	return err_type::no_error;
}

} // -- namespace tests_Dmin_planar

err_type exe_linarr_Dmin_planar(const input_list& inputs, ifstream& fin) {
	const set<string> allowed_algos({"AEF", "quadratic"});
	const set<string> allowed_quadratic_modes({"exhaustive", "random"});

	string algo;
	fin >> algo;
	if (allowed_algos.find(algo) == allowed_algos.end()) {
		cerr << ERROR << endl;
		cerr << "    Unrecognized algorithm '" << algo << "'." << endl;
		cerr << "    Allowed algorithms:" << endl;
		for (const auto& s : allowed_algos) {
		cerr << "    - " << s << endl;
		}
		return err_type::test_format;
	}

	err_type err = err_type::no_error;

	if (algo == "AEF") {
		if (inputs.size() != 1) {
			cerr << ERROR << endl;
			cerr << "    Exactly one input files are allowed in this test." << endl;
			cerr << "    Instead, " << inputs.size() << " were given." << endl;
			return err_type::test_format;
		}

		ifstream input_file(inputs[0].first);
		if (not input_file.is_open()) {
			cerr << ERROR << endl;
			cerr << "    Input file '" << inputs[0].first << "' could not be opened." << endl;
			return err_type::io;
		}

		err = linarr_brute_force_testing<free_tree>
		(
			[](const free_tree& t) {
				return min_sum_edge_lengths_planar(t);
			},
			[](const free_tree& t, const linear_arrangement& arr) {
				return sum_edge_lengths(t, arr);
			},
			[](const free_tree& t, const linear_arrangement& arr) {
				return lal::linarr::is_planar(t, arr);
			},
			[](const vector<node>& v) {
				return from_head_vector_to_free_tree(v).first;
			},
			[](free_tree&) { },
			input_file
		);
	}
	else if (algo == "quadratic") {
		if (inputs.size() != 0) {
			cerr << ERROR << endl;
			cerr << "    No input files are allowed in this test." << endl;
			cerr << "    Instead, " << inputs.size() << " were given." << endl;
			return err_type::test_format;
		}

		string mode;
		fin >> mode;
		if (allowed_quadratic_modes.find(mode) == allowed_quadratic_modes.end()) {
			cerr << ERROR << endl;
			cerr << "    Unrecognized mode for quadratic algorithm '" << mode << "'." << endl;
			cerr << "    Allowed modes:" << endl;
			for (const auto& s : allowed_quadratic_modes) {
			cerr << "    - " << s << endl;
			}
			return err_type::test_format;
		}

		uint64_t n;
		while (fin >> n) {
			if (mode == "exhaustive") {
				generate::all_ulab_free_trees Gen(n);
				while (not Gen.end()) {
					const auto T = Gen.get_tree();
					Gen.next();
					err = tests_Dmin_planar::check_tree(T);
					if (err != err_type::no_error) {
						return err;
					}
				}
			}
			else if (mode == "random") {
				size_t n_rand_trees;
				fin >> n_rand_trees;

				generate::rand_ulab_free_trees Gen(n, 1234);
				for (size_t i = 0; i < n_rand_trees; ++i) {
					const auto T = Gen.get_tree();
					err = tests_Dmin_planar::check_tree(T);
					if (err != err_type::no_error) {
						return err;
					}
				}
			}
		}
	}

	if (err != err_type::no_error) {
		return err;
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
