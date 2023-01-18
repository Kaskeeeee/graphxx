/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
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

#include "algorithms/algorithms_base.hpp"
#include "base.hpp"
#include "bfs.hpp"
#include "catch.hpp"
#include "list_graph.hpp"

namespace bfs_test {
using namespace graphxx;
using namespace algorithms;

TEST_CASE("BFS Tree correct visited order", "[BFS]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d, e };

  g.add_edge(a, b); // 0->1
  g.add_edge(a, c); // 0->2
  g.add_edge(a, d); // 0->3
  g.add_edge(b, c); // 1->2
  g.add_edge(d, e); // 3->4

  /*
    A--->B--->C
    --------->
    |
    |
    ---->D--->E
  */

  SECTION("check if all nodes were processed") {
    auto tree = graphxx::algorithms::bfs::visit(g, a);

    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      REQUIRE(tree[vertex].status == VertexStatus::PROCESSED);
    }
  }

  SECTION("check if all the distances from the source are correct") {
    auto tree = graphxx::algorithms::bfs::visit(g, a);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 2);
  }

  SECTION("check if all parent node are correct") {
    auto tree = graphxx::algorithms::bfs::visit(g, a);

    REQUIRE(tree[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  g.add_edge(a, a); // 0->0
  g.add_edge(b, b); // 1->1
  g.add_edge(c, b); // 2->1

  /*
   <->
    A--->B--->C
    |   <->   |
    |    <-----
    --------->
    |
    |
    ---->D--->E
  */

  SECTION("check if all the distances from the source are correct, now with "
          "cycles") {
    auto tree = graphxx::algorithms::bfs::visit(g, a);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 2);
  }

  SECTION("check if all parent node are correct, now with cycles") {
    auto tree = graphxx::algorithms::bfs::visit(g, a);

    REQUIRE(tree[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  SECTION("check if visit with function work properly") {
    std::vector<DefaultIdType> vertices;

    auto tree = graphxx::algorithms::bfs::visit(
        g, a, [&](DefaultIdType v) { vertices.push_back(v); });

    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      REQUIRE(std::find(vertices.begin(), vertices.end(), vertex) !=
              vertices.end());
    }
  }

  SECTION("work in undirected graph") {
    using Graph = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
    Graph g1{};

    g1.add_edge(a, b); // 0->1
    g1.add_edge(a, c); // 0->2
    g1.add_edge(a, d); // 0->3
    g1.add_edge(b, c); // 1->2
    g1.add_edge(d, e); // 3->4

    auto tree = graphxx::algorithms::bfs::visit(g1, c);

    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      REQUIRE(tree[vertex].status == VertexStatus::PROCESSED);
    }
  }
}
} // namespace bfs_test