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

#include "base.hpp"
#include "exceptions.hpp"
#include "list_graph.hpp"
#include "utils/tuple_utils.hpp"

#include <algorithm>
#include <type_traits>

namespace graphxx {
template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyListGraph<Id, D, AttributesType...>::AdjacencyListGraph(){};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
AdjacencyListGraph<Id, D, AttributesType...>::AdjacencyListGraph(
    const AdjacencyListGraph &graph) {
  for (auto &&vertex : graph) {
    for (auto &&edge : vertex) {
      std::apply(
          [&](auto &&...attributes) {
            add_edge(source(edge), target(edge), attributes...);
          },
          elements_from_index<2>(edge));
    }
  }
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::add_vertex() {
  Base::emplace_back();
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::add_vertex(Id id) {
  for (auto i = Base::size(); i <= id; ++i) {
    Base::emplace_back();
  }
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::add_edge(Id uid, Id vid,
                                                            Attributes attrs) {
  add_vertex(uid);
  add_vertex(vid);

  std::apply(
      [&](auto &&...props) {
        Base::operator[](uid).emplace_back(uid, vid, props...);
      },
      attrs);
  if (DIRECTEDNESS == Directedness::UNDIRECTED) {
    std::apply(
        [&](auto &&...props) {
          Base::operator[](vid).emplace_back(vid, uid, props...);
        },
        attrs);
  }
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::remove_vertex(Id id) {
  if (id < Base::size()) {
    Base::operator[](id).clear();
  }

  for (size_t i = 0; i < Base::size(); i++) {
    auto &inner = Base::operator[](i);
    auto [start, end] = std::ranges::remove_if(
        inner, [&](auto edge) { return target(edge) == id; });
    inner.erase(start, end);
  }
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::remove_edge(Id u, Id v) {
  if (u < Base::size() && v < Base::size()) {
    auto &inner = Base::operator[](u);
    auto [start, end] = std::ranges::remove_if(
        inner, [&](auto &&edge) { return target(edge) == v; });
    inner.erase(start, end);

    if (DIRECTEDNESS == Directedness::UNDIRECTED) {
      auto &other_inner = Base::operator[](v);
      auto [other_start, other_end] = std::ranges::remove_if(
          other_inner, [&](auto &&edge) { return target(edge) == u; });
      other_inner.erase(other_start, other_end);
    }
  }
};

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::set_attributes(
    Id u, Id v, Attributes attrs) {
  if (u < Base::size() && v < Base::size()) {
    auto it = std::ranges::find(Base::operator[](u),
                                [&](auto &&edge) { return target(edge) == v; });

    for (size_t i = 2; i < sizeof...(AttributesType); i++) {
    }
  }
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyListGraph<Id, D, AttributesType...>::num_attributes() const {
  return sizeof...(AttributesType);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
Id AdjacencyListGraph<Id, D, AttributesType...>::source(Edge edge) const {
  return std::get<0>(edge);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
Id AdjacencyListGraph<Id, D, AttributesType...>::target(Edge edge) const {
  return std::get<1>(edge);
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyListGraph<Id, D, AttributesType...>::num_vertices() const {
  return Base::size();
}

template <concepts::Identifier Id, Directedness D, typename... AttributesType>
size_t AdjacencyListGraph<Id, D, AttributesType...>::num_edges() const {
  size_t count = 0;
  for (size_t i = 0; i < Base::size(); i++) {
    count += (*this)[i].size();
  }
  return count;
}

} // namespace graphxx