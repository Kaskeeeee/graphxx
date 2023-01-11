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
      requires T::DIRECTEDNESS == Directedness::DIRECTED || T::DIRECTEDNESS == Directedness::UNDIRECTED;
    };

template <typename T>
concept Graph =
    VertexIterable<T> && EdgeIterable<T> && OutEdgesIterable<T> &&
    InEdgesIterable<T> && GraphEditable<T> && DirectednessTaggable<T>;

} // namespace graph::concepts