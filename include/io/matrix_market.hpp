/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
 *
 * @license{<blockquote>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </blockquote>}
 *
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <fstream>
#include <functional>

namespace graph::io::matrix_market {

/// @brief Writes a graph object into an output stream in Matrix Market NIST
///        format. It's possible to assign a weight to the edges of the graph.
/// @tparam G type of input graph
/// @tparam C type of weights map
/// @tparam WeightType type of weight
/// @param out output stream
/// @param graph input graph object
/// @param weights weights map
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
void serialize(std::ostream &out, const G &graph, C &weights);

/// @brief Writes a graph object into an output stream in Matrix Market NIST
///        format.
/// @tparam G type of input graph
/// @param out output stream
/// @param graph input graph object
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

/// @brief Interprets a graph described using the Matrix Market NIST language
///        and builds a graph object that captures that description.
/// @tparam G type of output graph
/// @tparam C C type of weights map
/// @tparam WeightType type of weight
/// @param in input stream
/// @param graph refrence to output graph
/// @param weights reference to map in which store edges' weights
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
void deserialize(std::istream &in, G &graph, C &weights);

} // namespace graph::io::matrix_market

#include "io/matrix_market.i.hpp"