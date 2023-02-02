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

#include "adaptors/map_list.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <tuple>
#include <unordered_map>
#include <vector>

namespace graphxx {

template <concepts::Identifier IdType = DefaultIdType,
          Directedness D = Directedness::DIRECTED, typename... AttributesType>
class AdjacencyMatrixGraph {
public:
  using Vertex = IdType;
  using Edge = std::tuple<Vertex, Vertex, AttributesType...>;
  using Attributes = std::tuple<AttributesType...>;
  using EdgeMap = MapList<Vertex, Edge>;
  using AdjacencyMatrix = std::vector<EdgeMap>;

  static constexpr Directedness DIRECTEDNESS = D;

  AdjacencyMatrixGraph();
  AdjacencyMatrixGraph(const AdjacencyMatrixGraph &graph);

  void add_vertex();
  void add_vertex(Vertex vertex);
  void remove_vertex(Vertex vertex);

  void add_edge(Vertex source, Vertex target, Attributes attributes = {});
  void remove_edge(Vertex source, Vertex target);

  /// @brief Get edge tuple based on the vertices it connects.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @return A tuple composed of source id, destination id and a variable
  /// number of attributes.
  const Edge &get_edge(Vertex source, Vertex target) const;

  void set_attributes(Vertex source, Vertex target, Attributes attributes = {});
  Attributes get_attributes(Vertex source, Vertex target) const;

  Vertex source(Edge &edge) const;
  Vertex target(Edge &edge) const;

  /// @brief Get number of edges attributes.
  /// @return Number of edges attributes.
  size_t num_attributes() const;

  size_t num_vertices() const;
  size_t num_edges() const;

  /// @brief
  /// @param vertex
  /// @return
  bool has_vertex(Vertex vertex) const;

  /// @brief
  /// @param source
  /// @param target
  /// @return
  bool has_edge(Vertex source, Vertex target) const;

  const EdgeMap &operator[](Vertex vertex) const;

  /// @brief Returns an iterator that points to the first element in the
  /// adjacency list.
  AdjacencyMatrix::iterator begin() { return _adj.begin(); }

  /// @brief Returns an iterator that points one past the last element in the
  /// adjacency list.
  AdjacencyMatrix::iterator end() { return _adj.end(); }

private:
  AdjacencyMatrix _adj;
};
} // namespace graphxx

#include "matrix_graph.i.hpp"