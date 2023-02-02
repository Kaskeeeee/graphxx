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

#include "catch.hpp"
#include "list_graph.hpp"

namespace list_graph_test {

using namespace graphxx;

TEST_CASE("build directed list graph", "[list_graph][directed][build]") {
  AdjacencyListGraph<unsigned long, Directedness::DIRECTED> g;

  SECTION("empty graph has no vertices or edges") {
    REQUIRE(g.num_vertices() == 0);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("add vertex to graph") {
    const size_t NUM_VERTICES = 10;

    SECTION("add vertex sequentially") {
      for (size_t i = 0; i < NUM_VERTICES; i++)
        g.add_vertex();

      REQUIRE(g.num_vertices() == NUM_VERTICES);

      for (size_t i = 0; i < NUM_VERTICES; i++)
        REQUIRE(g.has_vertex(i));
    }

    SECTION("add vertex add nodes up to a specific index") {
      g.add_vertex(NUM_VERTICES);

      REQUIRE(g.num_vertices() == NUM_VERTICES + 1);
      
      for (size_t i = 0; i <= NUM_VERTICES; i++)
        REQUIRE(g.has_vertex(i));
    }

    SECTION("removing vertices does not change the adjacency list size") {
      g.add_vertex(NUM_VERTICES);
      g.remove_vertex(NUM_VERTICES - 1);
      REQUIRE(g.num_vertices() == NUM_VERTICES + 1);
    }
  }

  SECTION("add edge to graph") {
    g.add_vertex();
    g.add_vertex();
    g.add_edge(0, 1);
    g.add_edge(0, 0);
    g.add_edge(1, 1);

    SECTION("add edges between existing vertices") {
      REQUIRE(g.num_vertices() == 2);
      REQUIRE(g.num_edges() == 3);
    }

    SECTION("add edges between non-existing vertices") {
      g.add_edge(2, 4);
      REQUIRE(g.num_vertices() == 5);
      REQUIRE(g.num_edges() == 4);
    }

    SECTION("remove edge from graph") {
      g.remove_edge(0, 1);
      REQUIRE(g.num_vertices() == 2);
      REQUIRE(g.num_edges() == 2);
    }

    SECTION("remove vertex connected by an edge") {
      g.remove_vertex(1);
      REQUIRE(g.num_vertices() == 2);
      REQUIRE(g.num_edges() == 1);
    }

    SECTION("remove non-existing edge") {
      g.remove_edge(1, 0);
      REQUIRE(g.num_vertices() == 2);
      REQUIRE(g.num_edges() == 3);
    }
  }

  SECTION("check num vertices and num edges") {
    REQUIRE(g.num_vertices() == 0);
    REQUIRE(g.num_edges() == 0);

    for (int i = 0; i < 100; i++) {
      g.add_vertex(i);
    }

    REQUIRE(g.num_vertices() == 100);
    REQUIRE(g.num_edges() == 0);

    for (int i = 0; i < 100; i++) {
      g.add_edge(i, i);
    }

    REQUIRE(g.num_vertices() == 100);
    REQUIRE(g.num_edges() == 100);
  }
}

TEST_CASE("build directed list graph with attributes", "[list_graph][directed][build]") {
    AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int, float> g;
    g.add_edge(0, 1, {0, 0.1});
    REQUIRE(std::get<1>(g.get_attributes(0, 1)) == 0.1f);
}


TEST_CASE("manage directed list graph", "[list_graph][directed][manage]") {
  AdjacencyListGraph<unsigned long, Directedness::DIRECTED> g;
  g.add_vertex(10);
  g.add_edge(0, 1);
  g.add_edge(1, 1);
  g.add_edge(1, 2);

  SECTION("check source and target getters") {
    auto edge01 = *(g[0].begin());
    REQUIRE(g.get_source(edge01) == 0);
    REQUIRE(g.get_target(edge01) == 1);
  }

  SECTION("the out edges are computed correctly") {
    REQUIRE(g[0].size() == 1);
    auto edge01 = *(g[0].begin());
    REQUIRE(g.get_target(edge01) == 1);

    REQUIRE(g[1].size() == 2);
    auto edge11 = *(++g[1].begin());
    auto edge12 = *(++g[1].begin());
    REQUIRE(g.get_target(edge11) == 1);
    REQUIRE(g.get_target(edge12) == 2);
  }
}

} // namespace list_graph_test
