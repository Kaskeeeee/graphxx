/**
 * @file This file contains the unit tests for Dijkstra algorithm
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea Cinelli. All rights reserved.
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
#include "dijkstra.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

namespace dijkstra_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Dijkstra shortest paths for directed list graph",
          "[dijkstra][list_graph][directed]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b); // 0->1
  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(c, e); // 2->4
  graph.add_edge(c, f); // 2->5
  graph.add_edge(d, f); // 3->5
  graph.add_edge(e, f); // 4->5

  /*
    A-->B
    |   |
    |   v
    --->C-->D------v
        --->E------v
        ---------->F
  */

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, b, {-1});

    REQUIRE_THROWS(dijkstra(graph, a));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {1});

    auto distances = dijkstra(graph, a);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 2);
    REQUIRE(distances[e].distance == 2);
    REQUIRE(distances[f].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }

  graph.add_edge(a, a); // 0->0
  graph.add_edge(d, d); // 3->3
  graph.add_edge(e, c); // 4->2

  /*
   <->
    A-->B
    |   |
    |   v  <->
    --->C-->D-------v
        --->E------>F
        <----
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (Vertex<Graph> vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {7});
    graph.set_attributes(d, d, {0});
    graph.set_attributes(e, c, {0});

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].distance == 0);
    REQUIRE(result[b].distance == 1);
    REQUIRE(result[c].distance == 1);
    REQUIRE(result[d].distance == 2);
    REQUIRE(result[e].distance == 2);
    REQUIRE(result[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights, now with cycles") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }
}

TEST_CASE("Dijkstra shortest paths for directed matrix graph",
          "[dijkstra][matrix_graph][directed]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b); // 0->1
  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(c, e); // 2->4
  graph.add_edge(c, f); // 2->5
  graph.add_edge(d, f); // 3->5
  graph.add_edge(e, f); // 4->5

  /*
    A-->B
    |   |
    |   v
    --->C-->D------v
        --->E------v
        ---------->F
  */

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, b, {-1});

    REQUIRE_THROWS(dijkstra(graph, a));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {1});

    auto distances = dijkstra(graph, a);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 2);
    REQUIRE(distances[e].distance == 2);
    REQUIRE(distances[f].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }

  graph.add_edge(a, a); // 0->0
  graph.add_edge(d, d); // 3->3
  graph.add_edge(e, c); // 4->2

  /*
   <->
    A-->B
    |   |
    |   v  <->
    --->C-->D-------v
        --->E------>F
        <----
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (Vertex<Graph> vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {7});
    graph.set_attributes(d, d, {0});
    graph.set_attributes(e, c, {0});

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].distance == 0);
    REQUIRE(result[b].distance == 1);
    REQUIRE(result[c].distance == 1);
    REQUIRE(result[d].distance == 2);
    REQUIRE(result[e].distance == 2);
    REQUIRE(result[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights, now with cycles") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }
}

TEST_CASE("Dijkstra shortest paths for undirected list graph",
          "[dijkstra][list_graph][undirected]") {
  using Graph =
      AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b); // 0->1
  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(c, e); // 2->4
  graph.add_edge(c, f); // 2->5
  graph.add_edge(d, f); // 3->5
  graph.add_edge(e, f); // 4->5

  /*
    A---B
    |   |
    |   |
    ----C---D------|
        ----E------|
        -----------F
  */

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, b, {-1});

    REQUIRE_THROWS(dijkstra(graph, a));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {1});

    auto distances = dijkstra(graph, a);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 2);
    REQUIRE(distances[e].distance == 2);
    REQUIRE(distances[f].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }
}

TEST_CASE("Dijkstra shortest paths for undirected matrix graph",
          "[dijkstra][matrix_graph][undirected]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b); // 0->1
  graph.add_edge(a, c); // 0->2
  graph.add_edge(b, c); // 1->2
  graph.add_edge(c, d); // 2->3
  graph.add_edge(c, e); // 2->4
  graph.add_edge(c, f); // 2->5
  graph.add_edge(d, f); // 3->5
  graph.add_edge(e, f); // 4->5

  /*
    A---B
    |   |
    |   |
    ----C---D------|
        ----E------|
        -----------F
  */

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(a, b, {-1});

    REQUIRE_THROWS(dijkstra(graph, a));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    graph.set_attributes(c, f, {1});

    auto distances = dijkstra(graph, a);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 2);
    REQUIRE(distances[e].distance == 2);
    REQUIRE(distances[f].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = dijkstra(graph, a);

    REQUIRE(result[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }
}
} // namespace dijkstra_test