#pragma once
#include "base.hpp"

#include <concepts>
#include <ranges>

namespace graph::concepts {

template <typename T>
concept VertexIterable = requires(T v) {
                           requires std::ranges::range<decltype(v.vertices())>;
                           {
                             *(v.vertices().begin())
                             } -> std::convertible_to<Vertex>;
                         };

template <typename T>
concept EdgeIterable = requires(T v) {
                         requires std::ranges::range<decltype(v.edges())>;
                         { *(v.edges().begin()) } -> std::convertible_to<Edge>;
                       };

template <typename T>
concept OutEdgesIterable =
    requires(T v, const Vertex &vertex) {
      requires std::ranges::range<decltype(v.out_edges(vertex))>;
      { *(v.out_edges(vertex).begin()) } -> std::convertible_to<Edge>;
    };

template <typename T>
concept InEdgesIterable =
    requires(T v, const Vertex &vertex) {
      requires std::ranges::range<decltype(v.in_edges(vertex))>;
      { *(v.in_edges(vertex).begin()) } -> std::convertible_to<Edge>;
    };

template <typename T>
concept GraphEditable =
    requires(T v, const Vertex &v1, const Vertex &v2, const Edge &e) {
      v.add_vertex();
      v.add_edge(v1, v2);
      v.remove_vertex(v1);
      v.remove_edge(e);
    };

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename Container, typename Key, typename Value>
concept Subscriptable =
    requires(const Container &c, const Key &k, const Value &v) {
      { c[k] } -> std::convertible_to<Value>;
    };

template <typename T>
concept Graph = VertexIterable<T> && EdgeIterable<T> && OutEdgesIterable<T> &&
                InEdgesIterable<T> && GraphEditable<T>;

} // namespace graph::concepts