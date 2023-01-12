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

namespace graph::concepts {

template <typename T>
concept VertexIterable = requires(T v) {
                           v.vertices();
                           requires std::ranges::range<decltype(v.vertices())>;
                           {
                             *(v.vertices().begin())
                             } -> std::convertible_to<Vertex>;
                         };

template <typename T>
concept EdgeIterable = requires(T v) {
                         v.edges();
                         requires std::ranges::range<decltype(v.edges())>;
                         { *(v.edges().begin()) } -> std::convertible_to<Edge>;
                       };

template <typename T>
concept OutEdgesIterable =
    requires(T v, Vertex vertex) {
      v.out_edges(vertex);
      requires std::ranges::range<decltype(v.out_edges(vertex))>;
      { *(v.out_edges(vertex).begin()) } -> std::convertible_to<Edge>;
    };

template <typename T>
concept InEdgesIterable =
    requires(T v, Vertex vertex) {
      v.in_edges(vertex);
      requires std::ranges::range<decltype(v.in_edges(vertex))>;
      { *(v.in_edges(vertex).begin()) } -> std::convertible_to<Edge>;
    };

template <typename T>
concept GraphEditable = requires(T v, Vertex v1, Vertex v2, Edge e, Id id) {
                          v.add_vertex();
                          v.add_edge(v1, v2);
                          v.remove_vertex(v1);
                          v.remove_edge(e);
                          v.get_vertex(id);
                          v.get_edge(id);
                        };

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename Container, typename Key>
concept Subscriptable = requires(Container c, Key k) { c[k]; };

template <typename T>
concept DirectednessTaggable =
    requires(T t) {
      T::DIRECTEDNESS;
      requires T::DIRECTEDNESS == Directedness::DIRECTED ||
                       T::DIRECTEDNESS == Directedness::UNDIRECTED;
    };

template <typename T>
concept Graph =
    VertexIterable<T> && EdgeIterable<T> && OutEdgesIterable<T> &&
    InEdgesIterable<T> && GraphEditable<T> && DirectednessTaggable<T>;

} // namespace graph::concepts