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

#include <tuple>
#include <unordered_map>
#include <vector>

namespace graphxx {

template <std::unsigned_integral IdType = DefaultIdType,
          Directedness D = Directedness::DIRECTED, typename... AttributesType>
class AdjacencyMatrixGraph
    : public std::vector<std::unordered_map<
          IdType, std::tuple<IdType, IdType, AttributesType>>> {
public:
  using Id = IdType;
  using Edge = std::tuple<Id, Id, AttributesType...>;
  using Attributes = std::tuple<AttributesType...>;
  using InnerRage = std::unordered_map<Id, Edge>;
  using Base = std::vector<InnerRage>;

  static constexpr Directedness DIRECTEDNESS = D;

  AdjacencyMatrixGraph();
  AdjacencyMatrixGraph(const AdjacencyMatrixGraph &graph);

  void add_vertex(Id);
  void remove_vertex(Id);

  void add_edge(Id, Id, Attributes);
  void remove_edge(Id, Id);

  Id source(Edge);
  Id target(Edge);
};
} // namespace graphxx

#include "matrix_graph.i.hpp"