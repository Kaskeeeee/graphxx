/**
 * @file
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "algorithms/bellman_ford.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <filesystem>
#include <iostream>
#include <nanobench.h>

using BoostGraph =
    boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                          boost::property<boost::vertex_distance_t, double>,
                          boost::property<boost::edge_weight_t, double>>;
using BoostVertex = boost::graph_traits<BoostGraph>::vertex_descriptor;

using ListGraph =
    graphxx::AdjacencyListGraph<unsigned long, graphxx::Directedness::DIRECTED,
                                double>;
using MatrixGraph =
    graphxx::AdjacencyMatrixGraph<unsigned long,
                                  graphxx::Directedness::DIRECTED, double>;

int main(int argc, char **argv) {
  // Graphxx
  ListGraph list_graph{};
  MatrixGraph matrix_graph{};

  // Boost
  BoostGraph boost_graph;

  ankerl::nanobench::Bench bench{};

  if (argc <= 1) {
    // default file, if not specified
    std::fstream input_file("../data/cage4.mtx");
    graphxx::io::mm_deserialize<double>(
        input_file, list_graph);
  } else if (argc >= 2) {
    // Check if the file is a regular file and is not empty
    if (std::filesystem::is_regular_file(argv[1])) {
      if (!std::filesystem::is_empty(argv[1])) {
        std::fstream input_file(argv[1]);
        graphxx::io::mm_deserialize<double>(
            input_file, list_graph);
      } else {
        std::cout << "An empty file was passed as input" << '\n';
        exit(1);
      }
    } else {
      std::cout << "File '" << argv[1] << "' does not exists" << '\n';
      exit(1);
    }
  }

  for (auto v : list_graph) {
    for (auto e : v) {
      boost::add_edge(list_graph.get_source(e), list_graph.get_target(e),
                      std::get<2>(e), boost_graph);
      matrix_graph.add_edge(list_graph.get_source(e), list_graph.get_target(e),
                            {std::get<2>(e)});
    }
  }

  bench
      .run("bellman_ford graphxx",
           [&]() { graphxx::algorithms::bellman_ford(list_graph, 0); })
      .run("bellman_ford matrix graphxx",
           [&]() { graphxx::algorithms::bellman_ford(matrix_graph, 0); })
      .run("bellman_ford boost", [&]() {
        boost::bellman_ford_shortest_paths(
            boost_graph, boost::root_vertex(boost::vertex(0, boost_graph)));
      });
}