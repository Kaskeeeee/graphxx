#include "graph.hpp"
#include <bits/stdc++.h>

using namespace graph;
using namespace std;

int main() {
  std::cout << "Hello World!" << std::endl;

  AdjacencyList<int> a = AdjacencyList<int>();

  while (true) {
    int opt = -1;
    cout << "Choose an option:\n";
    cout << "1. add vertex\n";
    cout << "2. add edge\n";
    cout << "3. remove vertex\n";
    cout << "4. remove edge\n";
    cin >> opt;
    switch (opt) {
    case 1: {
      int vertex_id = 0;
      cout << "Insert vertex data: ";
      cin >> vertex_id;
      a.add_vertex(vertex_id);
    } break;
    case 2: {
      int u, v;
      cout << "Insert u: ";
      cin >> u;
      cout << "Insert v: ";
      cin >> v;

      a.add_edge(a.get_vertex_by_id(u), a.get_vertex_by_id(v));
    } break;
    case 3: {
      int u;
      cout << "Remove vertex: ";
      cin >> u;
      a.remove_vertex(a.get_vertex_by_id(u));
    } break;
    case 4: {
      int u, v;
      cout << "Remove edge from u: ", cin >> u;
      cout << "To v: ", cin >> v;
    }
    }
    cout << "\n";

    a.print();
  }

  return 0;
}