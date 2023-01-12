#include "io/graphviz.hpp"
#include "io/graphml.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"

#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace graph;

int main() {

  graph::AdjacencyListGraph<graph::Directedness::DIRECTED> g{};

  std::unordered_map<int, std::string> vertex_map;

  Vertex v1 = g.add_vertex(); //A
  vertex_map.insert({v1, "A"});

  Vertex v2 = g.add_vertex();
  vertex_map.insert({v2, "B"});

  Vertex v3 = g.add_vertex();
  vertex_map.insert({v3, "C"});

  Vertex v4 = g.add_vertex();
  vertex_map.insert({v4, "D"});

  Vertex v5 = g.add_vertex();
  vertex_map.insert({v5, "E"});

  g.add_edge(v1, v2);
  g.add_edge(v2, v2);
  g.add_edge(v1, v3);
  g.add_edge(v3, v4);
  g.add_edge(v5, v3);
  g.add_edge(v4, v5);
  g.add_edge(v5, v4);
  g.add_edge(v4, v2);
  g.add_edge(v4, v4);

//   GraphGenerator gen;

//   gen.generate_random_graph(g, 1000, 3000);

//   std::unordered_map<graph::Id, graph::io::graphviz::GraphvizProperties>
//       vertex_properties;
//   std::unordered_map<graph::Id, graph::io::graphviz::GraphvizProperties>
//       edge_properties;

//   std::fstream input_file("../data/test.txt");
//   graph::io::graphml::deserialize(input_file, g, vertex_properties,
//                                    edge_properties);

  graph::io::graphml::serialize(std::cout, g);

  return 0;
}