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

#include "base.hpp"
#include "catch.hpp"
#include "johnson.hpp"
#include "list_graph.hpp"

#include <map>
#include <tuple>

namespace johnson_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Johnson shortest paths", "[johnson]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;

  g.add_edge(a, c); // 0->2
  g.add_edge(b, a); // 1->0
  g.add_edge(b, c); // 1->2
  g.add_edge(c, d); // 2->3
  g.add_edge(d, b); // 3->1

  /*
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
  */

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.source(e), g.target(e)}];
  };

  SECTION("finds the shortest path length with all positive weights") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto tree = johnson::visit(g, get_weight);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto tree = johnson::visit(g, get_weight);

    //REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    //REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    //REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    //REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, c}] = -2;

    auto tree = johnson::visit(g, get_weight);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == -2);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == -1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == -1);
    REQUIRE(tree[a][b].distance == 0);
  }

  SECTION("finds the parent nodes with one negative weight") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, c}] = -2;

    auto tree = johnson::visit(g, get_weight);

    //REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    //REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    //REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    //REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE(tree[b][c].parent == a);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  /*
    SECTION("throws on negative cycle found") {
      for (auto edge : g.edges()) {
        weights[edge] = 1;
      }

      weights[c_to_d] = -1;
      weights[d_to_b] = -1;

      REQUIRE_THROWS(floyd_warshall::visit(g, weights));
    }
  */

  g.add_edge(c, a); // 2->0
  g.add_edge(d, d); // 3->3

  /*
    <--------
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
   <->
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto tree = johnson::visit(g, get_weight);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][a].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights, now with cycles") {
    for (auto vertex : g) {
      for (auto edge : vertex) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto tree = johnson::visit(g, get_weight);

    //REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    //REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    //REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    //REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == b || tree[b][c].parent == a));
    REQUIRE(tree[c][a].parent == c);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }
}
} // namespace johnson_test