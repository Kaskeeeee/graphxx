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
#include "list_graph.hpp"
#include "tarjan.hpp"

namespace tarjan_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Tarjan for strongly connected components", "[tarjan]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  auto a = g.add_vertex();
  auto b = g.add_vertex();
  auto c = g.add_vertex();
  auto d = g.add_vertex();
  auto e = g.add_vertex();
  auto f = g.add_vertex();

  /*
      A       B       C       D       E       F
      |------->
              |------->
      <---------------|
              <-------<-------|------->
                              <-------|------->
  */

  auto a_to_b = g.add_edge(a, b);
  auto b_to_c = g.add_edge(b, c);
  auto c_to_a = g.add_edge(c, a);
  auto d_to_b = g.add_edge(d, b);
  auto d_to_c = g.add_edge(d, c);
  auto d_to_e = g.add_edge(d, e);
  auto e_to_d = g.add_edge(e, d);
  auto e_to_f = g.add_edge(e, f);

  SECTION("detects strongly connected components") {
    tarjan::SCCVector scc = tarjan::visit(g);

    REQUIRE(scc.size() == 3);
  }
}
} // namespace tarjan_test