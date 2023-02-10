/**
 * @file
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

#if 0

#include "algorithms/bfs.hpp"
#include "list_graph.hpp"

#include <bits/stdc++.h>

void printGraph(
    graphxx::AdjacencyListGraph<graphxx::Directedness::DIRECTED> &graph) {
  for (auto vertex : graph.vertices()) {
    std::cout << "V[" << vertex.id << "]: ";
    for (auto edge : graph.out_edges(vertex)) {
      std::cout << " -> V[" << edge.target << "]";
    }
    std::cout << "\n";
  }
}

int main() {
  graphxx::AdjacencyListGraph<graphxx::Directedness::DIRECTED> a;

  while (true) {
    int opt = -1;
    std::cout << "Choose an option:\n";
    std::cout << "1. add vertex\n";
    std::cout << "2. add edge\n";
    std::cout << "3. remove vertex\n";
    std::cout << "4. remove edge\n";
    std::cout << "5. print bfs\n";
    std::cin >> opt;
    switch (opt) {
    case 1: {
      a.add_vertex();
    } break;
    case 2: {
      int u, v;
      std::cout << "Insert u: ";
      std::cin >> u;
      std::cout << "Insert v: ";
      std::cin >> v;

      a.add_edge(graphxx::Vertex{u}, graphxx::Vertex{v});
    } break;
    case 3: {
      int u;
      std::cout << "Remove vertex: ";
      std::cin >> u;
      a.remove_vertex(graphxx::Vertex{u});
    } break;
    case 4: {
      int u, v;
      std::cout << "Remove edge from u: ";
      std::cin >> u;
      std::cout << "To v: ";
      std::cin >> v;

      for (auto edge : a.edges()) {
        if (edge.source == u && edge.target == v) {
          a.remove_edge(edge);
        }
      }
    } break;
    case 5: {
      int u;
      std::cout << "Source vertex: ";
      std::cin >> u;

      std::cout << "BFS TREE:\n";
      std::cout << "+++++++++++++++++++++++++++++++++\n";
    }
    }
    std::cout << "\nGRAPH STRUCTURE:\n";
    printGraph(a);
    std::cout << "-----------------------------------\n";
  }

  return 0;
}

#endif