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
#include <iostream>
#include <fstream>
#include <map>

// lal includes
#include <lal/detail/data_array.hpp>
#include <lal/detail/linear_queue.hpp>
using namespace lal::detail;

// common includes
#include "common/definitions.hpp"

// memory includes
#include "memory/data_array_utils.hpp"

#define MOVE
#define COPY

// LINE LEFT BLANK INTENTIONALLY
namespace tests {
namespace memory {

namespace lal_linear_queue {

struct S {
	std::string m_s;
	S() noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
	}
	S(std::string&& s) noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
		m_s = std::move(s);
	}
	S(S&& s) noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
		m_s = std::move(s.m_s);
	}
	S(const S& s) noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
		m_s = s.m_s;
	}

	S& operator= (S&& s) noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
		m_s = std::move(s.m_s);
		return *this;
	}
	S& operator= (const S& s) noexcept {
		std::cout << __PRETTY_FUNCTION__ << '\n';
		m_s = s.m_s;
		return *this;
	}
};

#if defined MOVE
void test_move_push() noexcept {
	begin_function;

	{
	begin_case;
	linear_queue<S> q;
	std::cout << "Initializing...\n";
	q.init(10);

	std::cout << "push 'S{\"asdf\"}'...\n";
	q.push(S{"asdf"});
	std::cout << "push 'S{\"qwer\"}'...\n";
	q.push(S("qwer"));
	std::cout << "push '{\"zxcv\"}'...\n";
	q.push({"zxcv"});
	}

	{
	begin_case;
	linear_queue<S> q;
	std::cout << "Initializing...\n";
	q.init(10);

	S asdf("asdf");
	S qwer("qwer");
	S zxcv("zxcv");

	std::cout << "Push 'asdf'\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	q.push(std::move(asdf));
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";

	std::cout << "Push 'qwer'\n";
	std::cout << "    string in 'qwer': '" << qwer.m_s << "'\n";
	q.push(std::move(qwer));
	std::cout << "    string in 'qwer': '" << qwer.m_s << "'\n";

	std::cout << "Push 'zxcv'\n";
	std::cout << "    string in 'zxcv': '" << zxcv.m_s << "'\n";
	q.push(std::move(zxcv));
	std::cout << "    string in 'zxcv': '" << zxcv.m_s << "'\n";
	}
}

void test_move_pop() noexcept {
	begin_function;

	{
	begin_case;
	linear_queue<S> q;
	std::cout << "Initializing...\n";
	q.init(10);

	S asdf("asdf");

	std::cout << "Push 'asdf'\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	q.push(std::move(asdf));
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";

	std::cout << "Pop queue\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	asdf = std::move(q.pop());
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	}
}
#endif

#if defined COPY
void test_copy_push() noexcept {
	begin_function;

	{
	begin_case;
	linear_queue<S> q;
	std::cout << "Initializing...\n";
	q.init(10);

	S asdf("asdf");
	S qwer("qwer");
	S zxcv("zxcv");

	std::cout << "Push 'asdf'\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	q.push(asdf);
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";

	std::cout << "Push 'qwer'\n";
	std::cout << "    string in 'qwer': '" << qwer.m_s << "'\n";
	q.push(qwer);
	std::cout << "    string in 'qwer': '" << qwer.m_s << "'\n";

	std::cout << "Push 'zxcv'\n";
	std::cout << "    string in 'zxcv': '" << zxcv.m_s << "'\n";
	q.push(zxcv);
	std::cout << "    string in 'zxcv': '" << zxcv.m_s << "'\n";
	}
}

void test_copy_pop() noexcept {
	begin_function;

	begin_function;

	{
	begin_case;
	linear_queue<S> q;
	std::cout << "Initializing...\n";
	q.init(10);

	S asdf("asdf");

	std::cout << "Push 'asdf'\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	q.push(std::move(asdf));
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";

	std::cout << "Pop queue\n";
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	asdf = q.pop();
	std::cout << "    string in 'asdf': '" << asdf.m_s << "'\n";
	}
}
#endif

} // -- namespace lal_linear_queue

err_type exe_memory_detail_linear_queue(std::ifstream&) noexcept {

#if defined MOVE
	lal_linear_queue::test_move_push();
	lal_linear_queue::test_move_pop();
#endif

#if defined COPY
	lal_linear_queue::test_copy_push();
	lal_linear_queue::test_copy_pop();
#endif

	linear_queue<int> q;
	q.init(10);
	q.push(1);
	q.push(2);
	q.push(3);
	[[maybe_unused]] const int x = q.pop();
	[[maybe_unused]] const int y = q.pop();
	[[maybe_unused]] const int z = q.pop();

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
