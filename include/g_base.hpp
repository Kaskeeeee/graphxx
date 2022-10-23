#pragma once

namespace graph {
using Id = int;

struct Vertex {
  Id id;
};

struct Edge {
  Id id;
  Id source, target;
};
} // namespace graph