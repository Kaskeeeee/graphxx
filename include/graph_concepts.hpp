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

#include <concepts>
#include <ranges>

namespace graphxx::concepts {

template <typename T> using InnerValue = std::ranges::range_value_t<T>;

template <typename T>
concept IsRangeOfRanges = std::ranges::random_access_range<T> &&
                          std::ranges::forward_range<InnerValue<T>>;

template <typename G>
concept HasGraphBasicTraits =
    requires(G) {
      typename G::Id;
      typename G::Attributes;
      typename G::Edge;
      requires G::DIRECTEDNESS == Directedness::DIRECTED ||
                       G::DIRECTEDNESS == Directedness::UNDIRECTED;
    };

template <typename G>
concept HasGraphBasicMethods =
    requires(G g, typename G::Id id, typename G::Attributes attributes,
             typename G::Edge edge) {
      g.add_vertex(id);
      g.add_edge(id, id, attributes);
      g.remove_vertex(id);
      g.remove_edge(id, id);
      { g.num_vertices() } -> std::same_as<size_t>;
      { g.num_edges() } -> std::same_as<size_t>;
      { g.source(edge) } -> std::convertible_to<typename G::Id>;
      { g.target(edge) } -> std::convertible_to<typename G::Id>;
    };

template <typename G>
concept Graph =
    IsRangeOfRanges<G> && HasGraphBasicTraits<G> && HasGraphBasicMethods<G>;

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename Container, typename Key>
concept Subscriptable = requires(Container c, Key k) { c[k]; };

} // namespace graphxx::concepts