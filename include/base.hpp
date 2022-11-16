#pragma once
#include <limits>

namespace graph {
/// The id type that is used to identify vertices and edges
using Id = int;

/// Base class that represents and object that can be identified
/// through an ID
struct Identifiable {
  explicit Identifiable(Id id) : id{id} {};

  Id id;

  /// @brief checks if the identifieable object is a valid one
  /// @return true if the id is non negative, false otherwise
  bool valid() { return id >= 0; }

  operator Id() const { return id; }
};

/// A simple struct that represents a vertex with an ID
struct Vertex : Identifiable {
  explicit Vertex(Id id) : Identifiable{id} {};

  bool operator==(const Vertex &other) const { return id == other.id; }
};

/// A simple struct that represents an edge with ID and the vertices
/// it connects
struct Edge : Identifiable {
  explicit Edge(Id id, Vertex u, Vertex v) : Identifiable{id}, u{u}, v{v} {};

  explicit Edge() : Identifiable{-1}, u{Vertex(-1)}, v{Vertex(-1)} {};

  bool operator==(const Edge &other) const {
    return id == other.id && u == other.u && v == other.v;
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

} // namespace graph