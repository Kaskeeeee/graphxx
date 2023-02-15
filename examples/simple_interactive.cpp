/**
 * @file This file contains a simple and interactive terminal program
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

#include "algorithms/bfs.hpp"
#include "list_graph.hpp"

#include <cstdint>
#include <iostream>

using namespace graphxx;

void printGraph(
    AdjacencyListGraph<unsigned long, Directedness::DIRECTED> &graph) {
  for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
    std::cout << "V[" << vertex << "]: ";
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      std::cout << " -> V[" << graph.get_target(edge) << "]";
    }
    std::cout << "\n";
  }
}

int main() {
  AdjacencyListGraph<unsigned long, Directedness::DIRECTED> g;

  while (true) {
    int opt = -1;
    std::cout << "Choose an option:\n";
    std::cout << "1. add vertex\n";
    std::cout << "2. add edge\n";
    std::cout << "3. remove edge\n";
    std::cin >> opt;
    switch (opt) {
    case 1: {
      g.add_vertex();
    } break;
    case 2: {
      int u, v;
      std::cout << "Insert u: ";
      std::cin >> u;
      std::cout << "Insert v: ";
      std::cin >> v;

      g.add_edge(u, v);
    } break;
    case 3: {
      int u, v;
      std::cout << "Remove edge from u: ";
      std::cin >> u;
      std::cout << "To v: ";
      std::cin >> v;

      g.remove_edge(u, v);

    } break;
    }
    std::cout << "\nGRAPH STRUCTURE:\n";
    printGraph(g);
    std::cout << "-----------------------------------\n";
  }

  return 0;
}
