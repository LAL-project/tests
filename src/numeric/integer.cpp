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
#include <map>
using namespace std;

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/integer_output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "definitions.hpp"

#define map_has(M, v) (M.find(v) != M.end())

#define assert_exists_variable(varname)										\
	if (not map_has(integer_vars, varname)) {								\
		cerr << ERROR << endl;												\
		cerr << "    Variable '" << varname << "' does not exist." << endl;	\
		return err_type::test_execution;									\
	}

#define var_type(var)	(vtypes.find(var)->second)
#define message_in_func(f) cerr << "    -- In '" << f << "' --" << endl;
#define assert_correct_var_type(assertion, vt)							\
	if (vt != "integer") {												\
		cerr << ERROR << endl;											\
		message_in_func(assertion)										\
		cerr << "    Invalid variable type '" << vt << "'." << endl;	\
		return err_type::test_format;								\
	}
#define assert_correct_format(assertion, f)								\
	if (f != "int" and f != "string") {									\
		cerr << ERROR << endl;											\
		message_in_func(assertion)										\
		cerr << "    Invalid format type '" << f << "'." << endl;		\
		return err_type::test_format;								\
	}

#define comparison_error(op, var1, var2, val_var1, val_var2)	\
	cerr << ERROR << endl;										\
	cerr << "    Assertion '" << op << "' failed for" << endl;	\
	cerr << "        '" << var1 << "' = " << val_var1 << endl;	\
	cerr << "        '" << var2 << "' = " << val_var2 << endl;

#define get_var_value(M, var) M.find(var)->second

namespace exe_tests {

template<typename U, typename V>
static inline err_type resolve_comp_integer(
	const string& var1, const string& var2,
	const U& val1, const string& op, const V& val2
)
{
	if (op == "==") {
		if (val1 != val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else if (op == "!=") {
		if (val1 == val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else if (op == ">=") {
		if (val1 < val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else if (op == ">") {
		if (val1 <= val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else if (op == "<=") {
		if (val1 > val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else if (op == "<") {
		if (val1 >= val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_execution;
		}
	}
	else {
		cerr << ERROR << endl;
		message_in_func("comparison " + op)
		cerr << "    Operator is not one of ==, !=, >=, >, <=, <" << endl;
		cerr << "    Operator is: " << op << endl;
		return err_type::test_format;
	}
	return err_type::no_error;
}

static inline err_type comp_integer(
	const map<string, integer>& integer_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1, var2;
	fin >> var1 >> op_comp >> var2;

	assert_exists_variable(var1)
	assert_exists_variable(var2)

	const integer& val1 = get_var_value(integer_vars, var1);
	const integer& val2 = get_var_value(integer_vars, var2);
	return resolve_comp_integer(var1, var2, val1, op_comp, val2);
}

static inline err_type comp_integer_lit(
	const map<string, integer>& integer_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1;
	int64_t val2;
	fin >> var1 >> op_comp >> val2;

	assert_exists_variable(var1)

	const integer& val1 = get_var_value(integer_vars, var1);
	return resolve_comp_integer(var1, "literal", val1, op_comp, val2);
}

template<typename U, typename V>
static inline
integer resolve_integer_operation(const U& var1, const string& op, const V& var2) {
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	// NOTE: var2, when it is an int64_t, its
	// signedness changes into uint64_t.
	if (op == "^") { return var1^var2; }
	return -1;
}

static inline err_type op_integer(
	map<string, integer>& integer_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1, var2;
	fin >> var0 >> var1 >> op >> var2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)
	assert_exists_variable(var2)

	const integer& val1 = get_var_value(integer_vars, var1);
	const integer& val2 = get_var_value(integer_vars, var2);

	const integer R = resolve_integer_operation(val1, op, val2);
	integer_vars[var0] = R;
	return err_type::no_error;
}

static inline err_type op_integer_lit(
	map<string, integer>& integer_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1;
	int64_t val2;
	fin >> var0 >> var1 >> op >> val2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)

	const integer& val1 = get_var_value(integer_vars, var1);

	const integer R = resolve_integer_operation(val1, op, val2);
	integer_vars[var0] = R;
	return err_type::no_error;
}

err_type exe_numeric_integer(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format;
	}

	map<string, integer> integer_vars;

	string command;
	string var_name;
	string format;
	err_type err;

	while (fin >> command) {
		if (command == "let") {
			fin >> var_name >> format;
			assert_correct_format("let", format)

			if (format == "int") {
				int64_t val;
				fin >> val;
				integer_vars[var_name] = val;
			}
			else if (format == "string") {
				string val;
				fin >> val;
				integer_vars[var_name] = integer(val);
			}
		}
		else if (command == "assign") {
			fin >> var_name;
			assert_exists_variable(var_name)
			int64_t value;
			fin >> value;
			integer_vars[var_name] = value;
		}
		else if (command == "print") {
			fin >> var_name;
			assert_exists_variable(var_name)
			cout << var_name << "= " << get_var_value(integer_vars, var_name) << endl;
		}
		else if (command == "compare") {
			err = comp_integer(integer_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "compare_lit") {
			err = comp_integer_lit(integer_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "operate") {
			err = op_integer(integer_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "operate_lit") {
			err = op_integer_lit(integer_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid command '" << command << "'." << endl;
			return err_type::test_format;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
