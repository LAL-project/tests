/********************************************************************
 *
 *  Tests of the Linear Arrangement Library - Programs used to test the
 *  algorithms in the linear arrangement library.
 *
 *  Copyright (C) 2019 - 2021
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

#pragma once

// lal includes
#include <lal/numeric.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace exe_tests {


/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Compute the variance of the number of crossings \f$V_{rla}[C]\f$ over the space
 * of random linear arrangements.
 *
 * This function enumerates all elements of \f$Q\f$, classifies all elements of
 * \f$Q\times Q\f$ into their corresponding frequency type (see \cite Alemany2019a)
 * and then computes the variance.
 * @param g Input graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V_{rla}[C]\f$.
 */
lal::numeric::rational variance_C_freqs_rational
(const lal::graphs::undirected_graph& g);


/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Given the set \f$Q(G)\f$ of a graph \f$G\f$, classifies all elements of
 * \f$Q\times Q\f$ into their corresponding frequency type (see \cite Alemany2019a)
 * and then computes the variance
 * @param Q Input set \f$Q\f$ of a graph.
 * @param nthreads Number of threads. Default: 8.
 * @return Returns the exact value of \f$V_{rla}[C]\f$.
 */
lal::numeric::rational variance_C_freqs_Q_rational
(const std::vector<lal::edge_pair>& Q);

/**
 * @brief Computes the variance of the number of crossings of a graph.
 *
 * Computes \f$V_{rla}[C]\f$ on the given graph. Applies a more efficient method than
 * the function @ref variance_C_freqs_Q_rational applies.
 * @param g Input graph \f$G\f$.
 * @param Q Set of pairs of independent edges \f$Q(G)\f$.
 * @return Returns the exact value of \f$V_{rla}[C]\f$ as a rational value.
 */
lal::numeric::rational variance_C_rational_Q
(const lal::graphs::undirected_graph& g, const std::vector<lal::edge_pair>& Q);

} // -- namespace exe_tests
