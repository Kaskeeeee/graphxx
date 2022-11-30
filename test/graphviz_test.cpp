#include "base.hpp"
#include "catch.hpp"
#include "io/graphviz.hpp"
#include "list_graph.hpp"

namespace graphviz_test {
using namespace graph;
using namespace graph::io;

TEST_CASE("Graphviz suite tests", "[graphviz]") {
  AdjacencyListGraph<DirectedGraph> g{};
}
} // namespace graphviz_test
