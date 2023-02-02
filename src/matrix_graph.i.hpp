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
      add_edge(get_source(edge), get_target(edge),
               get_elements_from_index<2>(edge));
    }
  }
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_vertex() {
  _adj.emplace_back();
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_vertex(
    Vertex vertex) {
  for (auto i = _adj.size(); i <= vertex; ++i) {
    _adj.emplace_back();
  }
};

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::add_edge(
    Vertex source, Vertex target, Attributes attributes) {
  add_vertex(source);
  add_vertex(target);

  std::apply(
      [&](auto &&...props) {
        _adj[source].emplace(target,
                             std::forward_as_tuple(source, target, props...));
      },
      attributes);

  if (DIRECTEDNESS == Directedness::UNDIRECTED) {
    std::apply(
        [&](auto &&...props) {
          _adj[target].emplace(source,
                               std::forward_as_tuple(target, source, props...));
        },
        attributes);
  }
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::remove_vertex(
    Vertex vertex) {
  if (!has_vertex(vertex)) {
    return;
  }

  _adj[vertex].clear();

  for (size_t i = 0; i < _adj.size(); i++) {
    auto &inner = _adj[i];
    auto [start, end] = std::ranges::remove_if(
        inner, [&](auto &&edge) { return get_target(edge) == vertex; });
    inner.erase(start, end);
  }
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::remove_edge(
    Vertex source, Vertex target) {
  if (!has_edge(source, target)) {
    return;
  }

  auto &source_map = _adj[source];
  auto [start_source_map, end_source_map] = std::ranges::remove_if(
      source_map, [&](auto &&edge) { return get_target(edge) == target; });
  source_map.erase(start_source_map, end_source_map);

  if (DIRECTEDNESS == Directedness::UNDIRECTED) {
    auto &target_map = _adj[target];
    auto [start_target_map, end_target_map] = std::ranges::remove_if(
        target_map, [&](Edge &&edge) { return get_target(edge) == source; });
    target_map.erase(start_target_map, end_target_map);
  }
}
template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
const AdjacencyMatrixGraph<IdType, D, AttributesType...>::Edge &
AdjacencyMatrixGraph<IdType, D, AttributesType...>::get_edge(
    Vertex source, Vertex target) const {
  if (!has_edge(source, target)) {
    throw exceptions::NoSuchEdgeException();
  }

  return _adj[source][target];
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
void AdjacencyMatrixGraph<IdType, D, AttributesType...>::set_attributes(
    Vertex source, Vertex target, Attributes attributes) {
  if (!has_edge(target, source))
    return;

  set_elements_from_index<2>(_adj[source], attributes);
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
AdjacencyMatrixGraph<IdType, D, AttributesType...>::Attributes
AdjacencyMatrixGraph<IdType, D, AttributesType...>::get_attributes(
    Vertex source, Vertex target) const {
  if (!has_edge(source, target)) {
    throw exceptions::NoSuchEdgeException();
  }

  return get_elements_from_index<2>(get_edge(source, target));
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyMatrixGraph<Id, D, AttributesType...>::num_attributes() const {
  return sizeof...(AttributesType);
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
AdjacencyMatrixGraph<IdType, D, AttributesType...>::Vertex
AdjacencyMatrixGraph<IdType, D, AttributesType...>::get_source(
    const Edge &edge) const {
  return std::get<0>(edge);
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
AdjacencyMatrixGraph<IdType, D, AttributesType...>::Vertex
AdjacencyMatrixGraph<IdType, D, AttributesType...>::get_target(
    const Edge &edge) const {
  return std::get<1>(edge);
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
bool AdjacencyMatrixGraph<IdType, D, AttributesType...>::has_vertex(
    Vertex vertex) const {
  return vertex < _adj.size();
}

template <concepts::Identifier IdType, Directedness D,
          typename... AttributesType>
bool AdjacencyMatrixGraph<IdType, D, AttributesType...>::has_edge(
    Vertex source, Vertex target) const {
  return has_vertex(source) && has_vertex(target) &&
         _adj[source].contains(target);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyMatrixGraph<Id, D, AttributesType...>::num_vertices() const {
  return _adj.size();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyMatrixGraph<Id, D, AttributesType...>::num_edges() const {
  size_t count = 0;
  for (auto &[vertex, map] : _adj) {
    count += map.size();
  }
  return count;
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
const AdjacencyMatrixGraph<Id, D, AttributesType...>::EdgeMap &
AdjacencyMatrixGraph<Id, D, AttributesType...>::operator[](
    Vertex vertex) const {
  return _adj.at(vertex);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyMatrixGraph<Id, D, AttributesType...>::AdjacencyMatrix::iterator
AdjacencyMatrixGraph<Id, D, AttributesType...>::begin() {
  return _adj.begin();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyMatrixGraph<Id, D, AttributesType...>::AdjacencyMatrix::iterator
AdjacencyMatrixGraph<Id, D, AttributesType...>::end() {
  return _adj.end();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyMatrixGraph<Id, D, AttributesType...>::AdjacencyMatrix::const_iterator
AdjacencyMatrixGraph<Id, D, AttributesType...>::begin() const {
  return _adj.cbegin();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyMatrixGraph<Id, D, AttributesType...>::AdjacencyMatrix::const_iterator
AdjacencyMatrixGraph<Id, D, AttributesType...>::end() const {
  return _adj.cend();
}

} // namespace graphxx