#include "matrix_graph.hpp"
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

int main() {
  using namespace graph;

  AdjacencyMatrixGraph<GraphType::Directed> graph_;

  for (auto i : std::views::iota(0, 10)) {
    graph_.add_edge(Vertex{i}, Vertex{i + 1});
  }

  std::cout << "Vertices: ";
  for (auto v : graph_.vertices()) {
    std::cout << v.id << ", ";
  }

  std::cout << "\nEdges: ";
  for (auto e : graph_.edges()) {
    std::cout << e.id << ", ";
  }

  std::cout << std::endl;
}