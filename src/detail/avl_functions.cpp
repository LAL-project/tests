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
#include <functional>
#include <iostream>
#include <utility>
#include <fstream>
#include <string>
#include <random>
#include <set>
// lal includes
#include <lal/detail/avl.hpp>

// common includes
#include "common/definitions.hpp"

namespace tests {
namespace detail {

struct my_struct {
	my_struct() noexcept { }
	my_struct(int _v) noexcept
		: v(_v),
		  vs(std::to_string(v))
	{ }
	my_struct(const my_struct& s) noexcept
	{
		v = s.v;
		vs = s.vs;
	}
	my_struct(my_struct&& s) noexcept
	{
		v = s.v;
		vs = std::move(s.vs);
	}

	my_struct& operator= (const my_struct& s) noexcept
	{
		v = s.v;
		vs = s.vs;
		return *this;
	}
	my_struct& operator= (my_struct&& s) noexcept
	{
		v = s.v;
		vs = std::move(s.vs);
		return *this;
	}

	inline friend std::ostream&
	operator<< (std::ostream& os, const my_struct& s) noexcept
	{
		os << '(' << s.v << ", '" + s.vs + "')";
		return os;
	}

	bool operator== (const my_struct& s) const noexcept
	{
		return v == s.v;
	}
	bool operator!= (const my_struct& s) const noexcept
	{
		return not(*this == s);
	}
	bool operator< (const my_struct& s) const noexcept
	{
		return v < s.v;
	}
	bool operator> (const my_struct& s) const noexcept
	{
		return v > s.v;
	}

	int v;
	std::string vs;
};

err_type exe_detail_avl_functions(std::ifstream& fin) noexcept
{
	std::string option;
	lal::detail::AVL<my_struct> t;
	std::size_t instruction_idx = 0;

	while (fin >> option and option != "exit") {
		++instruction_idx;
		std::cout << instruction_idx << ") " << option << '\n';

		if (option == "insert") {
			int x;
			fin >> x;

			const auto insert_freqs = t.insert(my_struct{x});
			std::cout << "(insert " << x << ") frequencies:\n"
					  << "    counter_equal:    " << insert_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << insert_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << insert_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			const auto tree_freqs = t.element_frequency(my_struct{x});
			if (insert_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << x << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << insert_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << insert_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << insert_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_unique") {
			int x;
			fin >> x;

			const auto tree_freqs = t.element_frequency(my_struct{x});

			const auto remove_freqs = t.remove<false>(my_struct{x});
			std::cout << "(remove " << x << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << x << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_non_unique") {
			int x;
			fin >> x;

			const auto tree_freqs = t.element_frequency(my_struct{x});

			const auto remove_freqs = t.remove<true>(my_struct{x});
			std::cout << "(remove " << x << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << x << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_unique_largest") {
			const std::pair<my_struct, lal::detail::AVL_frequencies<my_struct>>
				largest = t.get_largest_value();

			const auto tree_freqs = largest.second;

			const auto remove_freqs = t.remove_largest<false>();
			std::cout << "(remove " << largest.first << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << largest.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_non_unique_largest") {
			const std::pair<my_struct, lal::detail::AVL_frequencies<my_struct>>
				largest = t.get_largest_value();

			const auto tree_freqs = largest.second;

			const auto remove_freqs = t.remove_largest<true>();
			std::cout << "(remove " << largest.first << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << largest.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_unique_smallest") {
			const std::pair<my_struct, lal::detail::AVL_frequencies<my_struct>>
				smallest = t.get_smallest_value();

			const auto tree_freqs = smallest.second;

			const auto remove_freqs = t.remove_smallest<false>();
			std::cout << "(remove " << smallest.first << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << smallest.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "remove_non_unique_smallest") {
			const std::pair<my_struct, lal::detail::AVL_frequencies<my_struct>>
				smallest = t.get_smallest_value();

			const auto tree_freqs = smallest.second;

			const auto remove_freqs = t.remove_smallest<true>();
			std::cout << "(remove " << smallest.first << ") frequencies:\n"
					  << "    counter_equal:    " << remove_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << remove_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: "
					  << remove_freqs.num_nodes_larger << '\n';
			std::cout << '\n';

			if (remove_freqs != tree_freqs) {
				std::cerr << "User occurrences for value '" << smallest.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << remove_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << remove_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << remove_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "largest_element") {
			const auto tree_freqs = t.get_largest_value();

			const auto elem_freqs = t.element_frequency(tree_freqs.first);
			std::cout << "(largest element " << tree_freqs.first
					  << ") frequencies:\n"
					  << "    counter_equal:    " << elem_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << elem_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: " << elem_freqs.num_nodes_larger
					  << '\n';
			std::cout << '\n';

			if (elem_freqs != tree_freqs.second) {
				std::cerr << "Method frequenciesfor value '" << tree_freqs.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << elem_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << elem_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << elem_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.second.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.second.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.second.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "smallest_element") {
			const auto tree_freqs = t.get_smallest_value();

			const auto elem_freqs = t.element_frequency(tree_freqs.first);
			std::cout << "(smallest element " << tree_freqs.first
					  << ") frequencies:\n"
					  << "    counter_equal:    " << elem_freqs.counter_equal
					  << '\n'
					  << "    counter_larger:   " << elem_freqs.counter_larger
					  << '\n'
					  << "    num_nodes_larger: " << elem_freqs.num_nodes_larger
					  << '\n';
			std::cout << '\n';

			if (elem_freqs != tree_freqs.second) {
				std::cerr << "Method frequenciesfor value '" << tree_freqs.first
						  << "' do not\n"
						  << "agree with tree frequencies.\n"
						  << "    method counter_equal:  "
						  << elem_freqs.counter_equal << '\n'
						  << "    method counter_larger: "
						  << elem_freqs.counter_larger << '\n'
						  << "    method larger nodes:   "
						  << elem_freqs.num_nodes_larger << '\n'
						  << "    tree counter_equal:    "
						  << tree_freqs.second.counter_equal << '\n'
						  << "    tree counter_larger:   "
						  << tree_freqs.second.counter_larger << '\n'
						  << "    tree larger nodes:     "
						  << tree_freqs.second.num_nodes_larger << '\n';
				return err_type::test_execution;
			}
		}

		else if (option == "sanity_check") {
			const auto res = t.sanity_check();
			if (not res) {
				std::cerr << "Some error occurred\n";
				return err_type::test_execution;
			}
		}

		else if (option == "num_nodes") {
			std::cout << "Number of nodes in the tree: " << t.num_nodes()
					  << '\n';
		}

		else if (option == "clear") {
			t.clear();
		}

		else if (option == "total_elements") {
			std::cout << "Number of occurrences in the tree: "
					  << t.total_elements() << '\n';
		}

		else if (option == "join_trees") {
			std::size_t size;
			fin >> size;

			std::vector<my_struct> v(size);
			for (std::size_t i = 0; i < size; ++i) {
				int k;
				fin >> k;
				v[i] = my_struct{k};
			}

			t.join_sorted_all_greater(v);
		}

		else if (option == "assert_occurrences") {
			std::size_t num_elems_to_check;
			fin >> num_elems_to_check;

			for (std::size_t i = 0; i < num_elems_to_check; ++i) {
				int val;
				fin >> val;

				lal::detail::AVL_frequencies<my_struct> user_freqs;
				fin >> user_freqs.counter_equal >> user_freqs.counter_larger >>
					user_freqs.num_nodes_larger;

				const auto tree_freqs = t.element_frequency(my_struct{val});

				if (user_freqs != tree_freqs) {
					std::cerr << "User occurrences for value '" << val
							  << "' do not\n"
							  << "agree with tree frequencies.\n"
							  << "    user counter_equal:  "
							  << user_freqs.counter_equal << '\n'
							  << "    user counter_larger: "
							  << user_freqs.counter_larger << '\n'
							  << "    user larger nodes:   "
							  << user_freqs.num_nodes_larger << '\n'
							  << "    tree counter_equal:  "
							  << tree_freqs.counter_equal << '\n'
							  << "    tree counter_larger: "
							  << tree_freqs.counter_larger << '\n'
							  << "    tree larger nodes:   "
							  << tree_freqs.num_nodes_larger << '\n';
					return err_type::test_execution;
				}
			}
		}

		else if (option == "print_tree") {
			t.print_tree();
			std::cout << '\n';
		}

		else {
			std::cerr << "Wrong option '" << option << "'. Aborting...\n";
			return err_type::test_format;
		}
	}

	TEST_GOODBYE;
	return err_type::no_error;
}

} // namespace detail
} // namespace tests
