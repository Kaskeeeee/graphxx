#pragma once

namespace graph {
using Id = int;

struct Identifiable {
  Id id;

  bool valid() { return id >= 0; }
};

struct Vertex : Identifiable {
  explicit Vertex(Id id) : Identifiable{id} {};
};

struct Edge : Identifiable {
  Id source, target;
};

enum class GraphType {
  Directed = 0,
  Undirected = 1,
};
} // namespace graph