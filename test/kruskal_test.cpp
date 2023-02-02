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

#include <map>
#include <tuple>

namespace kruskal_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Kruskal minimum spanning tree", "[kruskal]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;

  g.add_edge(a, b);
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(b, a);
  g.add_edge(c, a);
  g.add_edge(d, a);

  g.add_edge(b, c);

  g.add_edge(c, d);

  /*
      A       B       C       D
      |-------|
              |-------|
      |---------------|-------|
      |-----------------------|
  */

  weight[{a, b}] = 2;
  weight[{a, c}] = 1;
  weight[{a, d}] = 4;
  weight[{b, a}] = 2;
  weight[{d, a}] = 1;
  weight[{d, a}] = 4;
  weight[{b, c}] = 3;
  weight[{c, d}] = 9;

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.get_source(e), g.get_target(e)}];
  };

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal::visit(g, get_weight);

    REQUIRE(vector.size() == 3);
  }
}
} // namespace kruskal_test