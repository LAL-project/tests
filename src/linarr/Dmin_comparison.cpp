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
#include <algorithm>
#include <iostream>
#include <numeric>
#include <fstream>
#include <cassert>
#include <queue>
#include <set>
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/linarr/D.hpp>
#include <lal/linarr/Dmin.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/io/basic_output.hpp>
#include <lal/internal/std_utils.hpp>
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>
using namespace lal;
using namespace graphs;
using namespace linarr;
using namespace numeric;

// custom includes
#include "definitions.hpp"
#include "arrgmnt_validity_check.hpp"
#include "test_utils.hpp"
#include "tree_classification.hpp"

typedef pair<uint32_t, linear_arrangement> algo_result;

bool lt(const algo_result& r1, const algo_result& r2) { return r1.first < r2.first; }
bool le(const algo_result& r1, const algo_result& r2) { return r1.first <= r2.first; }
bool eq(const algo_result& r1, const algo_result& r2) { return r1.first == r2.first; }
bool ge(const algo_result& r1, const algo_result& r2) { return r1.first >= r2.first; }
bool gt(const algo_result& r1, const algo_result& r2) { return r1.first > r2.first; }

namespace exe_tests {

inline
bool check_correctness_arr(const free_tree& tree, const pair<uint32_t, linear_arrangement>& res) {
	const linear_arrangement& arr = res.second;
	/* ensure that the result is an arrangement */
	if (not is_arrangement(arr)) {
		cerr << ERROR << endl;
		cerr << "    The result is not an arrangement (permutation)." << endl;
		cerr << "    Arrangement: " << arr << endl;
		cerr << "    For tree: " << endl;
		cerr << tree << endl;
		return false;
	}
	/* ensure that value of D is correct */
	const uint32_t D = sum_length_edges(tree, arr);
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

// -----------------------------------------------------------------------------

#define correct_algo_str(algo)												\
{																			\
	if (allowed_algos.find(algo) == allowed_algos.end()) {					\
		cerr << ERROR << endl;												\
		cerr << "    Algorithm name '" << algo << "' is invalid." << endl;	\
		cerr << "    Must be one of:" << endl;								\
		for (const string& s : allowed_algos) {								\
		cerr << "        " << s << endl;									\
		}																	\
		return err_type::test_format;									\
	}																		\
}

#define correct_comp_str(comp)												\
{																			\
	if (allowed_comps.find(comp) == allowed_comps.end()) {					\
		cerr << ERROR << endl;												\
		cerr << "    Comparison '" << comp << "' is invalid." << endl;	\
		cerr << "    Must be one of:" << endl;								\
		for (const string& s : allowed_comps) {								\
		cerr << "        " << s << endl;									\
		}																	\
		return err_type::test_format;									\
	}																		\
}


err_type exe_linarr_Dmin_comparison(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	const set<string> allowed_algos({"Proj", "Plan", "YS", "FC"});
	const auto Proj = [](const free_tree& t) -> algo_result { return Dmin(t, algorithms_Dmin::Projective); };
	const auto Plan = [](const free_tree& t) -> algo_result { return Dmin(t, algorithms_Dmin::Planar); };
	const auto FC = [](const free_tree& t) -> algo_result { return Dmin(t, algorithms_Dmin::Unconstrained_FC); };
	const auto YS = [](const free_tree& t) -> algo_result { return Dmin(t, algorithms_Dmin::Unconstrained_YS); };
	map<string, function<algo_result (const free_tree&)> > ALGOS;
	ALGOS["Proj"] = Proj;
	ALGOS["Plan"] = Plan;
	ALGOS["YS"] = YS;
	ALGOS["FC"] = FC;

	const set<string> allowed_comps({"<", "<=", "==", ">=", ">"});
	map<string, function<bool (const algo_result&, const algo_result&)>> COMPS;
	COMPS["<"] = lt;
	COMPS["<="] = le;
	COMPS["=="] = eq;
	COMPS[">="] = ge;
	COMPS[">"] = gt;

	string mode;
	while (fin >> mode) {
		if (mode != "exhaustive" and mode != "random") {
			cerr << ERROR << endl;
			cerr << "    Mode '" << mode << "' is invalid." << endl;
			cerr << "    Must be either 'exhaustive' or 'random'." << endl;
			return err_type::test_format;
		}

		string algo1, comp, algo2;
		fin >> algo1 >> comp >> algo2;

		correct_algo_str(algo1);
		correct_comp_str(comp);
		correct_algo_str(algo2);

		uint32_t n;
		fin >> n;

		cout << "Testing '" << mode << "' for "
			 << "'" << algo1 << "' " << comp << " '" << algo2 << "'"
			 << " on size: " << n;

		if (mode == "exhaustive") {
			cout << endl;

			generate::all_ulab_free_trees TreeGen(n);
			while (TreeGen.has_next()) {
				TreeGen.next();
				const free_tree tree = TreeGen.get_tree();

				const auto res1 = ALGOS[algo1](tree);
				const auto res2 = ALGOS[algo2](tree);

				check_correctness_arr(tree, res1);
				check_correctness_arr(tree, res2);

				if (not COMPS[comp](res1, res2)) {
					cerr << ERROR << endl;
					cerr << "    Result of algorithm '" << algo1 << "' is not "
						 << "'" << comp << "'"
						 << " with respect to the result of algorithm "
						 << "'" << algo2 << "'."
						 << endl;
					cerr << "    Algorithm: " << algo1 << endl;
					cerr << "        D= " << res1.first << endl;
					cerr << "        Arrangement: " << res1.second << endl;
					cerr << "    Algorithm: " << algo2 << endl;
					cerr << "        D= " << res2.first << endl;
					cerr << "        Arrangement: " << res2.second << endl;
					cerr << " In tree:" << endl;
					cerr << tree << endl;
					return err_type::test_execution;
				}
			}
		}
		else {
			uint32_t N;
			fin >> N;
			cout << " (N= " << N << ")" << endl;

			generate::rand_ulab_free_trees TreeGen(n);
			for (uint32_t i = 0; i < N; ++i) {
				const free_tree tree = TreeGen.make_rand_tree();

				const auto res1 = ALGOS[algo1](tree);
				const auto res2 = ALGOS[algo2](tree);

				check_correctness_arr(tree, res1);
				check_correctness_arr(tree, res2);

				if (not COMPS[comp](res1, res2)) {
					cerr << ERROR << endl;
					cerr << "    Result of algorithm '" << algo1 << "' is not "
						 << "'" << comp << "'"
						 << " with respect to the result of algorithm "
						 << "'" << algo2 << "'."
						 << endl;
					cerr << "    Algorithm: " << algo1 << endl;
					cerr << "        D= " << res1.first << endl;
					cerr << "        Arrangement: " << res1.second << endl;
					cerr << "    Algorithm: " << algo2 << endl;
					cerr << "        D= " << res2.first << endl;
					cerr << "        Arrangement: " << res2.second << endl;
					cerr << " In tree:" << endl;
					cerr << tree << endl;
					return err_type::test_execution;
				}
			}
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
