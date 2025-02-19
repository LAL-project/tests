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

// C++ includes
#include <iostream>
#include <fstream>

// lal includes
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/properties/vertex_orbits.hpp>
#include <lal/graphs/output.hpp>

// common includes
#include "common/definitions.hpp"
#include "nauty_orbits.hpp"

namespace tests {
namespace properties {

typedef std::vector<lal::node> orbit;
typedef std::vector<orbit> orbit_set;

bool equal(const orbit_set& LAL, const orbit_set& nauty) noexcept
{
	if (LAL.size() != nauty.size()) {
		return false;
	}

	lal::detail::array<char> nauty_matched(nauty.size(), 0);

	for (std::size_t i = 0; i < LAL.size(); ++i) {
		bool matched = false;

		for (std::size_t j = 0; j < nauty.size() and not matched; ++j) {
			// ignore nauty orbits that have been matched with a LAL orbit
			if (nauty_matched[j] == 1) {
				continue;
			}
			// if the orbits do not have the same size, they cannot be equal
			if (LAL[i].size() != nauty[j].size()) {
				continue;
			}

			if (LAL[i] == nauty[j]) {
				// these two orbits have been paired
				matched = true;
				nauty_matched[j] = 1;
			}
		}

		if (not matched) {
			return false;
		}
	}

	return std::all_of(
		nauty_matched.begin(),
		nauty_matched.end(),
		[](char c)
		{
			return c == 1;
		}
	);
}

err_type test_exhaustive(std::ifstream& fin) noexcept
{
	uint64_t n;
	while (fin >> n) {

		lal::generate::all_ulab_free_trees gen(n);
		while (not gen.end()) {
			const lal::graphs::free_tree t = gen.get_tree();

			orbit_set LAL = lal::properties::vertex_orbits_compute(t);
			for (orbit& o : LAL) {
				std::sort(o.begin(), o.end());
			}

			orbit_set nauty = nauty_lib::vertex_orbits_compute(t);
			for (orbit& o : nauty) {
				std::sort(o.begin(), o.end());
			}

			if (not equal(LAL, nauty)) {
				std::cerr << ERROR << '\n';
				std::cerr << "Orbits for tree\n";
				std::cerr << t << '\n';
				std::cerr << "were not computed by LAL correctly.\n";

				std::cerr << "LAL computed:\n";
				for (const orbit& o : LAL) {
					std::cerr << "    *";
					for (lal::node u : o) {
						std::cerr << ' ' << u;
					}
					std::cerr << '\n';
				}

				std::cerr << "nauty computed:\n";
				for (const orbit& o : nauty) {
					std::cerr << "    *";
					for (lal::node u : o) {
						std::cerr << ' ' << u;
					}
					std::cerr << '\n';
				}

				return err_type::test_execution;
			}

			gen.next();
		}
	}
	return err_type::no_error;
}

err_type test_random(std::ifstream& fin) noexcept
{
	uint64_t n, R;
	while (fin >> n >> R) {

		lal::generate::rand_ulab_free_trees gen(n);
		for (std::size_t i = 0; i < R; ++i) {
			const lal::graphs::free_tree t = gen.get_tree();

			orbit_set LAL = lal::properties::vertex_orbits_compute(t);
			for (orbit& o : LAL) {
				std::sort(o.begin(), o.end());
			}

			orbit_set nauty = nauty_lib::vertex_orbits_compute(t);
			for (orbit& o : nauty) {
				std::sort(o.begin(), o.end());
			}

			if (not equal(LAL, nauty)) {
				std::cerr << ERROR << '\n';
				std::cerr << "Orbits for tree\n";
				std::cerr << t << '\n';
				std::cerr << "were not computed by LAL correctly.\n";

				std::cerr << "LAL computed:\n";
				for (const orbit& o : LAL) {
					std::cerr << "    *";
					for (lal::node u : o) {
						std::cerr << ' ' << u;
					}
					std::cerr << '\n';
				}

				std::cerr << "nauty computed:\n";
				for (const orbit& o : nauty) {
					std::cerr << "    *";
					for (lal::node u : o) {
						std::cerr << ' ' << u;
					}
					std::cerr << '\n';
				}

				return err_type::test_execution;
			}
		}
	}
	return err_type::no_error;
}

err_type exe_properties_orbits(std::ifstream& fin) noexcept
{
	std::string mode;
	fin >> mode;

	err_type r;
	if (mode == "exhaustive") {
		r = test_exhaustive(fin);
	}
	else if (mode == "random") {
		r = test_random(fin);
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Wrong mode '" << mode << "'\n";
		return err_type::test_format;
	}

	if (r == err_type::no_error) {
		TEST_GOODBYE;
	}
	return r;
}

} // namespace properties
} // namespace tests
