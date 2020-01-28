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
#include "../definitions.hpp"

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
	if (vt != "int" and vt != "integer") {								\
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

#define comparison_error(op, var1, var2, val_var1, val_var2)				\
	cerr << ERROR << endl;													\
	cerr << "    Assertion '" << op << "' failed for" << endl;	\
	cerr << "        '" << var1 << "' = " << val_var1 << endl;	\
	cerr << "        '" << var2 << "' = " << val_var2 << endl;

#define get_var_value(M, var) M.find(var)->second

namespace exe_tests {

template<typename U, typename V>
static inline
err_type resolve_comparison(
	const string& varname1, const string& varname2,
	const U& var1, const string& op, const V& var2
)
{
	if (op == "==") {
		if (var1 != var2) {
			comparison_error(op, varname1, varname2, var1, var2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "!=") {
		if (var1 == var2) {
			comparison_error(op, varname1, varname2, var1, var2)
			return err_type::test_exe_error;
		}
	}
	else if (op == ">=") {
		if (var1 < var2) {
			comparison_error(op, varname1, varname2, var1, var2)
			return err_type::test_exe_error;
		}
	}
	else if (op == ">") {
		if (var1 <= var2) {
			comparison_error(op, varname1, varname2, var1, var2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "<=") {
		if (var1 > var2) {
			comparison_error(op, varname1, varname2, var1, var2)
			return err_type::test_exe_error;
		}
	}
	else if (op == "<") {
		if (var1 >= var2) {
			comparison_error(op, varname1, varname2, var1, var2)
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

err_type comparison(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	const map<string, int64_t>& int64_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1, var2;
	fin >> var1 >> op_comp >> var2;

	assert_exists_variable(var1)
	assert_exists_variable(var2)

	const string& tvar1 = var_type(var1);
	const string& tvar2 = var_type(var2);

	int64_t sval_var1;
	integer val_var1;
	if (tvar1 == "int") {
		sval_var1 = int64_vars.find(var1)->second;
	}
	else if (tvar1 == "integer") {
		val_var1 = integer_vars.find(var1)->second;
	}

	int64_t sval_var2;
	integer val_var2;
	if (tvar2 == "int") {
		sval_var2 = int64_vars.find(var2)->second;
	}
	else if (tvar2 == "integer") {
		val_var2 = integer_vars.find(var2)->second;
	}

	if (tvar1 == "int" and tvar2 == "int") {
		return resolve_comparison(var1, var2, sval_var1, op_comp, sval_var2);
	}
	if (tvar1 == "integer" and tvar2 == "int") {
		return resolve_comparison(var1, var2, val_var1, op_comp, sval_var2);
	}
	return resolve_comparison(var1, var2, val_var1, op_comp, val_var2);
}

err_type comparison_lit(
	const map<string, string>& vtypes,
	const map<string, integer>& integer_vars,
	const map<string, int64_t>& int64_vars,
	ifstream& fin
)
{
	string op_comp;
	string var1;
	int64_t val2;
	fin >> var1 >> op_comp >> val2;

	assert_exists_variable(var1)
	if (var_type(var1) == "int") {
		int64_t val_var1 = get_var_value(int64_vars, var1);
		return resolve_comparison(var1, "literal", val_var1, op_comp, val2);
	}

	const integer& val_var1 = get_var_value(integer_vars, var1);
	return resolve_comparison(var1, "literal", val_var1, op_comp, val2);
}

template<typename U, typename V>
static inline
integer resolve_operation(const U& var1, const string& op, const V& var2) {
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	// NOTE: var2, when it is an int64_t, its
	// signedness changes into uint64_t.
	if (op == "^") { return var1^var2; }
	return -1;
}

err_type operation(
	const map<string, string>& vtypes,
	map<string, integer>& integer_vars,
	const map<string, int64_t>& int64_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1, var2;
	fin >> var0 >> var1 >> op >> var2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)
	assert_exists_variable(var2)

	const string& tvar0 = var_type(var0);
	const string& tvar1 = var_type(var1);
	const string& tvar2 = var_type(var2);

	if (tvar0 != "integer") {
		cerr << ERROR << endl;
		message_in_func("operation " + op)
		cerr << "    Variable 0's type is not integer." << endl;
		cerr << "    The type is: " << tvar0 << endl;
		return err_type::test_exe_error;
	}

	int64_t sval_var1;
	integer val_var1;
	if (tvar1 == "int") {
		sval_var1 = int64_vars.find(var1)->second;
	}
	else if (tvar1 == "integer") {
		val_var1 = integer_vars.find(var1)->second;
	}

	int64_t sval_var2;
	integer val_var2;
	if (tvar2 == "int") {
		sval_var2 = int64_vars.find(var2)->second;
	}
	else if (tvar2 == "integer") {
		val_var2 = integer_vars.find(var2)->second;
	}

	integer R;
	if (tvar1 == "int" and tvar2 == "int") {
		R = resolve_operation(sval_var1, op, sval_var2);
	}
	else if (tvar1 == "integer" and tvar2 == "int") {
		R = resolve_operation(val_var1, op, sval_var2);
	}
	else  {
		R = resolve_operation(val_var1, op, val_var2);
	}
	integer_vars[var0] = R;
	return err_type::no_error;
}

err_type operation_lit(
	const map<string, string>& vtypes,
	map<string, integer>& integer_vars,
	const map<string, int64_t>& int64_vars,
	ifstream& fin
)
{
	string op;
	string var0, var1;
	int64_t val_var2;
	fin >> var0 >> var1 >> op >> val_var2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)

	const string& tvar0 = var_type(var0);
	const string& tvar1 = var_type(var1);

	if (tvar0 != "integer") {
		cerr << ERROR << endl;
		message_in_func("operation " + op)
		cerr << "    Variable 0's type is not integer." << endl;
		cerr << "    The type is: " << tvar0 << endl;
		return err_type::test_exe_error;
	}

	int64_t sval_var1;
	integer val_var1;
	if (tvar1 == "int") {
		sval_var1 = int64_vars.find(var1)->second;
	}
	else if (tvar1 == "integer") {
		val_var1 = integer_vars.find(var1)->second;
	}

	integer R;
	if (tvar1 == "int") {
		R = resolve_operation(sval_var1, op, val_var2);
	}
	else {
		R = resolve_operation(val_var1, op, val_var2);
	}
	integer_vars[var0] = R;
	return err_type::no_error;
}

err_type exe_numeric_integer(ifstream& fin) {
	string field;
	size_t n;

	fin >> field;
	if (field != "INPUT") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'INPUT'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}
	fin >> n;
	if (n != 0) {
		cerr << ERROR << endl;
		cerr << "    No input files are allowed in this test." << endl;
		cerr << "    Instead, " << n << " were specified." << endl;
		return err_type::test_format_error;
	}
	// parse body field
	fin >> field;
	if (field != "BODY") {
		cerr << ERROR << endl;
		cerr << "    Expected field 'BODY'." << endl;
		cerr << "    Instead, '" << field << "' was found." << endl;
		return err_type::test_format_error;
	}

	map<string, string> vtypes;
	map<string, integer> integer_vars;
	map<string, int64_t> int64_vars;

	string command;
	string var_name;
	string var_type;
	string format;

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
					int64_vars[var_name] = val;
				}
				else if (format == "string") {
					string val;
					fin >> val;
					int64_vars[var_name] = atoi(val.c_str());
				}
			}
		}
		else if (command == "assign") {
			fin >> var_name;
			assert_exists_variable(var_name)
			int64_t value;
			fin >> value;
			if (var_type(var_name) == "integer") {
				integer_vars[var_name] = value;
			}
			else if (var_type(var_name) == "int") {
				int64_vars[var_name] = value;
			}
		}
		else if (command == "print") {
			fin >> var_name;
			assert_exists_variable(var_name)
			if (var_type(var_name) == "integer") {
				cout << var_name << "= " << get_var_value(integer_vars, var_name) << endl;
			}
			else if (var_type(var_name) == "int") {
				cout << var_name << "= " << get_var_value(int64_vars, var_name) << endl;
			}
		}
		else if (command == "compare") {
			comparison(vtypes, integer_vars, int64_vars, fin);
		}
		else if (command == "compare_lit") {
			comparison_lit(vtypes, integer_vars, int64_vars, fin);
		}
		else if (command == "operate") {
			operation(vtypes, integer_vars, int64_vars, fin);
		}
		else if (command == "operate_lit") {
			operation_lit(vtypes, integer_vars, int64_vars, fin);
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
