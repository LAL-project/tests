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

#include "memory/numeric.hpp"

// C++ includes
#include <iostream>
#include <vector>

// lal includes
#include <lal/numeric/rational_output.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace memory {

err_type test_rational_swap() {
	{
	lal::numeric::rational i1;
	lal::numeric::rational i2;
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 0);
	}
	{
	lal::numeric::rational i1;
	lal::numeric::rational i2;
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 0);
	}

	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2;
	i1.swap(i2);
	check_1v_1v(i1, 0, i2, 50);
	}
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2;
	i2.swap(i1);
	check_1v_1v(i1, 0, i2, 50);
	}

	{
	lal::numeric::rational i1;
	lal::numeric::rational i2 = 50;
	i1.swap(i2);
	check_1v_1v(i1, 50, i2, 0);
	}
	{
	lal::numeric::rational i1;
	lal::numeric::rational i2 = 50;
	i2.swap(i1);
	check_1v_1v(i1, 50, i2, 0);
	}

	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = 2;
	i1.swap(i2);
	check_1v_1v(i1, 2, i2, 50);
	}
	{
	lal::numeric::rational i1 = 50;
	lal::numeric::rational i2 = 2;
	i2.swap(i1);
	check_1v_1v(i1, 2, i2, 50);
	}

	return err_type::no_error;
}

} // -- namespace memory
} // -- namespace tests
