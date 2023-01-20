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

#include "exceptions.hpp"
#include "matrix_graph.hpp"

#include <algorithm>
namespace graphxx {
template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
AdjacencyMatrixGraph<IdType, D, AttributesType...>::AdjacencyMatrixGraph(){};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
AdjacencyMatrixGraph<IdType, D, AttributesType...>::AdjacencyMatrixGraph(
    const AdjacencyMatrixGraph &graph) {
  for (auto &&vertex : graph) {
    for (auto &&edge : vertex) {
      std::apply(
          [&](auto &&...attributes) {
            add_edge(source(edge), target(edge), attributes)
          },
          elements_from_index<2>(edge));
    }
  }
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_vertex(Id id) {
  Base::emplace_back();
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_vertex(Id id) {
  for (auto i = Base::size(); i <= id; ++i) {
    Base::emplace_back();
  }
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_edge(
    Id uid, Id vid, Attributes attrs) {
  add_node(uid);
  add_node(vid);

  Base::operator[](uid).emplace(vid, std::forward_as_tuple(uid, vid, attrs...));
  if (DIRECTEDNESS == Directedness::UNDIRECTED) {
    Base::operator[](vid).emplace(uid,
                                  std::forward_as_tuple(vid, uid, attrs...));
  }
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::remove_vertex(Id id) {
  if (id < Base::size()) {
    Base::operator[](id).clear();
  }

  for (size_t i = 0; i < Base::size(); i++) {
    auto &inner = Base::operator[](i);
    auto [start, end] = std::ranges::remove_if(
        inner, [&](auto &&edge) { return target(edge) == id; });
    inner.erase(start, end);
  }
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::remove_edge(Id u,
                                                                     Id v) {
  if (u < Base::size() && v < Base::size()) {
    auto &inner_u = Base::operator[](u);
    auto [start_u, end_u] = std::ranges::remove_if(
        inner_u, [&](auto &&edge) { return target(edge) == v; });
    inner_u.erase(start_u, end_u);

    if (DIRECTEDNESS == Directedness::UNDIRECTED) {
      auto &inner_v = Base::operator[](v);
      auto [start_v, end_v] = std::ranges::remove_if(
          inner_v, [&](Edge &&edge) { return target(edge) == u; });
      inner_v.erase(start_v, end_v);
    }
  }
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
Id AdjacencyMatrixGraph<Id, D, AttributesType...>::source(Edge edge) const {
  return std::get<0>(edge);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
Id AdjacencyMatrixGraph<Id, D, AttributesType...>::target(Edge edge) const {
  return std::get<1>(edge);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyMatrixGraph<Id, D, AttributesType...>::num_vertices() const {
  return Base::size();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyMatrixGraph<Id, D, AttributesType...>::num_edges() const {
  size_t count = 0;
  for (size_t i = 0; i < Base::size(); i++) {
    count += (*this)[i].size();
  }
  return count;
}

} // namespace graphxx