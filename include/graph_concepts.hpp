#pragma once
#include "base.hpp"

#include <concepts>

namespace graph {
namespace concepts {
template <typename T, typename RT>
concept Iterable = requires(T v) {
                     v.begin();
                     v.end();
                     v.begin()++;
                     { *v.begin() } -> std::convertible_to<RT>;
                   };

template <typename T>
concept VertexIterable = requires(T v) {
                           v.vertices();
                           requires Iterable<decltype(v.vertices()), Vertex>;
                         };

template <typename T>
concept EdgeIterable = requires(T v) {
                         v.edges();
                         requires Iterable<decltype(v.edges()), Edge>;
                       };

template <typename T>
concept OutEdgesIterable = requires(T v) {
                             v.out_edges();
                             requires Iterable<decltype(v.out_edges()), Edge>;
                           };

template <typename T>
concept InEdgesIterable = requires(T v) {
                            v.in_edges();
                            requires Iterable<decltype(v.in_edges()), Edge>;
                          };

template <typename T>
concept BasicGraphEditable =
    requires(T v, const Vertex &v1, const Vertex &v2, const Edge &e) {
      v.add_vertex();
      v.add_edge(v1, v2);
      v.remove_vertex(v1);
      v.remove_edge(e);
    };

template <typename T>
concept Graph = VertexIterable<T> && EdgeIterable<T> && OutEdgesIterable<T> &&
                InEdgesIterable<T> && BasicGraphEditable<T>;
} // namespace concepts
} // namespace graph