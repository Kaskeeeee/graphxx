#include "algorithms/bfs.hpp"
#include "catch.hpp"
#include "list_graph.hpp"

namespace bfs_test {
using namespace graph;

TEST_CASE("BFS Tree builds correctly", "[BFS]") {
  AdjacencyListGraph<GraphType::Directed> g;
  graph::algorithms::BFS bfs{g};

  SECTION("") {}
}
} // namespace bfs_test