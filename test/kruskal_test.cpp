#include "base.hpp"
#include "catch.hpp"
#include "kruskal.hpp"
#include "list_graph.hpp"

namespace kruskal_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Kruskal", "[kruskal]") {
  AdjacencyListGraph<DirectedGraph> g{};
  std::unordered_map<Id, int> weights;
  kruskal::visit(g, weights);
}
} // namespace kruskal_test