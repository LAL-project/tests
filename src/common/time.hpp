/********************************************************************
 *
 * Tests of the Linear Arrangement Library - Programs used to test the
 * algorithms in the linear arrangement library.
 *
 * Copyright (C) 2019 - 2025
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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
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

#pragma once

// C++ includes
#include <chrono>

namespace tests {
namespace timing {

typedef std::chrono::high_resolution_clock::time_point time_point;

// Returns the current time
inline time_point now() noexcept
{
	return std::chrono::high_resolution_clock::now();
}

// Returns the elapsed time between 'begin' and 'end' in seconds
inline double
elapsed_seconds(const time_point& begin, const time_point& end) noexcept
{
	return std::chrono::duration<double, std::chrono::seconds::period>(
			   end - begin
	)
		.count();
}

// Returns the elapsed time between 'begin' and 'end' in microseconds
inline double
elapsed_milliseconds(const time_point& begin, const time_point& end) noexcept
{
	return std::chrono::duration<double, std::chrono::milliseconds::period>(
			   end - begin
	)
		.count();
}

// Returns the elapsed time between 'begin' and 'end' in microseconds
inline double
elapsed_microseconds(const time_point& begin, const time_point& end) noexcept
{
	return std::chrono::duration<double, std::chrono::microseconds::period>(
			   end - begin
	)
		.count();
}

} // namespace timing
} // namespace tests
