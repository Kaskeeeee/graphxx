#pragma once
#include <limits>

namespace graph {
/// The id type that is used to identify vertices and edges
using Id = int;

/// Base class that represents and object that can be identified
/// through an ID
struct Identifiable {
  explicit constexpr Identifiable(Id id) : id{id} {};

  Id id;

  operator Id() const { return id; }
};

/// A simple struct that represents a vertex with an ID
struct Vertex : Identifiable {
  explicit constexpr Vertex(Id id) : Identifiable{id} {};

  /// @brief checks if the identifieable object is a valid one
  /// @return true if the id is non negative, false otherwise
  bool valid() const { return id >= 0; }

  bool operator==(const Vertex &other) const {
    return (!other.valid() && !valid()) || (id == other.id);
  }
};

/// A simple struct that represents an edge with ID and the vertices
/// it connects
struct Edge : Identifiable {
  explicit constexpr Edge(Id id, Vertex u, Vertex v)
      : Identifiable{id}, u{u}, v{v} {};

  explicit constexpr Edge() : Identifiable{-1}, u{Vertex(-1)}, v{Vertex(-1)} {};

  /// @brief checks if the identifieable object is a valid one
  /// @return true if the id is non negative, false otherwise
  bool valid() const { return id >= 0 && u.valid() && v.valid(); }

  bool operator==(const Edge &other) const {
    return (!other.valid() && !valid()) ||
           (id == other.id && u == other.u && v == other.v);
  }

  Vertex u, v;
};

/// Enum used to distinguish if the graph is directed or undirected
enum class GraphType {
  Directed = 0,
  Undirected = 1,
};

constexpr Id MIN_VALID_ID = 0;
constexpr Id MAX_VALID_ID = std::numeric_limits<Id>::max();
constexpr Vertex INVALID_VERTEX = Vertex(-1);
constexpr Edge INVALID_EDGE = Edge(-1, INVALID_VERTEX, INVALID_VERTEX);

} // namespace graph