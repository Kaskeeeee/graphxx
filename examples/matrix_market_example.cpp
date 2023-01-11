#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"

#include <bits/stdc++.h>
#include <unordered_map>

int main() {
  graph::AdjacencyListGraph<graph::Directedness::DIRECTED> g{};
  std::unordered_map<int, double> weights;
  std::fstream input_file("../data/mm.txt");
  graph::io::matrix_market::deserialize(input_file, g, weights);
  graph::io::matrix_market::serialize(std::cout, g, weights);
  return 0;
}