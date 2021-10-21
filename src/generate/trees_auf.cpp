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
using namespace std;

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
#include <lal/utilities/tree_isomorphism.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

// generate includes
#include "generate/test_exhaustive_tree_enumeration.hpp"
#include "common/tree_validity_check.hpp"

// number of caterpillar trees of a given size
inline integer num_caterpillar_trees(uint64_t n) noexcept {
	if (n == 1) { return 1; }
	if (n == 2) { return 1; }
	if (n == 3) { return 1; }
	if (n == 4) { return 2; }
	integer n1 = 2;
	n1.powt(n - 4);
	integer n2 = 2;
	n2.powt((n - 4)/2);
	n1 += n2;
	return n1;
}

namespace tests {
namespace generate {

namespace auf {
struct extra_params {
	vector<integer> UFT;
};

#define process																\
	const ftree_check err = test_validity_tree(n, T);						\
	if (err != ftree_check::correct) {										\
		cerr << ERROR << endl;												\
		cerr << "    Tree of index " << gen << " is not correct." << endl;	\
		cerr << "    Error: " << tree_check_to_string(err) << endl;			\
		cerr << T << endl;													\
		return err_type::test_execution;									\
	}																		\
	/* compute 'statistics' */												\
	n_caterpillar += T.is_of_tree_type(tree_type::caterpillar);				\
	gen += 1;																\
	/* ensure uniqueness */													\
	for (size_t j = 0; j < it; ++j) {										\
		if (utilities::are_trees_isomorphic(all_free_trees[j], T)) {		\
			cerr << ERROR << endl;											\
			cerr << "    Found two isomorphic trees!." << endl;				\
			cerr << "    After generating the " << it << " tree." << endl;	\
			cerr << "    The isomorphic tree is at j= " << j << endl;		\
			cerr << "    Just generated:" << endl;							\
			cerr << T << endl;												\
			cerr << "   The tree at position j= " << j << ":" << endl;		\
			cerr << all_free_trees[j] << endl;								\
			return err_type::test_execution;								\
		}																	\
	}																		\
	all_free_trees[it] = std::move(T);										\
	++it;

#define check																							\
	/* check the number of caterpillar trees is correct */												\
	const integer n_cat = num_caterpillar_trees(n);														\
	if (n_cat != n_caterpillar) {																		\
		cerr << ERROR << endl;																			\
		cerr << "    Number of caterpillar trees detected does not agree with the formula." << endl;	\
		cerr << "    Number of vertices: " << n << endl;												\
		cerr << "    Formula:  " << n_cat << endl;														\
		cerr << "    Detected: " << n_caterpillar << endl;												\
		return err_type::test_execution;																\
	}																									\
	/* make sure that the amount of trees generate coincides with the series from the OEIS */			\
	if (n < UFT.size() and gen != UFT[n]) {																\
		cerr << ERROR << endl;																			\
		cerr << "    Exhaustive generation of unlabelled free trees" << endl;							\
		cerr << "    Amount of trees should be: " << UFT[n] << endl;									\
		cerr << "    But generated: " << gen << endl;													\
		cerr << "    For a size of " << n << " vertices" << endl;										\
		return err_type::test_execution;																\
	}

err_type test_for_n_while
(uint64_t n, all_ulab_free_trees& TreeGen, const extra_params& params)
{
	const auto& UFT = params.UFT;

	// number of caterpillar trees
	integer n_caterpillar = 0;
	// number of generated trees
	integer gen = 0;

	// store the trees so we can test for uniqueness
	detail::data_array<free_tree> all_free_trees(UFT[n].to_uint());
	size_t it = 0;

	while (not TreeGen.end()) {
		free_tree T = TreeGen.get_tree();
		TreeGen.next();
		process;
	}

	check;
	return err_type::no_error;
}

err_type test_for_n_for
(uint64_t n, all_ulab_free_trees& TreeGen, const extra_params& params)
{
	const auto& UFT = params.UFT;

	// number of caterpillar trees
	integer n_caterpillar = 0;
	// number of generated trees
	integer gen = 0;

	// store the trees so we can test for uniqueness
	detail::data_array<free_tree> all_free_trees(UFT[n].to_uint());
	size_t it = 0;

	for (; not TreeGen.end(); TreeGen.next()) {
		free_tree T = TreeGen.get_tree();
		process;
	}

	check;
	return err_type::no_error;
}

err_type test_for_n_yield
(uint64_t n, all_ulab_free_trees& TreeGen, const extra_params& params)
{
	const auto& UFT = params.UFT;

	// number of caterpillar trees
	integer n_caterpillar = 0;
	// number of generated trees
	integer gen = 0;

	// store the trees so we can test for uniqueness
	detail::data_array<free_tree> all_free_trees(UFT[n].to_uint());
	size_t it = 0;

	while (not TreeGen.end()) {
		free_tree T = TreeGen.yield_tree();
		process;
	}

	check;
	return err_type::no_error;
}

template<bool init>
err_type call_test_exhaustive(uint64_t n1, uint64_t n2, const extra_params& ep) noexcept {
	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, all_ulab_free_trees>
		(n1, n2, test_for_n_while, ep);
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, all_ulab_free_trees>
		(n1, n2, test_for_n_for, ep);
	if (err != err_type::no_error) { return err; }
	}

	{
	const auto err =
		test_exhaustive_enumeration_of_trees<init, all_ulab_free_trees>
		(n1, n2, test_for_n_yield, ep);
	if (err != err_type::no_error) { return err; }
	}
	return err_type::no_error;
}


} // -- namespace auf

err_type exe_gen_trees_auf(const input_list& inputs, ifstream& fin) {

	/* BUILD TESTING DATA */

	// from: http://oeis.org/A000055/list
	// amount of unlabelled free trees
	auf::extra_params params;
	// size of the vector with the number of unlabelled free trees

	params.UFT =
	vector<integer>{
		1,
		1,
		1,
		1,
		2,
		3,
		6,
		11,
		23,
		47,
		106,
		235,
		551,
		1301,
		3159,
		7741,
		19320,
		48629,
		123867,
		317955,
		823065,
		2144505,
		5623756,
		14828074,
		39299897,
		integer("104636890"),
		integer("279793450"),
		integer("751065460"),
		integer("2023443032"),
		integer("5469566585"),
		integer("14830871802"),
		integer("40330829030"),
		integer("109972410221"),
		integer("300628862480"),
		integer("823779631721"),
		integer("2262366343746"),
		integer("6226306037178")
	};

	// -------------------------------------------------------------------------

	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	// --- do the tests

	uint64_t n1, n2;
	while (fin >> n1 >> n2) {
		{
		const auto err = auf::call_test_exhaustive<true>(n1, n2, params);
		if (err != err_type::no_error) { return err; }
		}

		{
		const auto err = auf::call_test_exhaustive<false>(n1, n2, params);
		if (err != err_type::no_error) { return err; }
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
