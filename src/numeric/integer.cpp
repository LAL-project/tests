/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2024
 *
 * This file is part of Tests of the Linear Arrangement Library. The full code
 * is available at:
 *     https://github.com/LAL-project/tests.git
 *
 * Tests of the Linear Arrangement Library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Affero
 * General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Tests of the Linear Arrangement Library is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Tests of the Linear Arrangement Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
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
#include <lal/numeric/integer_output.hpp>

// common includes
#include "common/definitions.hpp"

#define map_has(M, v) (M.find(v) != M.end())

#define assert_exists_variable(varname)										\
	if (not map_has(integer_vars, varname)) {								\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    Variable '" << varname << "' does not exist.\n";	\
		return err_type::test_execution;									\
	}

#define var_type(var)	(vtypes.find(var)->second)
#define message_in_func(f) std::cerr << "    -- In '" << f << "' --\n";
#define assert_correct_var_type(assertion, vt)							\
	if (vt != "integer") {												\
		std::cerr << ERROR << '\n';										\
		message_in_func(assertion)										\
		std::cerr << "    Invalid variable type '" << vt << "'.\n";	\
		return err_type::test_format;									\
	}
#define assert_correct_format(assertion, f)								\
	if (f != "int" and f != "string") {									\
		std::cerr << ERROR << '\n';										\
		message_in_func(assertion)										\
		std::cerr << "    Invalid format type '" << f << "'.\n";		\
		return err_type::test_format;									\
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
err_type resolve_comp_integer(
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

err_type comp_integer(
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op_comp;
	std::string var1, var2;
	fin >> var1 >> op_comp >> var2;

	assert_exists_variable(var1)
	assert_exists_variable(var2)

	const lal::numeric::integer& val1 = get_var_value(integer_vars, var1);
	const lal::numeric::integer& val2 = get_var_value(integer_vars, var2);
	return resolve_comp_integer(var1, var2, val1, op_comp, val2);
}

err_type comp_integer_lit(
	const std::map<std::string, lal::numeric::integer>& integer_vars,
	std::ifstream& fin
)
noexcept
{
	std::string op_comp;
	std::string var1;
	int64_t val2;
	fin >> var1 >> op_comp >> val2;

	assert_exists_variable(var1)

	const lal::numeric::integer& val1 = get_var_value(integer_vars, var1);
	return resolve_comp_integer(var1, "literal", val1, op_comp, val2);
}

template <typename U, typename V>
lal::numeric::integer resolve_integer_operation
(const U& var1, const std::string& op, const V& var2)
noexcept
{
	if (op == "+") { return var1 + var2; }
	if (op == "-") { return var1 - var2; }
	if (op == "*") { return var1 * var2; }
	if (op == "/") { return var1 / var2; }

	// NOTE: var2, when it is an int64_t, its
	// signedness changes into uint64_t.
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

err_type op_integer(
	std::map<std::string, lal::numeric::integer>& integer_vars,
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

	const lal::numeric::integer& val1 = get_var_value(integer_vars, var1);
	const lal::numeric::integer& val2 = get_var_value(integer_vars, var2);

	const lal::numeric::integer R = resolve_integer_operation(val1, op, val2);
	integer_vars[var0] = R;
	return err_type::no_error;
}

err_type op_integer_lit(
	std::map<std::string, lal::numeric::integer>& integer_vars,
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

	const lal::numeric::integer& val1 = get_var_value(integer_vars, var1);

	const lal::numeric::integer R = resolve_integer_operation(val1, op, val2);
	integer_vars[var0] = R;
	return err_type::no_error;
}

err_type exe_numeric_integer(std::ifstream& fin) noexcept {

	std::map<std::string, lal::numeric::integer> integer_vars;

	std::string command;
	std::string var_name;
	std::string format;
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
				std::string val;
				fin >> val;
				integer_vars[var_name] = lal::numeric::integer(val);
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
			std::cout << var_name << "= " << get_var_value(integer_vars, var_name) << '\n';
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
			std::cerr << ERROR << '\n';
			std::cerr << "    Invalid command '" << command << "'.\n";
			return err_type::test_format;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

err_type exe_numeric_integer_manual(std::ifstream&) noexcept {
	int i = 0;

#define check_result(WHAT, EXPR, RES)										\
	i += 1;																	\
	if (EXPR != RES) {														\
		std::cerr << ERROR << '\n';											\
		std::cerr << "    At expression " << i << ")\n";					\
		std::cerr << "    Input expression computes a result different\n";	\
		std::cerr << "    from the ground truth.\n";						\
		std::cerr << "    Result of '" << WHAT << "'= " << EXPR << '\n';	\
		std::cerr << "    Expected: " << RES << '\n';						\
		return err_type::test_execution;									\
	}

	lal::numeric::integer k(3);
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
	check_result("3/k", 3/k, 0);
	check_result("(-3)/k", (-3)/k, 0);
	check_result("3/(-k)", 3/(-k), 0);
	check_result("(-3)/(-k)", (-3)/(-k), 0);

	check_result("18/k", 18/k, 3);

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace numeric
} // -- namespace tests
