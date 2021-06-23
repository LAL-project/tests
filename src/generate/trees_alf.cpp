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
#include <lal/generate/all_lab_free_trees.hpp>
#include <lal/graphs/output.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>
#include <lal/properties/degrees.hpp>
using namespace lal;
using namespace graphs;
using namespace generate;
using namespace numeric;

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"
#include "common/tree_validity_check.hpp"

// generate includes
#include "generate/test_exhaustive_enumeration.hpp"

// expected second moment of degree over all labelled trees
inline rational exp_mmt_deg_2_lab_trees(uint64_t n) {
	rational p1 = 1;
	p1 -= rational(1,n);
	rational p2 = 5;
	p2 -= rational(6,n);
	return p1*p2;
}

namespace tests {
namespace generate {

namespace alf {
struct extra_params { };

#define process																\
	const ftree_check err = test_validity_tree(n, T);						\
	if (err != ftree_check::correct) {										\
		cerr << ERROR << endl;												\
		cerr << "    Tree of index " << gen << " is not correct." << endl;	\
		cerr << "    Error: " << ftree_check_to_string(err) << endl;		\
		cerr << T << endl;													\
		return err_type::test_execution;									\
	}																		\
	/* compute 'statistics' */												\
	mmtdeg2 += properties::moment_degree_rational(T, 2);					\
	gen += 1;

#define check																	\
	/* check that the expected second moment of degree is correct */			\
	mmtdeg2 /= gen;																\
	if (mmtdeg2 != exp_mmtdeg2) {												\
		cerr << ERROR << endl;													\
		cerr << "    Calculated 2nd moment of degree: " << mmtdeg2 << endl;		\
		cerr << "    Does not agree with the formula: " << exp_mmtdeg2 << endl;	\
		return err_type::test_execution;										\
	}																			\
	/* Prüfer's formula: make sure that the generator made						\
	   as many trees as n^(n - 2)												\
	  also: https://oeis.org/A000272/list */									\
	const integer total = (n == 1 ? 1 : (nn.pow(nn - 2)));						\
	if (gen != total) {															\
		cerr << ERROR << endl;													\
		cerr << "    Exhaustive generation of labelled free trees" << endl;		\
		cerr << "    Amount of trees should be: " << total << endl;				\
		cerr << "    But generated: " << gen << endl;							\
		cerr << "    For a size of " << n << " vertices" << endl;				\
		return err_type::test_execution;										\
	}

err_type test_for_n_while
(uint64_t n, all_lab_free_trees& TreeGen, const extra_params&)
{
	const integer nn = integer(n);

	// expected second moment of degree
	const rational exp_mmtdeg2 = exp_mmt_deg_2_lab_trees(n);
	rational mmtdeg2 = 0;
	// number of generated trees
	integer gen = 0;

	// generate all trees
	while (not TreeGen.end()) {
		const free_tree T = TreeGen.get_tree();
		TreeGen.next();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_for
(uint64_t n, all_lab_free_trees& TreeGen, const extra_params&)
{
	const integer nn = integer(n);

	// expected second moment of degree
	const rational exp_mmtdeg2 = exp_mmt_deg_2_lab_trees(n);
	rational mmtdeg2 = 0;
	// number of generated trees
	integer gen = 0;

	// generate all trees
	for (; not TreeGen.end(); TreeGen.next()) {
		const free_tree T = TreeGen.get_tree();
		process;
	}
	check;
	return err_type::no_error;
}

err_type test_for_n_yield
(uint64_t n, all_lab_free_trees& TreeGen, const extra_params&)
{
	const integer nn = integer(n);

	// expected second moment of degree
	const rational exp_mmtdeg2 = exp_mmt_deg_2_lab_trees(n);
	rational mmtdeg2 = 0;
	// number of generated trees
	integer gen = 0;

	// generate all trees
	while (not TreeGen.end()) {
		const free_tree T = TreeGen.yield_tree();
		process;
	}
	check;
	return err_type::no_error;
}

} // -- namespace alf

err_type exe_gen_trees_alf(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	// --- do the tests

	uint64_t n;
	while (fin >> n) {
		const auto err1 =
			test_exhaustive_enumeration_of_trees<all_lab_free_trees>
			(n, alf::test_for_n_while, alf::extra_params{});
		if (err1 != err_type::no_error) { return err1; }

		const auto err2 =
			test_exhaustive_enumeration_of_trees<all_lab_free_trees>
			(n, alf::test_for_n_for, alf::extra_params{});
		if (err2 != err_type::no_error) { return err2; }

		const auto err3 =
			test_exhaustive_enumeration_of_trees<all_lab_free_trees>
			(n, alf::test_for_n_yield, alf::extra_params{});
		if (err3 != err_type::no_error) { return err3; }
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace generate
} // -- namespace tests
