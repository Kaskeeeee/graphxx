#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"

#include <bits/stdc++.h>

using namespace graph;
using namespace std;

int main() {
  AdjacencyListGraph<Directedness::DIRECTED> g{};
  unordered_map<int, double> weights;
  fstream input_file("../data/mm.txt");
  io::matrix_market::deserialize(input_file, g, weights);
  io::matrix_market::serialize(cout, g, weights);
  return 0;
}