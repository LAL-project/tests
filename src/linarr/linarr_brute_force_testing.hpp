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

#pragma once

// C++ includes
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/output.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/test_utils.hpp"
#include "common/std_utils.hpp"

namespace tests {
namespace linarr {

template<class T> using Solver =
	function< pair<uint32_t, lal::linear_arrangement>(const T&) >;

template<class T> using TreeEval =
	function< uint32_t (const T&, const lal::linear_arrangement&) >;

template<class T> using ArrgmtCheck =
	function< bool (const T&, const lal::linear_arrangement&) >;

template<class T> using InputConv =
	function< T (const vector<lal::node>&) >;

template<class T> using TreeInit =
	function< void (T&) >;

template<class T>
err_type linarr_brute_force_testing(
	// function that computes the solution
	const Solver<T>& solver,
	// function that evaluates the input tree on a given arrangement
	const TreeEval<T>& tree_eval,
	// function that tests the correctness of the resulting arrangement
	const ArrgmtCheck<T>& arrgmnt_check,
	// function that converts the input data into a tree
	const InputConv<T>& conv,
	// function that initializes the input tree
	const TreeInit<T>& tree_initializer,
	// the input stream
	ifstream& fin
)
{
	// read number of nodes
	uint32_t n;
	fin >> n;

	if (n == 1) {
		// nothing to do
		return err_type::no_error;
	}

	lal::head_vector tree_as_head_vector(n);
	while (fin >> tree_as_head_vector[0]) {
		// read head vector
		for (uint32_t i = 1; i < n; ++i) {
			fin >> tree_as_head_vector[i];
		}

		// construct tree
		T tree = conv(tree_as_head_vector);
		tree_initializer(tree);
		if constexpr (std::is_base_of_v<lal::graphs::rooted_tree, T>) {
#if defined DEBUG
			assert(tree.find_edge_orientation());
#endif
		}

		// read value of D calculated by brute force
		uint32_t brute_force_value;
		fin >> brute_force_value;

		// read input arrays and test their correctness
		{
		uint32_t n_classes;
		fin >> n_classes;
		for (uint32_t c = 0; c < n_classes; ++c) {
			uint32_t mult; fin >> mult;	// multiplicity
			char star; fin >> star;		// *
			// arrangement
			lal::linear_arrangement input_arr(n);
			for (uint32_t i = 0; i < n; ++i) {
				fin >> input_arr[i];
			}

			const uint32_t check_value = tree_eval(tree, input_arr);
			// check correctness of input array
			if (check_value != brute_force_value) {
				cerr << ERROR << endl;
				cerr << "    Input value (calculated by brute force) does not" << endl;
				cerr << "    agree with the evaluation of the tree at the arrangement" << endl;
				cerr << "    calculated by brute force." << endl;
				cerr << "        Brute force arrangement:     " << input_arr << endl;
				cerr << "        Brute force Inv Arrangement: " << invlinarr(input_arr) << endl;
				cerr << "        Brute force value:           " << brute_force_value << endl;
				cerr << "        Evaluation at arrangement:   " << check_value << endl;
				return err_type::test_format;
			}
		}
		}

		// execute library's algorithm
		const auto library_res = solver(tree);
		const lal::linear_arrangement& library_arr = library_res.second;

		// ensure that the arrangement is a planar permutation
		{
		if (not arrgmnt_check(tree, library_arr)) {
			cerr << ERROR << endl;
			cerr << "    The arrangement produced by the algorithm is not correct." << endl;
			cerr << "        Arrangement:     " << library_arr << endl;
			cerr << "        Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			cerr << tree.get_head_vector() << endl;
			return err_type::test_execution;
		}
		}

		// ensure that value of D matches the evaluation of the arrangement
		{
		const uint32_t check_value = tree_eval(tree, library_arr);
		if (check_value != library_res.first) {
			cerr << ERROR << endl;
			cerr << "    The value calculated by the library's algorithm does not" << endl;
			cerr << "    agree with the evaluation of the tree at the arrangement" << endl;
			cerr << "    that the library's algorithm calculated." << endl;
			cerr << "        Algorithm's Arrangement:     " << library_arr << endl;
			cerr << "        Algorithm's Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "        Algorithm's value:           " << library_res.first << endl;
			cerr << "        Evaluation at arrangement:   " << check_value << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_execution;
		}
		}

		// ensure that the value of D is actually minimum
		if (library_res.first != brute_force_value) {
			cerr << ERROR << endl;
			cerr << "    The value calculated by the library and by bruteforce do not agree." << endl;
			cerr << "    Library:" << endl;
			cerr << "        Value:           " << library_res.first << endl;
			cerr << "        Arrangement:     " << library_res.second << endl;
			cerr << "        Inv Arrangement: " << invlinarr(library_res.second) << endl;
			cerr << "    bruteforce:" << endl;
			cerr << "        Value:           " << brute_force_value << endl;
			cerr << "        Arrangement:     " << library_arr << endl;
			cerr << "        Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
