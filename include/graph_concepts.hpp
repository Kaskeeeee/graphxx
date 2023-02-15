/**
 * @file This file contains graphs concepts
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

#include "base.hpp" // G::Vertex

#include <concepts> // std::unsigned_integral
#include <cstdint>  // size_t
#include <ranges>   // std::ranges::range_value_t

namespace graphxx::concepts {

/// @brief Type of the values contained in a range
template <typename T> using InnerValue = std::ranges::range_value_t<T>;

/// @brief Check if type is a range of ranges (e.g. matrix)
template <typename T>
concept IsRangeOfRanges = std::ranges::random_access_range<T> &&
                          std::ranges::forward_range<InnerValue<T>>;

/// @brief Check if type has graph basic traits
template <typename G>
concept HasGraphBasicTraits =
    requires(G) {
      typename G::Vertex;
      typename G::Attributes;
      typename G::Edge;
      requires G::DIRECTEDNESS == Directedness::DIRECTED ||
                       G::DIRECTEDNESS == Directedness::UNDIRECTED;
    };

/// @brief Check if type has graph basic methods
template <typename G>
concept HasGraphBasicMethods =
    requires(G g, typename G::Vertex vertex, typename G::Attributes attributes,
             typename G::Edge edge) {
      g.add_vertex();
      g.add_vertex(vertex);
      g.remove_vertex(vertex);
      g.add_edge(vertex, vertex, attributes);
      g.remove_edge(vertex, vertex);
      { g.get_edge(vertex, vertex) } -> std::convertible_to<typename G::Edge>;
      g.set_attributes(vertex, vertex, attributes);
      {
        g.get_attributes(vertex, vertex)
        } -> std::same_as<typename G::Attributes>;
      { g.num_attributes() } -> std::same_as<size_t>;
      { g.num_vertices() } -> std::same_as<size_t>;
      { g.num_edges() } -> std::same_as<size_t>;
      { g.has_vertex(vertex) } -> std::same_as<bool>;
      { g.has_edge(vertex, vertex) } -> std::same_as<bool>;
      { g.get_source(edge) } -> std::convertible_to<typename G::Vertex>;
      { g.get_target(edge) } -> std::convertible_to<typename G::Vertex>;
    };

/// @brief Check if type is compatible with graph type
template <typename G>
concept Graph =
    IsRangeOfRanges<G> && HasGraphBasicTraits<G> && HasGraphBasicMethods<G>;

/// @brief type of the graph identifiers
template <typename T>
concept Identifier = std::unsigned_integral<T>;

/// @brief Check if type is a struct with a `parent` field
template <typename T>
concept HasParent = requires(T t) { t.parent; };

/// @brief Check if type is numeric
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

} // namespace graphxx::concepts