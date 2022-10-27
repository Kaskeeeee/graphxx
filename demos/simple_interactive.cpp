#include "g_bfs.hpp"
#include "g_list_digraph.hpp"
#include <bits/stdc++.h>

using namespace graph;
using namespace std;

void printGraph(AdjacencyListDigraph &graph) {
  for (auto vertex : graph.vertices()) {
    cout << "V[" << vertex.id << "]: ";
    for (auto edge : graph.out_edges(vertex)) {
      cout << " -> V[" << edge.target << "]";
    }
    cout << "\n";
  }
}

int main() {
  AdjacencyListDigraph a;

  while (true) {
    int opt = -1;
    cout << "Choose an option:\n";
    cout << "1. add vertex\n";
    cout << "2. add edge\n";
    cout << "3. remove vertex\n";
    cout << "4. remove edge\n";
    cout << "5. print bfs\n";
    cin >> opt;
    switch (opt) {
    case 1: {
      a.add_vertex();
    } break;
    case 2: {
      int u, v;
      cout << "Insert u: ";
      cin >> u;
      cout << "Insert v: ";
      cin >> v;

      a.add_edge({u}, {v});
    } break;
    case 3: {
      int u;
      cout << "Remove vertex: ";
      cin >> u;
      a.remove_vertex({u});
    } break;
    case 4: {
      int u, v;
      cout << "Remove edge from u: ";
      cin >> u;
      cout << "To v: ";
      cin >> v;

      for (auto edge : a.edges()) {
        if (edge.source == u && edge.target == v) {
          a.remove_edge(edge);
        }
      }
    } break;
    case 5: {
      int u;
      cout << "Source vertex: ";
      cin >> u;

      cout << "BFS TREE:\n";
      cout << "+++++++++++++++++++++++++++++++++\n";
    }
    }
    cout << "\nGRAPH STRUCTURE:\n";
    printGraph(a);
    cout << "-----------------------------------\n";
  }

  return 0;
}