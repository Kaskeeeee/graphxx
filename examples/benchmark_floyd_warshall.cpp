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

#include "algorithms/floyd_warshall.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

#include <bits/stdc++.h> //DA TENERE?

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <filesystem>
#include <iostream>
#include <nanobench.h>

using graph_t =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                          boost::property<boost::vertex_distance_t, double>,
                          boost::property<boost::edge_weight_t, double>>;
typedef boost::property_map<graph_t, boost::edge_weight_t>::type WeightMap;

// Declare a matrix type and its corresponding property map that
// will contain the distances between each pair of vertices.
typedef boost::exterior_vertex_property<graph_t, double> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

int main(int argc, char **argv) {
  // Graphxx
  graphxx::AdjacencyListGraph<unsigned long, graphxx::Directedness::DIRECTED,
                              double>
      list_graph{};

  graphxx::AdjacencyMatrixGraph<unsigned long, graphxx::Directedness::DIRECTED,
                                double>
      matrix_graph{};

  if (argc <= 1) {
    // default file, if not specified
    std::fstream input_file("../data/cage4.mtx");
    graphxx::io::matrix_market::deserialize<decltype(list_graph), double>(
        input_file, list_graph);
  } else if (argc >= 2) {
    // Check if the file is a regular file and is not empty
    if (std::filesystem::is_regular_file(argv[1])) {
      if (!std::filesystem::is_empty(argv[1])) {
        std::fstream input_file(argv[1]);
        graphxx::io::matrix_market::deserialize<decltype(list_graph), double>(
            input_file, list_graph);
      } else {
        std::cout << "An empty file was passed as input" << std::endl;
        exit(1);
      }
    } else {
      std::cout << "File '" << argv[1] << "' does not exists" << std::endl;
      exit(1);
    }
  }

  ankerl::nanobench::Bench().run("floyd warshall graphxx", [&]() {
    graphxx::algorithms::floyd_warshall::visit(list_graph);
  });

  // Boost

  using BoostVertex = boost::graph_traits<graph_t>::vertex_descriptor;

  graph_t boost_graph;

  for (auto v : list_graph) {
    for (auto e : v) {
      boost::add_edge(list_graph.get_source(e), list_graph.get_target(e),
                      std::get<2>(e), boost_graph);
      matrix_graph.add_edge(list_graph.get_source(e), list_graph.get_target(e),
                            {std::get<2>(e)});
    }
  }

  ankerl::nanobench::Bench().run("floyd warshall matrix graphxx", [&]() {
    graphxx::algorithms::floyd_warshall::visit(matrix_graph);
  });

  WeightMap weight_pmap = boost::get(boost::edge_weight, boost_graph);
  DistanceMatrix distances(num_vertices(boost_graph));
  DistanceMatrixMap dm(distances, boost_graph);

  // boost::write_graphviz(std::cout, boost_graph);

  ankerl::nanobench::Bench().run("floyd warshall boost", [&]() {
    boost::floyd_warshall_all_pairs_shortest_paths(
        boost_graph, dm, boost::weight_map(weight_pmap));
  });

  return 0;
}