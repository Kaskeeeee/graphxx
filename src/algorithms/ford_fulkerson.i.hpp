/**
 * @file This file is the header implementation of Ford Fulkerson algorithm
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "algorithms/ford_fulkerson.hpp" // ford_fulkerson
#include "base.hpp"                      // Vertex
#include "graph_concepts.hpp"            // Graph

#include <limits> // std::numeric_limits
#include <vector> // std::vector

namespace graphxx::algorithms {

namespace detail::ford_fulkerson {
template <concepts::Graph G, concepts::Numeric Flow>
Flow dfs(const G &graph, Vertex<G> u, Vertex<G> sink,
         std::vector<bool> &visited, Flow flow,
         std::vector<std::vector<Flow>> &capacity) {
  if (u == sink) {
    return flow;
  }
  visited[u] = true;
  for (Vertex<G> v = 0; v < graph.num_vertices(); ++v) {
    if (!visited[v] && capacity[u][v] > 0) {
      Flow new_flow = dfs(graph, v, sink, visited,
                          std::min(flow, capacity[u][v]), capacity);
      if (new_flow > 0) {
        capacity[u][v] -= new_flow;
        capacity[v][u] += new_flow;
        return new_flow;
      }
    }
  }
  return 0;
}
} // namespace detail::ford_fulkerson

template <concepts::Graph G, concepts::Numeric Flow>
Flow ford_fulkerson(const G &graph, Vertex<G> source, Vertex<G> sink,
                    std::vector<std::vector<Flow>> &capacity) {

  Flow max_flow = 0;
  std::vector<bool> visited(graph.num_vertices());

  while (true) {
    std::fill(visited.begin(), visited.end(), false);
    Flow flow =
        detail::ford_fulkerson::dfs(graph, source, sink, visited,
                                    std::numeric_limits<Flow>::max(), capacity);
    if (flow == 0) {
      break;
    }
    max_flow += flow;
  }

  return max_flow;
}

} // namespace graphxx::algorithms