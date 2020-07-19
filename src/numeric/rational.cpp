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
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/output.hpp>
using namespace lal;
using namespace numeric;

// custom includes
#include "definitions.hpp"

#define map_has(M, v) (M.find(v) != M.end())

#define assert_exists_variable(varname)										\
	if (not map_has(vtypes, varname)) {										\
		cerr << ERROR << endl;												\
		cerr << "    Variable '" << varname << "' does not exist." << endl;	\
		return err_type::test_exe_error;									\
	}

#define var_type(var)	(vtypes.find(var)->second)
#define message_in_func(f) cerr << "    -- In '" << f << "' --" << endl;
#define assert_correct_var_type(assertion, vt)							\
	if (vt != "integer" and vt != "rational") {							\
		cerr << ERROR << endl;											\
		message_in_func(assertion)										\
		cerr << "    Invalid variable type '" << vt << "'." << endl;	\
		return err_type::test_format_error;								\
	}
#define assert_correct_format(assertion, f)								\
	if (f != "int" and f != "string") {									\
		cerr << ERROR << endl;											\
		message_in_func(assertion)										\
		cerr << "    Invalid format type '" << f << "'." << endl;		\
		return err_type::test_format_error;								\
	}

#define comparison_error(op, var1, var2, val_var1, val_var2)	\
	cerr << ERROR << endl;										\
	cerr << "    Assertion '" << op << "' failed for" << endl;	\
	cerr << "        '" << var1 << "' = " << val_var1 << endl;	\
	cerr << "        '" << var2 << "' = " << val_var2 << endl;

#define get_var_value(M, var) M.find(var)->second

namespace exe_tests {

template<typename U, typename V>
static inline err_type resolve_comp_rational(
	const string& var1, const string& var2,
	const U& val1, const string& op, const V& val2
)
{
	if (op == "==") {
		if (val1 != val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "!=") {
		if (val1 == val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else if (op == ">=") {
		if (val1 < val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else if (op == ">") {
		if (val1 <= val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "<=") {
		if (val1 > val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "<") {
		if (val1 >= val2) {
			comparison_error(op, var1, var2, val1, val2)
			return err_type::test_exe_error;
		}
	}
	else {
		cerr << ERROR << endl;
		message_in_func("comparison " + op)
		cerr << "    Operator is not one of ==, !=, >=, >, <=, <" << endl;
		cerr << "    Operator is: " << op << endl;
		return err_type::test_format_error;
	}
	return err_type::no_error;
}

static inline err_type comp_rational(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	const map<string, rational>& rational_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1, var2;
	fin >> var1 >> op_comp >> var2;

	assert_exists_variable(var1)
	assert_exists_variable(var2)

	integer ival1;
	rational rval1;
	const string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	integer ival2;
	rational rval2;
	const string& type2 = var_type(var2);
	if (type2 == "integer") { ival2 = get_var_value(integer_vars, var2); }
	else { rval2 = get_var_value(rational_vars, var2); }

	if (type1 == "integer" and type2 == "integer") {
		return resolve_comp_rational(var1, var2, ival1, op_comp, ival2);
	}
	if (type1 == "rational" and type2 == "integer") {
		return resolve_comp_rational(var1, var2, rval1, op_comp, ival2);
	}
	if (type1 == "rational" and type2 == "rational") {
		return resolve_comp_rational(var1, var2, rval1, op_comp, rval2);
	}

	cerr << ERROR << endl;
	cerr << "    In operation '" << op_comp << "': combination of variable's types is invalid." << endl;
	cerr << "    Type of variable '" << var1 << "' is: " << type1 << "." << endl;
	cerr << "    Type of variable '" << var2 << "' is: " << type2 << "." << endl;
	return err_type::test_exe_error;
}

static inline err_type comp_rational_lit(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	const map<string, rational>& rational_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1;
	int64_t val2;
	fin >> var1 >> op_comp >> val2;

	assert_exists_variable(var1)

	integer ival1;
	rational rval1;
	const string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	return
	(type1 == "integer" ?
		resolve_comp_rational(var1, "literal", ival1, op_comp, val2) :
		resolve_comp_rational(var1, "literal", rval1, op_comp, val2)
	);
}

template<typename U>
static inline rational resolve_op_rational(
	const U& var1, const string& op, const rational& var2
)
{
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	return -1;
}

template<
	typename U, typename V,
	typename std::enable_if_t<
		std::is_same<V, integer>::value || std::is_integral<V>::value, int
	> = 0
>
static inline rational resolve_op_rational(
	const U& var1, const string& op, const V& var2
)
{
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	if (op == "^") { return var1 ^ var2; }
	return -1;
}

err_type operation(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	map<string, rational>& rational_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1, var2;
	fin >> var0 >> var1 >> op >> var2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)
	assert_exists_variable(var2)

	integer ival1, ival2;
	rational rval1, rval2;

	const string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	const string& type2 = var_type(var2);
	if (type2 == "integer") { ival2 = get_var_value(integer_vars, var2); }
	else { rval2 = get_var_value(rational_vars, var2); }

	rational R;
	if (type1 == "integer" and type2 == "integer") {
		R = resolve_op_rational(ival1, op, ival2);
	}
	else if (type1 == "rational" and type2 == "integer") {
		R = resolve_op_rational(rval1, op, ival2);
	}
	else if (type1 == "rational" and type2 == "rational") {
		R = resolve_op_rational(rval1, op, rval2);
	}
	else if (type1 == "integer" and type2 == "rational") {
		cerr << ERROR << endl;
		cerr << "    In operation '" << op << "': combination of variable's types is invalid." << endl;
		cerr << "    Type of variable '" << var1 << "' is: " << type1 << endl;
		cerr << "    Type of variable '" << var2 << "' is: " << type2 << endl;
		return err_type::test_exe_error;
	}

	rational_vars[var0] = R;
	return err_type::no_error;
}

static inline err_type op_rational_lit(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	map<string, rational>& rational_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1;
	int64_t val2;
	fin >> var0 >> var1 >> op >> val2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)

	integer ival1;
	rational rval1;

	const string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	rational R;
	if (type1 == "integer") {
		R = resolve_op_rational(ival1, op, val2);
	}
	else {
		R = resolve_op_rational(rval1, op, val2);
	}
	rational_vars[var0] = R;
	return err_type::no_error;
}

void test_rational_minutia() {
	{
	integer i1;
	integer i2;
	i1.swap(i2);
	}
	{
	integer i1;
	integer i2;
	i2.swap(i1);
	}

	{
	integer i1 = 0;
	integer i2;
	i1.swap(i2);
	}
	{
	integer i1 = 0;
	integer i2;
	i2.swap(i1);
	}

	{
	integer i1;
	integer i2 = 0;
	i1.swap(i2);
	}
	{
	integer i1;
	integer i2 = 0;
	i2.swap(i1);
	}

	{
	integer i1 = 0;
	integer i2 = 0;
	i1.swap(i2);
	}
	{
	integer i1 = 0;
	integer i2 = 0;
	i2.swap(i1);
	}
}

err_type exe_numeric_rational(const input_list& inputs, ifstream& fin) {
	if (inputs.size() != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << inputs.size() << " were given." << endl;
		return err_type::test_format_error;
	}

	map<string, string> vtypes;
	map<string, integer> integer_vars;
	map<string, rational> rational_vars;

	string command;
	string var_name, var_type;
	string format;
	err_type err;

	while (fin >> command) {
		if (command == "let") {
			fin >> var_name >> var_type >> format;
			assert_correct_var_type("let", var_type)
			assert_correct_format("let", format)

			vtypes[var_name] = var_type;
			if (var_type == "integer") {
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
			else {
				if (format == "int") {
					int64_t val;
					fin >> val;
					rational_vars[var_name] = val;
				}
				else if (format == "string") {
					string val;
					fin >> val;
					rational_vars[var_name] = rational(val);
				}
			}
		}
		else if (command == "assign") {
			fin >> var_name;
			assert_exists_variable(var_name)
			string value;
			fin >> value;
			rational_vars[var_name] = rational(value);
		}
		else if (command == "print") {
			fin >> var_name;
			assert_exists_variable(var_name)
			if (vtypes[var_name] == "rational") {
				cout << var_name << "= " << get_var_value(rational_vars, var_name) << endl;
			}
			else {
				cout << var_name << "= " << get_var_value(integer_vars, var_name) << endl;
			}
		}
		else if (command == "compare") {
			err = comp_rational(vtypes, integer_vars, rational_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "compare_lit") {
			err = comp_rational_lit(vtypes, integer_vars, rational_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "operate") {
			err = operation(vtypes, integer_vars, rational_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "operate_lit") {
			err = op_rational_lit(vtypes, integer_vars, rational_vars, fin);
			if (err != err_type::no_error) { return err; }
		}
		else if (command == "minutia") {
			test_rational_minutia();
		}
		else {
			cerr << ERROR << endl;
			cerr << "    Invalid command '" << command << "'." << endl;
			return err_type::test_format_error;
		}
	}

	TEST_GOODBYE
	return err_type::no_error;
}

} // -- namespace exe_tests
