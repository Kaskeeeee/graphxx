/**
 * @file This file is the header of the adjacency list graph
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "base.hpp"           // DefaultIdType
#include "graph_concepts.hpp" // concepts::Identifier

#include <cstdint> // size_t
#include <list>    // std::list
#include <tuple>   // std::tuple
#include <vector>  // std::vector

namespace graphxx {

/// @brief Represents a graph implemented with an adjacency list.
///        Vertices are represented by id that correspond to positions in a
///        vector. Edges are tuple composed by source vertex, target vertex and
///        a variable number of attributes.
/// @tparam ...AttributesType Types of edges attributes. They can be a variable
/// number.
/// @tparam D Graph directedness
/// @tparam IdType Numeric type representing vertices
template <concepts::Identifier IdType = DefaultIdType,
          Directedness D = Directedness::DIRECTED, typename... AttributesType>
class AdjacencyListGraph {
public:
  using Vertex = IdType;
  using Edge = std::tuple<Vertex, Vertex, AttributesType...>;
  using Attributes = std::tuple<AttributesType...>;

  using EdgeList = std::vector<Edge>;
  using AdjacencyList = std::vector<EdgeList>;

  /// @brief Indicates whether the graph is directed or undirected
  static constexpr Directedness DIRECTEDNESS = D;

  /// @brief Adds a new vertex to the graph.
  ///        The vertex id will be the index of the first free position in the
  ///        adjacency list
  void add_vertex();

  /// @brief Adds a new vertex with specific id to the graph.
  ///        Warning! This operation creates in adjacency list all the positions
  ///        with index < vertex with empty edge list.
  /// @param vertex Id of the vertex to insert.
  void add_vertex(Vertex vertex);

  /// @brief  Remove a vertex from the graph.
  ///         Warning! This operation clear the edge list of the vertex and
  ///         removes all the vertex related edges. The vertex position is still
  ///         present in adjacency list.
  /// @param vertex Id of the vertex to remove.
  void remove_vertex(Vertex vertex);

  /// @brief Add new edge to the graph.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @param attributes Tuple containing edge attributes.
  void add_edge(Vertex source, Vertex target, Attributes attributes = {});

  /// @brief Remove edge from the graph.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  void remove_edge(Vertex source, Vertex target);

  /// @brief Get edge tuple based on the vertices it connects.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @return A tuple composed of source id, destination id and a variable
  /// number of attributes.
  const Edge &get_edge(Vertex source, Vertex target) const;

  /// @brief Get edge source vertex.
  /// @param edge The edge to extract the source from.
  /// @return Id of the source vertex.
  Vertex get_source(const Edge &edge) const;

  /// @brief Get edge target vertex.
  /// @param edge The edge to extract the target from.
  /// @return Id of the target vertex.
  Vertex get_target(const Edge &edge) const;

  /// @brief Updates edge attributes tuple.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @param attributes Tuple containing new edge attributes.
  void set_attributes(Vertex source, Vertex target, Attributes attributes);

  /// @brief Retrives edge attributes tuple.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @return Edges attributes tuple.
  Attributes get_attributes(Vertex source, Vertex target) const;

  /// @brief Get number of edges attributes.
  /// @return Number of edges attributes.
  [[nodiscard]] constexpr size_t num_attributes() const;

  /// @brief Get number of vertices in the graph.
  /// @return Number of vertices.
  [[nodiscard]] size_t num_vertices() const;

  /// @brief Get number of edges in the graph.
  /// @return Number of edges.
  [[nodiscard]] size_t num_edges() const;

  /// @brief Checks if a vertex is present in the graph.
  /// @param vertex Vertex id.
  /// @return True if the vertex exists.
  bool has_vertex(Vertex vertex) const;

  /// @brief Checks if an edge is present in the graph.
  /// @param source Id of the source vertex.
  /// @param target Id of the target vertex.
  /// @return True if the edge exists.
  bool has_edge(Vertex source, Vertex target) const;

  /// @brief Retrieves the adjacency list of a vertex.
  /// @param vertex Vertex id.
  /// @return Edge list of a vertex.
  const EdgeList &operator[](Vertex vertex) const;

  /// @brief Returns an iterator that points to the first element in the
  /// adjacency list.
  typename AdjacencyList::iterator begin();

  /// @brief Returns an iterator that points one past the last element in the
  /// adjacency list.
  typename AdjacencyList::iterator end();

  /// @brief Returns an iterator that points to the first element in the
  /// adjacency list.
  typename AdjacencyList::const_iterator begin() const;

  /// @brief Returns an iterator that points one past the last element in the
  /// adjacency list.
  typename AdjacencyList::const_iterator end() const;

private:
  /// @brief The adjacency list.
  AdjacencyList _adj;
};

} // namespace graphxx

#include "list_graph.i.hpp"