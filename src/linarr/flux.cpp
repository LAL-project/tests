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
#include <fstream>
using namespace std;

// lal includes
#include <lal/linarr/dependency_flux.hpp>
#include <lal/linarr/flux.hpp>
#include <lal/internal/graphs/trees/convert_to_ftree.hpp>
#include <lal/graphs/output.hpp>
using namespace lal;
using namespace graphs;

// custom includes
#include "definitions.hpp"

#define test_integral_field(FIELD, field_str)									\
	for (size_t i = 0; i < S; ++i) {											\
		if (input_flux[i].FIELD() != algo_flux[i].FIELD()) {					\
			cerr << ERROR << endl;												\
			cerr << "    " << field_str << " do not agree at: " << i << endl;	\
			cerr << "    Input:     " << input_flux[i].FIELD() << endl;			\
			cerr << "    Algorithm: " << algo_flux[i].FIELD() << endl;			\
			cerr << "    At tree:   " << tree_idx << endl;						\
			return err_type::test_execution;									\
		}																		\
	}

#define hidden_stringize(x) #x
#define stringize(x) hidden_stringize(x)

#define parse_integral_field(FIELD)	\
	for (auto& v : input_flux) { fin >> v.FIELD(); }

namespace exe_tests {

namespace test_flux {
inline ostream& operator<< (ostream& out, const edge& e) {
	out << "(" << e.first << "," << e.second << ")";
	return out;
}

inline
ostream& operator<< (ostream& out, const vector<edge>& deps) {
	if (deps.size() > 0) {
		out << deps[0];
		for (size_t i = 1; i < deps.size(); ++i) {
			out << " " << deps[i];
		}
	}
	return out;
}
} // -- namespace test_flux
using namespace test_flux;

err_type exe_linarr_dependency_flux(const input_list& inputs,ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	string field;
	free_tree T;

	vector<linarr::dependency_flux> algo_flux;
	vector<linarr::dependency_flux> input_flux;
	size_t S = 0;
	size_t tree_idx = 0;

	while (fin >> field) {
		if (field == "tree") {
			++tree_idx;
			uint32_t n;
			fin >> n;
#if defined DEBUG
			assert(n > 0);
#endif

			vector<uint32_t> linear_sequence(n);
			for (auto& v : linear_sequence) { fin >> v; }

			T.clear();
			T = internal::linear_sequence_to_ftree(linear_sequence).first;

			algo_flux = linarr::compute_flux(T);
			input_flux.resize(n - 1);
			S = n - 1;
		}
		else if (field == "left_span") {
			parse_integral_field(get_left_span);
			test_integral_field(get_left_span, "Left spans");
		}
		else if (field == "right_span") {
			parse_integral_field(get_right_span);
			test_integral_field(get_right_span, "Right spans");
		}
		else if (field == "dependencies") {
			for (auto& v : input_flux) {
				uint32_t k;
				fin >> k;
				v.get_dependencies().resize(k);
				auto& deps = v.get_dependencies();
				for (uint32_t i = 0; i < k; ++i) {
					fin >> deps[i].first >> deps[i].second;
				}
			}

			// test dependencies
			for (size_t i = 0; i < S; ++i) {
				auto ideps = input_flux[i].get_dependencies();
				auto adeps = algo_flux[i].get_dependencies();

				std::sort(ideps.begin(), ideps.end());
				std::sort(adeps.begin(), adeps.end());
				if (ideps != adeps) {
					cerr << ERROR << endl;
					cerr << "    Dependencies do not agree at: " << i << endl;
					cerr << "    Input:     " << ideps << endl;
					cerr << "    Algorithm: " << adeps << endl;
					cerr << "    At tree:   " << tree_idx << endl;
					return err_type::test_execution;
				}
			}
		}
		else if (field == "weight") {
			parse_integral_field(get_weight);
			test_integral_field(get_weight, "Weights");
		}
		else if (field == "is_left_bouquet") {
			parse_integral_field(is_left_bouquet);
			test_integral_field(is_left_bouquet, "Left bouquets");
		}
		else if (field == "is_right_bouquet") {
			parse_integral_field(is_right_bouquet);
			test_integral_field(is_right_bouquet, "Right bouquets");
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Input field/option not valid." << endl;
			cerr << "    Received: '" << field << "'." << endl;
			return err_type::test_format;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
