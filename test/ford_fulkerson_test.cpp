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

#if 0

#include "base.hpp"
#include "catch.hpp"
#include "ford_fulkerson.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

namespace ford_fulkerson_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Ford Fulkerson maximum flow for list graphs",
          "[ford_fulkerson][list_graph]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a); // 0->1
  graph.add_edge(s, d); // 0->4
  graph.add_edge(a, b); // 1->2
  graph.add_edge(b, t); // 2->5
  graph.add_edge(c, t); // 3->5
  graph.add_edge(d, b); // 4->2
  graph.add_edge(d, c); // 4->3

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
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

    REQUIRE_THROWS(ford_fulkerson::visit(graph, s, t));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    graph.set_attributes(s, a, {8});
    graph.set_attributes(s, d, {3});
    graph.set_attributes(a, b, {9});
    graph.set_attributes(b, t, {2});
    graph.set_attributes(c, t, {5});
    graph.set_attributes(d, b, {7});
    graph.set_attributes(d, c, {4});

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 6);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for list graphs",
          "[ford_fulkerson][list_graph]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a); // 0->1
  graph.add_edge(s, d); // 0->4
  graph.add_edge(a, b); // 1->2
  graph.add_edge(b, t); // 2->5
  graph.add_edge(c, t); // 3->5
  graph.add_edge(d, b); // 4->2
  graph.add_edge(d, c); // 4->3

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
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

    REQUIRE_THROWS(ford_fulkerson::visit(graph, s, t));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    graph.set_attributes(s, a, {8});
    graph.set_attributes(s, d, {3});
    graph.set_attributes(a, b, {9});
    graph.set_attributes(b, t, {2});
    graph.set_attributes(c, t, {5});
    graph.set_attributes(d, b, {7});
    graph.set_attributes(d, c, {4});

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 6);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for undirected list graphs",
          "[ford_fulkerson][list_graph][undirected]") {
  using Graph =
      AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a); // 0->1
  graph.add_edge(s, d); // 0->4
  graph.add_edge(a, b); // 1->2
  graph.add_edge(b, t); // 2->5
  graph.add_edge(c, t); // 3->5
  graph.add_edge(d, b); // 4->2
  graph.add_edge(d, c); // 4->3

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
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

    REQUIRE_THROWS(ford_fulkerson::visit(graph, s, t));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    graph.set_attributes(s, a, {8});
    graph.set_attributes(s, d, {3});
    graph.set_attributes(a, b, {9});
    graph.set_attributes(b, t, {2});
    graph.set_attributes(c, t, {5});
    graph.set_attributes(d, b, {7});
    graph.set_attributes(d, c, {4});

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 6);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for undirected matrix graphs",
          "[ford_fulkerson][matrix_graph][undirected]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a); // 0->1
  graph.add_edge(s, d); // 0->4
  graph.add_edge(a, b); // 1->2
  graph.add_edge(b, t); // 2->5
  graph.add_edge(c, t); // 3->5
  graph.add_edge(d, b); // 4->2
  graph.add_edge(d, c); // 4->3

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
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

    REQUIRE_THROWS(ford_fulkerson::visit(graph, s, t));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        graph.set_attributes(graph.get_source(edge), graph.get_target(edge),
                             {1});
      }
    }

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    graph.set_attributes(s, a, {8});
    graph.set_attributes(s, d, {3});
    graph.set_attributes(a, b, {9});
    graph.set_attributes(b, t, {2});
    graph.set_attributes(c, t, {5});
    graph.set_attributes(d, b, {7});
    graph.set_attributes(d, c, {4});

    auto result = ford_fulkerson::visit(graph, s, t);

    REQUIRE(result.max_flow == 6);
  }
}
} // namespace ford_fulkerson_test

#endif