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

#include <algorithm>
#include <vector>

namespace digraph_test {

using namespace graphxx;
using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;

TEST_CASE("digraph is built correctly", "[digraph]") {
  Graph g;

  SECTION("empty graph has no vertices or edges") {
    REQUIRE(g.num_vertices() == 0);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("vertices can be inserted and removed") {
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();
    REQUIRE(g.num_vertices() == 3);

    /* g.remove_vertex(0);
    REQUIRE_FALSE(g.vertices().empty());
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == z.id));

    g.remove_vertex(u);
    g.remove_vertex(z);
    REQUIRE(g.vertices().empty()); */
  }

  SECTION("edges can be inserted and removed") {
    g.add_edge(0, 1);
    g.add_edge(1, 1);
    REQUIRE(g.num_edges() == 2);

    g.remove_edge(0, 1);
    g.remove_edge(1, 1);
    REQUIRE(g.num_edges() == 0);
  }
}

TEST_CASE("digraph manages vertices and edges correctly", "[digraph]") {
  Graph g;

  SECTION("removing a vertex will remove also the connected edges") {
    g.add_vertex();
    g.add_vertex();
    g.add_edge(0, 1);

    g.remove_vertex(1);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("the outgoing edges are returned correctly") {
    g.add_vertex();
    g.add_vertex();
    g.add_vertex();
    g.add_edge(0, 1);
    g.add_edge(1, 1);
    g.add_edge(1, 2);

    REQUIRE(g[0].size() == 1);
    REQUIRE(std::get<1>(*g[0].begin()) == 1);

    REQUIRE(g[1].size() == 2);
    REQUIRE(std::get<1>(*(++g[0].begin())) == 1);
    REQUIRE(std::get<1>(*(++g[1].begin())) == 2);
  }
}

TEST_CASE("digraph helper methods", "[digraph]") {
  Graph g;

  SECTION("source and target getters") {
    g.add_edge(0, 1);
    REQUIRE(g.source(*g[0].begin()) == 0);
    REQUIRE(g.target(*g[0].begin()) == 1);
  }

  SECTION("num vertices and num edges") {
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

} // namespace digraph_test
