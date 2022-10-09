/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Tests of the Linear Arrangement Library. The full code
 *  is available at:
 *      https://github.com/LAL-project/tests.git
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
#if defined DEBUG
#include <cassert>
#endif
#include <iostream>
#include <fstream>
#include <map>

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/numeric/output.hpp>

// common includes
#include "common/definitions.hpp"

#define map_has(M, v) (M.find(v) != M.end())

#define assert_exists_variable(varname)										\
	if (not map_has(vtypes, varname)) {										\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Variable '" << varname << "' does not exist.\n";	\
		return err_type::test_execution;									\
	}

#define var_type(var)	(vtypes.find(var)->second)
#define message_in_func(f) std::cerr << "    -- In '" << f << "' --\n";
#define assert_correct_var_type(assertion, vt)						\
	if (vt != "integer" and vt != "rational") {						\
		std::cerr << ERROR << '\n';									\
		message_in_func(assertion)									\
		std::cerr << "    Invalid variable type '" << vt << "'.\n";	\
		return err_type::test_format;								\
	}
#define assert_correct_format(assertion, f)							\
	if (f != "int" and f != "string") {								\
		std::cerr << ERROR << '\n';									\
		message_in_func(assertion)									\
		std::cerr << "    Invalid format type '" << f << "'.\n";	\
		return err_type::test_format;								\
	}

#define comparison_error(op, var1, var2, val_var1, val_var2)		\
	std::cerr << ERROR << '\n';										\
	std::cerr << "    Assertion '" << op << "' failed for\n";		\
	std::cerr << "        '" << var1 << "' = " << val_var1 << '\n';	\
	std::cerr << "        '" << var2 << "' = " << val_var2 << '\n';

#define get_var_value(M, var) M.find(var)->second

namespace tests {
namespace numeric {

template <typename U, typename V>
err_type resolve_comp_rational(
	const std::string& var1, const std::string& var2,
	const U& val1, const std::string& op, const V& val2
)
noexcept
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
		if (val2 == val1) {
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
		std::cerr << ERROR << '\n';
		message_in_func("comparison " + op)
		std::cerr << "    Operator is not one of ==, !=, >=, >, <=, <\n";
		std::cerr << "    Operator is: " << op << '\n';
		return err_type::test_format;
	}
	return err_type::no_error;
}

err_type comp_rational(
	const std::map<std::string, std::string>& vtypes,
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	const std::map<std::string, lal::numeric::rational>& rational_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op_comp;
	std::string var1, var2;
	fin >> var1 >> op_comp >> var2;

	assert_exists_variable(var1)
	assert_exists_variable(var2)

	lal::numeric::integer ival1;
	lal::numeric::rational rval1;
	const std::string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	lal::numeric::integer ival2;
	lal::numeric::rational rval2;
	const std::string& type2 = var_type(var2);
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

	std::cerr << ERROR << '\n';
	std::cerr << "    In operation '" << op_comp << "': combination of variable's types is invalid.\n";
	std::cerr << "    Type of variable '" << var1 << "' is: " << type1 << ".\n";
	std::cerr << "    Type of variable '" << var2 << "' is: " << type2 << ".\n";
	return err_type::test_execution;
}

err_type comp_rational_lit(
	const std::map<std::string, std::string>& vtypes,
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	const std::map<std::string, lal::numeric::rational>& rational_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op_comp;
	std::string var1;
	int64_t val2;
	fin >> var1 >> op_comp >> val2;

	assert_exists_variable(var1)

	lal::numeric::integer ival1;
	lal::numeric::rational rval1;
	const std::string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	return
	(type1 == "integer" ?
		resolve_comp_rational(var1, "literal", ival1, op_comp, val2) :
		resolve_comp_rational(var1, "literal", rval1, op_comp, val2)
	);
}

template <typename U>
lal::numeric::rational resolve_op_rational
(const U& var1, const std::string& op, const lal::numeric::rational& var2)
noexcept
{
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	return -1;
}

template <
	typename U, typename V,
	typename std::enable_if_t<
		std::is_same<V, lal::numeric::integer>::value || std::is_integral<V>::value, int
	> = 0
>
lal::numeric::rational resolve_op_rational
(const U& var1, const std::string& op, const V& var2)
noexcept
{
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }
	if (op == "^") {
#if defined DEBUG
		assert(var2 >= 0);
#endif
		if constexpr (std::is_same_v<V, lal::numeric::integer>) {
			return var1.power(var2);
		}
		else {
			return var1.power(static_cast<uint64_t>(var2));
		}
	}
	return -1;
}

err_type operation(
	const std::map<std::string, std::string>& vtypes,
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	std::map<std::string, lal::numeric::rational>& rational_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op;
	std::string var0, var1, var2;
	fin >> var0 >> var1 >> op >> var2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)
	assert_exists_variable(var2)

	lal::numeric::integer ival1, ival2;
	lal::numeric::rational rval1, rval2;

	const std::string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	const std::string& type2 = var_type(var2);
	if (type2 == "integer") { ival2 = get_var_value(integer_vars, var2); }
	else { rval2 = get_var_value(rational_vars, var2); }

	lal::numeric::rational R;
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
		std::cerr << ERROR << '\n';
		std::cerr << "    In operation '" << op << "': combination of variable's types is invalid.\n";
		std::cerr << "    Type of variable '" << var1 << "' is: " << type1 << '\n';
		std::cerr << "    Type of variable '" << var2 << "' is: " << type2 << '\n';
		return err_type::test_execution;
	}

	rational_vars[var0] = R;
	return err_type::no_error;
}

err_type op_rational_lit(
	const std::map<std::string, std::string>& vtypes,
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	std::map<std::string, lal::numeric::rational>& rational_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op;
	std::string var0, var1;
	int64_t val2;
	fin >> var0 >> var1 >> op >> val2;

	assert_exists_variable(var0)
	assert_exists_variable(var1)

	lal::numeric::integer ival1;
	lal::numeric::rational rval1;

	const std::string& type1 = var_type(var1);
	if (type1 == "integer") { ival1 = get_var_value(integer_vars, var1); }
	else { rval1 = get_var_value(rational_vars, var1); }

	lal::numeric::rational R;
	if (type1 == "integer") {
		R = resolve_op_rational(ival1, op, val2);
	}
	else {
		R = resolve_op_rational(rval1, op, val2);
	}
	rational_vars[var0] = R;
	return err_type::no_error;
}

err_type test_rational_minutia() noexcept {
	{
	lal::numeric::integer i1;
	lal::numeric::integer i2;
	i1.swap(i2);
	}
	{
	lal::numeric::integer i1;
	lal::numeric::integer i2;
	i2.swap(i1);
	}

	{
	lal::numeric::integer i1 = 0;
	lal::numeric::integer i2;
	i1.swap(i2);
	}
	{
	lal::numeric::integer i1 = 0;
	lal::numeric::integer i2;
	i2.swap(i1);
	}

	{
	lal::numeric::integer i1;
	lal::numeric::integer i2 = 0;
	i1.swap(i2);
	}
	{
	lal::numeric::integer i1;
	lal::numeric::integer i2 = 0;
	i2.swap(i1);
	}

	{
	lal::numeric::integer i1 = 0;
	lal::numeric::integer i2 = 0;
	i1.swap(i2);
	}
	{
	lal::numeric::integer i1 = 0;
	lal::numeric::integer i2 = 0;
	i2.swap(i1);
	}

	{
	std::map<uint64_t, std::pair<lal::numeric::integer, lal::numeric::integer>> hash;
	hash.insert( {3, {{"42"}, {"9999"}}} );
	hash.insert( {4, {{"40"}, {"99899"}}} );

#define test_eq(data, good)																\
	if (data != good) {																	\
		std::cerr << ERROR << '\n';														\
		std::cerr << "    Integer values do not coincide.\n";							\
		std::cerr << "    Expected '" << good << "', retrieved '" << data << "'.\n";	\
		return err_type::test_execution;												\
	}

	const auto& p1 = hash[3];
	test_eq(p1.first, lal::numeric::integer("42"))
	test_eq(p1.second, lal::numeric::integer("9999"))

	const auto& p2 = hash[4];
	test_eq(p2.first, lal::numeric::integer("40"))
	test_eq(p2.second, lal::numeric::integer("99899"))
	}

	return err_type::no_error;
}

err_type exe_numeric_rational(const input_list& inputs, std::ifstream& fin) noexcept {
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	std::map<std::string, std::string> vtypes;
	std::map<std::string, lal::numeric::integer> integer_vars;
	std::map<std::string, lal::numeric::rational> rational_vars;

	std::string command;
	std::string var_name, var_type;
	std::string format;
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
				else if (format == "std::string") {
					std::string val;
					fin >> val;
					integer_vars[var_name] = lal::numeric::integer(val);
				}
			}
			else {
				if (format == "int") {
					int64_t val;
					fin >> val;
					rational_vars[var_name] = val;
				}
				else if (format == "string") {
					std::string val;
					fin >> val;
					rational_vars[var_name] = lal::numeric::rational(val);
				}
			}
		}
		else if (command == "assign") {
			fin >> var_name;
			assert_exists_variable(var_name)
			std::string value;
			fin >> value;
			rational_vars[var_name] = lal::numeric::rational(value);
		}
		else if (command == "print") {
			fin >> var_name;
			assert_exists_variable(var_name)
			if (vtypes[var_name] == "rational") {
				std::cout << var_name << "= " << get_var_value(rational_vars, var_name) << '\n';
			}
			else {
				std::cout << var_name << "= " << get_var_value(integer_vars, var_name) << '\n';
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
			err = test_rational_minutia();
			if (err != err_type::no_error) { return err; }
		}
		else {
			std::cerr << ERROR << '\n';
			std::cerr << "    Invalid command '" << command << "'.\n";
			return err_type::test_format;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

err_type exe_numeric_rational_manual(const input_list& inputs,std::ifstream&)
noexcept
{
	if (inputs.size() != 0) {
		std::cerr << ERROR << '\n';
		std::cerr << "    No input files are allowed in this test.\n";
		std::cerr << "    Instead, " << inputs.size() << " were given.\n";
		return err_type::test_format;
	}

	int i = 0;

#define check_result(WHAT, EXPR, RES)										\
	i += 1;																	\
	if (EXPR != RES) {														\
		std::cerr << ERROR << '\n';												\
		std::cerr << "    At expression " << i << ")\n";					\
		std::cerr << "    Input expression computes a result different\n";	\
		std::cerr << "    from the ground truth.\n";						\
		std::cerr << "    Result of '" << WHAT << "'= " << EXPR << '\n';			\
		std::cerr << "    Expected: " << RES << '\n';							\
		return err_type::test_execution;									\
	}

	{
	lal::numeric::rational k(3);
	k.invert();
	if (k != lal::numeric::rational(1,3)) {
		std::cerr << ERROR << '\n';
		std::cerr << "    Inversion of lal::numeric::rationalfailed.\n";
		return err_type::test_execution;
	}
	}

	lal::numeric::rational k(3);
	check_result("k + 3", k + 3, 6);
	check_result("3 + k", 3 + k, 6);
	check_result("k - 3", k - 3, 0);
	check_result("k - (-3)", k - (-3), 6);
	check_result("3 - k", 3 - k, 0);
	check_result("3 - (-k)", 3 - (-k), 6);
	check_result("k*3", k*3, 9);
	check_result("k*(-3)", k*(-3), -9);
	check_result("(-k)*3", (-k)*3, -9);
	check_result("(-k)*(-3)", (-k)*(-3), 9);
	check_result("3*k", 3*k, 9);
	check_result("(-3)*k", (-3)*k, -9);
	check_result("3*(-k)", 3*(-k), -9);
	check_result("(-3)*(-k)", (-3)*(-k), 9);
	check_result("k/3", k/3, 1);
	check_result("k/(-3)", k/(-3), -1);
	check_result("(-k)/3", (-k)/3, -1);
	check_result("(-k)/(-3)", (-k)/(-3), 1);
	check_result("3/k", 3/k, 1);
	check_result("(-3)/k", (-3)/k, -1);
	check_result("3/(-k)", 3/(-k), -1);
	check_result("(-3)/(-k)", (-3)/(-k), 1);

	check_result("18/k", 18/k, 6);
	check_result("18/integer(1234)", 18/lal::numeric::integer(1234), 0);

	k = 6;
	check_result("k + 3", k + 3, 9);
	check_result("3 + k", 3 + k, 9);
	check_result("k - 3", k - 3, 3);
	check_result("k - (-3)", k - (-3), 9);
	check_result("3 - k", 3 - k, -3);
	check_result("3 - (-k)", 3 - (-k), 9);
	check_result("k*3", k*3, 18);
	check_result("k*(-3)", k*(-3), -18);
	check_result("(-k)*3", (-k)*3, -18);
	check_result("(-k)*(-3)", (-k)*(-3), 18);
	check_result("3*k", 3*k, 18);
	check_result("(-3)*k", (-3)*k, -18);
	check_result("3*(-k)", 3*(-k), -18);
	check_result("(-3)*(-k)", (-3)*(-k), 18);
	check_result("k/3", k/3, 2);
	check_result("k/(-3)", k/(-3), -2);
	check_result("(-k)/3", (-k)/3, -2);
	check_result("(-k)/(-3)", (-k)/(-3), 2);
	check_result("3/k", 3/k, lal::numeric::rational(1,2));
	check_result("(-3)/k", (-3)/k, lal::numeric::rational(-1,2));
	check_result("3/(-k)", 3/(-k), lal::numeric::rational(-1,2));
	check_result("(-3)/(-k)", (-3)/(-k), lal::numeric::rational(1,2));

	k = lal::numeric::rational(3,4);
	check_result("k + 3", k + 3, lal::numeric::rational(15,4));
	check_result("3 + k", 3 + k, lal::numeric::rational(15,4));
	check_result("k - 3", k - 3, lal::numeric::rational(-9,4));
	check_result("k - (-3)", k - (-3), lal::numeric::rational(15,4));
	check_result("3 - k", 3 - k, lal::numeric::rational(9,4));
	check_result("3 - (-k)", 3 - (-k), lal::numeric::rational(15,4));
	check_result("k*3", k*3, lal::numeric::rational(9,4));
	check_result("k*(-3)", k*(-3), lal::numeric::rational(-9,4));
	check_result("(-k)*3", (-k)*3, lal::numeric::rational(-9,4));
	check_result("(-k)*(-3)", (-k)*(-3), lal::numeric::rational(9,4));
	check_result("3*k", 3*k, lal::numeric::rational(9,4));
	check_result("(-3)*k", (-3)*k, lal::numeric::rational(-9,4));
	check_result("3*(-k)", 3*(-k), lal::numeric::rational(-9,4));
	check_result("(-3)*(-k)", (-3)*(-k), lal::numeric::rational(9,4));
	check_result("k/3", k/3, lal::numeric::rational(1,4));
	check_result("k/(-3)", k/(-3), lal::numeric::rational(-1,4));
	check_result("(-k)/3", (-k)/3, lal::numeric::rational(-1,4));
	check_result("(-k)/(-3)", (-k)/(-3), lal::numeric::rational(1,4));
	check_result("3/k", 3/k, lal::numeric::rational(4));
	check_result("(-3)/k", (-3)/k, lal::numeric::rational(-4));
	check_result("3/(-k)", 3/(-k), lal::numeric::rational(-4));
	check_result("(-3)/(-k)", (-3)/(-k), lal::numeric::rational(4));

	check_result("18/k", 18/k, lal::numeric::rational(24));

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace numeric
} // -- namespace tests
