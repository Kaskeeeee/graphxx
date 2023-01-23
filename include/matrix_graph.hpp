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
#include "id_manager.hpp"

#include <unordered_map>

namespace graph {

template <Directedness D> class AdjacencyMatrixGraph {

public:
  static constexpr Directedness DIRECTEDNESS = D;

  AdjacencyMatrixGraph();
  AdjacencyMatrixGraph(const AdjacencyMatrixGraph &graph);

  Vertex add_vertex();
  void remove_vertex(const Vertex &);

  Edge add_edge(const Vertex &, const Vertex &);
  void remove_edge(const Edge &);

  auto vertices() const;
  auto edges() const;
  auto out_edges(const Vertex &) const;
  auto in_edges(const Vertex &) const;

  Vertex get_vertex(const Id &id) const;
  Edge get_edge(const Id &id) const;

protected:
  using AdjacencyMap = std::unordered_map<Id, Id>;
  using AdjacencyMatrix = std::unordered_map<Id, AdjacencyMap>;
  using EdgeMap = std::unordered_map<Id, Edge>;

private:
  AdjacencyMatrix _adj;
  EdgeMap _edge_map;

  utils::IdManager _vertex_id_manager;
  utils::IdManager _edge_id_manager;
};
} // namespace graph

#include "matrix_graph.i.hpp"