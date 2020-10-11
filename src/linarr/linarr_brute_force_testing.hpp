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
using namespace lal;

// custom includes
#include "definitions.hpp"
#include "test_utils.hpp"
#include "std_utils.hpp"

namespace exe_tests {

template<class T> using Solver =
	function< pair<uint32_t, linear_arrangement>(const T&) >;

template<class T> using TreeEval =
	function< uint32_t (const T&, const linear_arrangement&) >;

template<class T> using ArrgmtCheck =
	function< string (const T&, const linear_arrangement&) >;

template<class T> using InputConv =
	function< T (const vector<node>&) >;

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

	vector<uint32_t> node_list(n);
	while (fin >> node_list[0]) {
		for (uint32_t i = 1; i < n; ++i) {
			fin >> node_list[i];
		}

		// construct tree
		T tree = conv(node_list);
		tree_initializer(tree);

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
			linear_arrangement input_arr(n);
			for (uint32_t i = 0; i < n; ++i) {
				fin >> input_arr[i];
			}

			const uint32_t check_value = tree_eval(tree, input_arr);
			// check correctness of input array
			if (check_value != brute_force_value) {
				cerr << ERROR << endl;
				cerr << "    Input value calculated by brute force does not" << endl;
				cerr << "    agree with the evaluation of the tree." << endl;
				cerr << "        BF Arrangement:     " << input_arr << endl;
				cerr << "        BF Inv Arrangement: " << invlinarr(input_arr) << endl;
				cerr << "        BF value: " << brute_force_value << endl;
				cerr << "        Evaluation: " << check_value << endl;
				return err_type::test_format;
			}
		}
		}

		// execute library's algorithm
		const auto library_res = solver(tree);
		const linear_arrangement& library_arr = library_res.second;

		// ensure that the arrangement is a planar permutation
		{
		const string err = arrgmnt_check(tree, library_arr);
		if (err != "") {
			cerr << ERROR << endl;
			cerr << "    The arrangement produced is not correct." << endl;
			cerr << "    Error: '" << err << "'" << endl;
			cerr << "        Arrangement:     " << library_arr << endl;
			cerr << "        Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_execution;
		}
		}

		// ensure that value of D matches the evaluation of the arrangement
		{
		const uint32_t check_value = tree_eval(tree, library_arr);
		if (check_value != library_res.first) {
			cerr << ERROR << endl;
			cerr << "    Input value calculated by brute force does not" << endl;
			cerr << "    agree with the evaluation of the tree." << endl;
			cerr << "        BF Arrangement:     " << library_arr << endl;
			cerr << "        BF Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "        BF value: " << brute_force_value << endl;
			cerr << "        Evaluation: " << check_value << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_execution;
		}
		}

		// ensure that the value of D is actually minimum
		if (library_res.first != brute_force_value) {
			cerr << ERROR << endl;
			cerr << "    The value calculated by the library and the brute force do not agree." << endl;
			cerr << "    Library:" << endl;
			cerr << "        Value: " << library_res.first << endl;
			cerr << "        Arrangement:     " << library_res.second << endl;
			cerr << "        Inv Arrangement: " << invlinarr(library_res.second) << endl;
			cerr << "    Bruteforce:" << endl;
			cerr << "        Value: " << brute_force_value << endl;
			cerr << "        BF Arrangement:     " << library_arr << endl;
			cerr << "        BF Inv Arrangement: " << invlinarr(library_arr) << endl;
			cerr << "    For tree: " << endl;
			cerr << tree << endl;
			return err_type::test_execution;
		}
	}
	return err_type::no_error;
}

} // -- namespace exe_tests
