#include "graph.hpp"
#include <iostream>

using namespace graph;
using namespace std;

int main() {
  std::cout << "Hello World!" << std::endl;

  AdjacencyList<int, int> a = AdjacencyList<int, int>();

  while (true) {
    int opt = -1;
    cout << "Choose an option:\n";
    cout << "1. add vertex\n";
    cout << "2. add edge\n";
    cin >> opt;
    switch (opt) {
    case 1: {
      int vertex_id = 0;
      cout << "Insert vertex id: ";
      cin >> vertex_id;
      a.add_vertex(vertex_id);
    } break;
    case 2: {
      int u, v;
      cout << "Insert u: ";
      cin >> u;
      cout << "Insert v: ";
      cin >> v;
      a.add_edge(u, v, 0);
    } break;
    default:
      cout << "Terminating...\n";
      return 0;
    }
    cout << "\n";
    a.print();
  }

  return 0;
}