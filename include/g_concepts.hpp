#pragma once
#include "g_base.hpp"

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
                           typename T::InternalStructIterator<Vertex>;
                           v.vertices();
                           requires Iterable<decltype(v.vertices()), Vertex>;
                         };

template <typename T>
concept EdgeIterable = requires(T v) {
                         typename T::InternalStructIterator<Edge>;
                         v.edges();
                         requires Iterable<decltype(v.edges()), Edge>;
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
concept Graph = VertexIterable<T> && EdgeIterable<T> && BasicGraphEditable<T>;
} // namespace concepts
} // namespace graph