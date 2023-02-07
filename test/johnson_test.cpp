/**
 * @file This file contains the unit tests for Johnson algorithm
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
#include "matrix_graph.hpp"

namespace johnson_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Johnson shortest paths for directed list graph",
          "[johnson][list_graph][directed]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, a); // 1->0
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(d, b); // 3->1

  /*
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, c, {-2});

    auto tree = johnson(graph);
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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, c, {-2});

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE(tree[b][c].parent == a);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  graph.add_edge(c, a); // 2->0
  graph.add_edge(d, d); // 3->3

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

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

TEST_CASE("Johnson shortest paths for directed matrix graph",
          "[johnson][matrix_graph][directed]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, a); // 1->0
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(d, b); // 3->1

  /*
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, c, {-2});

    auto tree = johnson(graph);
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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, c, {-2});

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE(tree[b][c].parent == a);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  graph.add_edge(c, a); // 2->0
  graph.add_edge(d, d); // 3->3

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

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

TEST_CASE("Johnson shortest paths for undirected list graph",
          "[johnson][list_graph][undirected]") {
  using Graph =
      AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, a); // 1->0
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(d, b); // 3->1

  /*
    A-------C-------|
    |       |       |
    |       |       |
    B-------|       |
    |               |
    |               |
    D---------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    REQUIRE(tree[a][b].distance == 1);
  }

  SECTION("finds the parent nodes with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE((tree[a][d].parent == c || tree[a][d].parent == b));
    REQUIRE((tree[a][b].parent == d || tree[a][b].parent == a));
  }
}

TEST_CASE("Johnson shortest paths for undirected matrix graph",
          "[johnson][matrix_graph][undirected]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, a); // 1->0
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(d, b); // 3->1

  /*
    A-------C-------|
    |       |       |
    |       |       |
    B-------|       |
    |               |
    |               |
    D---------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

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
    REQUIRE(tree[a][b].distance == 1);
  }

  SECTION("finds the parent nodes with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto tree = johnson(graph);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX<Graph>);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX<Graph>);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE((tree[a][d].parent == c || tree[a][d].parent == b));
    REQUIRE((tree[a][b].parent == d || tree[a][b].parent == a));
  }
}
} // namespace johnson_test