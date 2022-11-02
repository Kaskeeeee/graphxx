#pragma once

namespace graph {
using Id = int;

struct Identifiable {
  Id id;

  bool valid() { return id >= 0; }
};

struct Vertex : Identifiable {};

struct Edge : Identifiable {
  Id source, target;
};
} // namespace graph