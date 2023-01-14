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
#include "kruskal.hpp"
#include "list_graph.hpp"

namespace kruskal_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Kruskal minimum spanning tree", "[kruskal]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<DefaultIdType, int> weights;

  auto a = g.add_vertex();
  auto b = g.add_vertex();
  auto c = g.add_vertex();
  auto d = g.add_vertex();

  /*
      A       B       C       D
      |-------|
              |-------|
      |---------------|-------|
      |-----------------------|
  */

  auto a_to_b = g.add_edge(a, b);
  auto a_to_c = g.add_edge(a, c);
  auto a_to_d = g.add_edge(a, d);
  auto b_to_a = g.add_edge(b, a);
  auto c_to_a = g.add_edge(c, a);
  auto d_to_a = g.add_edge(d, a);

  auto b_to_c = g.add_edge(b, c);

  auto c_to_d = g.add_edge(c, d);

  weights[a_to_b] = 2;
  weights[a_to_c] = 1;
  weights[a_to_d] = 4;
  weights[b_to_a] = 2;
  weights[d_to_a] = 1;
  weights[d_to_a] = 4;
  weights[b_to_c] = 3;
  weights[c_to_d] = 9;

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal::visit(g, weights);

    REQUIRE(vector.size() == 3);
  }
}
} // namespace kruskal_test
