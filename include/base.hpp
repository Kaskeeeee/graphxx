/**
 * @file This file contains definitions of basic graph types
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include <cstddef> // size_t
#include <limits>  // std::numeric_limits

/// graphxx namespace contains the main features of the graphxx library
namespace graphxx {

/// The id type that is used to identify vertices and edges
using DefaultIdType = size_t;

/// @brief enum whose values are used to distinguish between directed and
/// undirected graphs
enum class Directedness { DIRECTED, UNDIRECTED };

/// @brief Type of a graph vertex
/// @tparam  G type of the graph
template <typename G> using Vertex = typename G::Vertex;

/// @brief Type of a graph edge
/// @tparam G type of the graph
template <typename G> using Edge = typename G::Edge;

/// @brief Represents an invalid vertex
/// @tparam G type of the graph
template <typename G>
constexpr Vertex<G> INVALID_VERTEX = std::numeric_limits<Vertex<G>>::max();

} // namespace graphxx