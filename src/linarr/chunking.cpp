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
#include <set>

// lal includes
#include <lal/generate/rand_lab_rooted_trees.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/conversions.hpp>
#include <lal/linarr/chunking/algorithms.hpp>
#include <lal/linarr/chunking/chunking.hpp>

// common includes
#include "common/definitions.hpp"
#include "common/parse_header.hpp"

namespace tests {
namespace linarr {

void output_head_vector(const lal::head_vector& hv) noexcept {
	std::cout << hv[0];
	for (std::size_t i = 1; i < hv.size(); ++i) {
		std::cout << ' ' << hv[i];
	}
	std::cout << '\n';
}

void output_chunks(const lal::linarr::chunk_sequence& chunks) noexcept {
	for (std::size_t i = 0; i < chunks.size(); ++i) {
		std::cout << i << ":\n";
		const lal::linarr::chunk& chunk = chunks[i];

		if (not chunk.has_parent_node()) {
			std::cout << "\t!\n";
		}
		else {
			std::cout << "\tpar: " << chunk.get_parent_node() << '\n';
		}

		std::cout << "\tnod:";
		const auto& nodes = chunk.get_nodes();
		for (lal::node v : nodes) {
			std::cout << ' ' << v;
			if (chunk.has_root_node() and chunk.get_root_node() == v) {
				std::cout << '*';
			}
		}
		std::cout << '\n';
	}
}

bool are_nodes_contiguous(
	const std::vector<lal::node>& node_set,
	const lal::linear_arrangement& arr
)
noexcept
{
	std::set<lal::position> positions;
	for (lal::node_t v : node_set) {
		positions.insert( arr[v] );
	}
	
	auto it = positions.begin();
	lal::position prev = *it;
	++it;
	for (; it != positions.end(); ++it) {
		const lal::position current = *it;
		if (prev != current - 1) {
			return false;
		}
		prev = current;
	}
	
	return true;
}

err_type test_chunked_tree(
	const lal::graphs::rooted_tree& rt,
	const lal::linear_arrangement& arr,
	const lal::graphs::rooted_tree& chunked_rt,
	const lal::linarr::chunk_sequence& chunks,
	lal::linarr::algorithms_chunking algo
)
noexcept
{
	bool error = false;
	if (not chunked_rt.is_rooted_tree()) {
		std::cerr << ERROR << '\n';
		std::cerr << "Chunked tree is not a rooted tree.\n";
		error = true;
	}
	
	if (chunked_rt.get_num_nodes() != chunks.size()) {
		std::cerr << ERROR << '\n';
		std::cerr << "Number of vertices of chunked tree is different from the number of chunks.\n";
		std::cerr << "    vertices: " << chunked_rt.get_num_nodes() << '\n';
		std::cerr << "    chunks:   " << chunks.size() << '\n';
		error = true;
	}
	
	{
	// exactly one chunk has no parent vertex
	int num_chunks_with_no_parent = 0;
	for (const auto& c : chunks.get_chunks()) {
		if (not c.has_parent_node()) {
			++num_chunks_with_no_parent;
		}
	}
	if (num_chunks_with_no_parent > 1) {
		std::cerr << ERROR << '\n';
		std::cerr << "More than one chunk has no parent node\n";
		error = true;
	}
	}
	
	// ensure that chunks are contiguous
	if (
		algo == lal::linarr::algorithms_chunking::Anderson or
		algo == lal::linarr::algorithms_chunking::Macutek
	)
	{
		for (std::size_t i = 0; i < chunks.size(); ++i) {
			const auto& c = chunks[i];
			if (not are_nodes_contiguous(c.get_nodes(), arr)) {
				std::cerr << ERROR << '\n';
				std::cerr << "Nodes of chunk " << i << " are not contiguous in the arrangement\n";
				error = true;
			}
		}
	}

	// ensure the chunk's root and its parent are adjacent in the tree
	{
	for (std::size_t i = 0; i < chunks.size(); ++i) {
		const auto& c = chunks[i];
		if (c.has_parent_node() and c.has_root_node()) {
			const lal::node parent = c.get_parent_node();
			const lal::node root = c.get_root_node();
			if (not rt.has_edge(parent, root)) {
				std::cerr << ERROR << '\n';
				std::cerr << "Parent of chunk (" << i << "): " << parent << '\n';
				std::cerr << "Root of chunk (" << i << "):   " << root<< '\n';
				std::cerr << "Are not adjacent in the tree.\n";
				error = true;
			}
		}
		if (not c.has_root_node()) {
			std::cerr << ERROR << '\n';
			std::cerr << "Chunk (" << i << ") does not have a root vertex\n";
			error = true;
		}
	}
	}
	
	if (error) {
		std::cerr << "=================================\n";
		std::cerr << "Chunks: " << chunks.size() << '\n';
		for (const auto& c : chunks.get_chunks()) {
			std::cerr << "---------------------------------\n";
			std::cerr << "    Size: " << c.get_nodes().size() << '\n';
			std::cerr
				<< "    Chunk parent: "
				<< (c.has_parent_node() ? std::to_string(c.get_parent_node()) : "*")
				<< '\n';
			std::cerr << "    Nodes:";
			for (auto u : c.get_nodes()) {
				std::cerr << ' ' << u;
			}
			std::cerr << '\n';
		}
		std::cerr << "=================================\n";
		std::cerr << "Mapping:\n";
		for (lal::node u = 0; u < rt.get_num_nodes(); ++u) {
			std::cerr << "    map[" << u << "]= " << chunks.get_chunk_index(u) << '\n';
		}
		std::cerr << "=================================\n";
		std::cerr << "Arrangement:\n";
		for (lal::node_t u = 0ull; u < rt.get_num_nodes(); ++u) {
			std::cerr << "arr[" << *u << "]= " << arr[u] << '\n';
		}
		return err_type::test_execution;
	}
	
	return err_type::no_error;
}

void read_manual_input(
	std::ifstream& fin,
	lal::graphs::rooted_tree& rt,
	lal::linear_arrangement& arr
)
noexcept
{
	uint64_t n;
	fin >> n;

	lal::head_vector hv(n);
	for (uint64_t i = 0; i < n; ++i) {
		fin >> hv[i];
	}

	rt = lal::graphs::from_head_vector_to_rooted_tree(hv);

	arr = lal::linear_arrangement(n);
	for (lal::position p = 0; p < n; ++p) {
		lal::node u;
		fin >> u;
		arr.assign(u, p);
	}
}

err_type test_algo_manual(
	std::ifstream& fin,
	const lal::linarr::algorithms_chunking& algo
)
noexcept
{
	lal::graphs::rooted_tree rt;
	lal::linear_arrangement arr;
	read_manual_input(fin, rt, arr);
	
	const lal::linarr::chunk_sequence chunks =
		lal::linarr::chunk_syntactic_dependency_tree_as_sequence(rt, arr, algo);
	
	const lal::graphs::rooted_tree chunked_rt =
		lal::linarr::make_tree_from_chunk_sequence(chunks);
	
	output_chunks(chunks);
	output_head_vector(chunked_rt.get_head_vector());
	
	return test_chunked_tree(rt, arr, chunked_rt, chunks, algo);
}

err_type test_algo_automatic(
	std::ifstream& fin,
	const lal::linarr::algorithms_chunking& algo
)
noexcept
{
	uint64_t n, R;
	
	while (fin >> n >> R) {
		lal::generate::rand_lab_rooted_trees gen(n, 1234);
		for (uint64_t r = 0; r < R; ++r) {
			const auto rt = gen.get_tree();
			const auto arr = lal::linear_arrangement::identity(n);
			
			const lal::linarr::chunk_sequence chunks =
				lal::linarr::chunk_syntactic_dependency_tree_as_sequence(rt, arr, algo);
			
			const lal::graphs::rooted_tree chunked_rt =
				lal::linarr::make_tree_from_chunk_sequence(chunks);
			
			std::cout << "===\n";
			output_chunks(chunks);
			output_head_vector(chunked_rt.get_head_vector());
			
			const auto err = test_chunked_tree(rt, arr, chunked_rt, chunks, algo);
			if (err != err_type::no_error) {
				return err;
			}
		}
	}

	return err_type::no_error;
}

err_type exe_linarr_chunking(std::ifstream& fin) noexcept {
	const input_list inputs = read_input_list(fin);

	std::string what_algo;
	fin >> what_algo;
	
	std::string what_mode;
	fin >> what_mode;
	
	lal::linarr::algorithms_chunking algo;
	if (what_algo == "Anderson") {
		algo = lal::linarr::algorithms_chunking::Anderson;
	}
	else if (what_algo == "Macutek") {
		algo = lal::linarr::algorithms_chunking::Macutek;
	}
	else {
		std::cerr << ERROR << '\n';
		std::cerr << "Algorithm '" << what_algo << "' not valid.\n";
		return err_type::test_format;
	}

	err_type r = err_type::no_error;

	if (what_mode == "manual") {
		r = test_algo_manual(fin, algo);
	}
	else if (what_mode == "automatic") {
		r = test_algo_automatic(fin, algo);
	}

	if (r != err_type::no_error) { return r; }

	TEST_GOODBYE;
	return err_type::no_error;
}

} // -- namespace linarr
} // -- namespace tests
