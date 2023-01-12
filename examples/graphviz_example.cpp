/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
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