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

#include <limits>

namespace graph {
/// The id type that is used to identify vertices and edges
using Id = int;

/// @brief base class that represents an object which is identifiable throught
/// its id
struct Identifiable {
  explicit constexpr Identifiable(Id id) : id{id} {};

  Id id;

  operator Id() const { return id; }
};

/// @brief A simple struct representing a vertex of any graph which has an id
struct Vertex : Identifiable {
  explicit constexpr Vertex(Id id) : Identifiable{id} {};

  /// @brief checks if the identifieable object is a valid one
  /// @return true if the id is non negative, false otherwise
  [[nodiscard]] bool is_valid() const { return id >= 0; }

  bool operator==(const Vertex &other) const {
    return (!other.is_valid() && !is_valid()) || (id == other.id);
  }
};

/// @brief A simple struct that represents an edge with ID and the vertices
/// it connects
struct Edge : Identifiable {
  explicit constexpr Edge(Id id, Vertex u, Vertex v)
      : Identifiable{id}, u{u}, v{v} {};

  explicit constexpr Edge() : Identifiable{-1}, u{Vertex(-1)}, v{Vertex(-1)} {};

  /// @brief checks if the identifieable object is a valid one
  /// @return true if the id is non negative, false otherwise
  [[nodiscard]] bool is_valid() const {
    return id >= 0 && u.is_valid() && v.is_valid();
  }

  bool operator==(const Edge &other) const {
    return (!other.is_valid() && !is_valid()) ||
           (id == other.id && u == other.u && v == other.v);
  }

  Vertex u, v;
};

/// @brief enum whose values are used to distinguish between directed and
/// undirected graphs
enum class Directedness { DIRECTED, UNDIRECTED };

constexpr Id MIN_VALID_ID = 0;
constexpr Id MAX_VALID_ID = std::numeric_limits<Id>::max();
constexpr Vertex INVALID_VERTEX = Vertex(-1);
constexpr Edge INVALID_EDGE = Edge(-1, INVALID_VERTEX, INVALID_VERTEX);

} // namespace graph