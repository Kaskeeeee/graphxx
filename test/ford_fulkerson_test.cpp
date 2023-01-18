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

#include <map>
#include <tuple>

namespace ford_fulkerson_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Ford Fulkerson maximum flow", "[ford_fulkerson]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { s, a, b, c, d, t };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;

  g.add_edge(s, a); // 0->1
  g.add_edge(s, d); // 0->4
  g.add_edge(a, b); // 1->2
  g.add_edge(b, t); // 2->5
  g.add_edge(c, t); // 3->5
  g.add_edge(d, b); // 4->2
  g.add_edge(d, c); // 4->3

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
  */

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.source(e), g.target(e)}];
  };

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, b}] = -1;

    REQUIRE_THROWS(ford_fulkerson::visit(g, s, t, get_weight));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto result = ford_fulkerson::visit(g, s, t, get_weight);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    weight[{s, a}] = 8;
    weight[{s, d}] = 3;
    weight[{a, b}] = 9;
    weight[{b, t}] = 2;
    weight[{c, t}] = 5;
    weight[{d, b}] = 7;
    weight[{d, c}] = 4;

    auto result = ford_fulkerson::visit(g, s, t, get_weight);

    REQUIRE(result.max_flow == 6);
  }
}

} // namespace ford_fulkerson_test

#endif