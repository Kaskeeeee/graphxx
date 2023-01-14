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

#include <type_traits>

namespace graphxx {
template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
AdjacencyListGraph<Id, D, AttributesType...>::AdjacencyListGraph(){};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
AdjacencyListGraph<Id, D, AttributesType...>::AdjacencyListGraph(
    const AdjacencyListGraph &graph) {
  for (auto &&vertex : graph) {
    for (auto &&edge : vertex) {
      std::apply(
          [&](auto &&...attributes) {
            add_edge(source(edge), target(edge), attributes...)
          },
          elements_from_index<2>(edge));
    }
  }
};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::add_vertex(Id id) {
  for (auto i = Base::size(); i <= id; ++i) {
    Base::emplace_back();
  }
};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::add_edge(Id uid, Id vid,
                                                            Attributes attrs) {
  add_vertex(uid);
  add_vertex(vid);

  Base::operator[](uid).emplace_back(
      {uid, vid, elements_from_index<0>(attrs)...});
  if (DIRECTEDNESS == Directedness::UNDIRECTED) {
    Base::operator[](vid).emplace_back(
        {vid, uid, elements_from_index<0>(attrs)...});
  }
};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::remove_vertex(Id id) {
  if (id < size()) {
    Base::operator[](id).clear();
  }

  constexpr auto is_edge_to_id = [=](Edge edge) { return target(edge) == id; };

  for (size_t i = 0; i < size(); i++) {
    std::ranges::remove_if(Base::at(i), is_edge_to_id);
  }
};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
void AdjacencyListGraph<Id, D, AttributesType...>::remove_edge(Id uid, Id vid) {
  if (u < size() && v < size()) {
    std::ranges::remove_if(Base::at(u),
                           [=](Edge &&edge) { return target(edge) == v; });

    if (DIRECTEDNESS == Directedness::UNDIRECTED) {
      std::ranges::remove_if(at(v),
                             [=](Edge &&edge) { return target(edge) == u; });
    }
  }
};

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
Id AdjacencyListGraph<Id, D, AttributesType...>::source(Edge edge) {
  return std::get<0>(edge);
}

template <std::unsigned_integral Id, Directedness D, typename... AttributesType>
Id AdjacencyListGraph<Id, D, AttributesType...>::target(Edge edge) {
  return std::get<1>(edge);
}

} // namespace graph