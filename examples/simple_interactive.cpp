#include "algorithms/bfs.hpp"
#include "list_graph.hpp"

#include <bits/stdc++.h>

void printGraph(
    graph::AdjacencyListGraph<graph::Directedness::DIRECTED> &graph) {
  for (auto vertex : graph.vertices()) {
    std::cout << "V[" << vertex.id << "]: ";
    for (auto edge : graph.out_edges(vertex)) {
      std::cout << " -> V[" << edge.target << "]";
    }
    std::cout << "\n";
  }
}

int main() {
  graph::AdjacencyListGraph<graph::Directedness::DIRECTED> a;

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

      a.add_edge(graph::Vertex{u}, graph::Vertex{v});
    } break;
    case 3: {
      int u;
      std::cout << "Remove vertex: ";
      std::cin >> u;
      a.remove_vertex(graph::Vertex{u});
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