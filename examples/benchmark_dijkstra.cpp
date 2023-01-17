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

#include "algorithms/dijkstra.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"

#include <bits/stdc++.h> //DA TENERE?

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <filesystem>
#include <nanobench.h>
// #include <boost/graph/graph_traits.hpp>
// #include <boost/graph/graphviz.hpp>

int main(int argc, char **argv) {
  // Graphxx
  graphxx::AdjacencyListGraph<unsigned long, graphxx::Directedness::DIRECTED,
                              double>
      g{};

  // auto s = g.add_vertex(); // 0
  // auto a = g.add_vertex(); // 1
  // auto b = g.add_vertex(); // 2
  // auto c = g.add_vertex(); // 3
  // auto d = g.add_vertex(); // 4
  // auto t = g.add_vertex(); // 5
  //
  // auto s_to_a = g.add_edge(s, a); // 0->1
  // auto s_to_d = g.add_edge(s, d); // 0->4
  // auto a_to_b = g.add_edge(a, b); // 1->2
  // auto b_to_t = g.add_edge(b, t); // 2->5
  // auto c_to_t = g.add_edge(c, t); // 3->5
  // auto d_to_b = g.add_edge(d, b); // 4->2
  // auto d_to_c = g.add_edge(d, c); // 4->3
  //
  // weights[s_to_a] = 8;
  // weights[s_to_d] = 3;
  // weights[a_to_b] = 9;
  // weights[b_to_t] = 2;
  // weights[c_to_t] = 5;
  // weights[d_to_b] = 7;
  // weights[d_to_c] = 4;

  if (argc <= 1) {
    // default file, if not specified
    std::fstream input_file("../data/cage4.mtx");
    graphxx::io::matrix_market::deserialize<decltype(g), double>(input_file, g);
  } else if (argc == 2) {
    // Check if the file is a regular file and is not empty
    if (std::filesystem::is_regular_file(argv[1])) {
      if (!std::filesystem::is_empty(argv[1])) {
        std::fstream input_file(argv[1]);
        graphxx::io::matrix_market::deserialize<decltype(g), double>(input_file,
                                                                     g);
      } else {
        // Throw exception file empty
        throw graphxx::exceptions::EmptyFileException();
      }
    } else {
      // Throw exception file not exists
      throw graphxx::exceptions::NotFileException();
    }
  } else {
    // Throw exception too many args
    throw graphxx::exceptions::TooManyArgumentsException();
  }

  ankerl::nanobench::Bench().run("dijkstra graphxx", [&]() {
    graphxx::algorithms::dijkstra::visit(g, 0);
  });

  // Boost
  using graph_t =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                            boost::property<boost::vertex_distance_t, double>,
                            boost::property<boost::edge_weight_t, double>>;
  using BoostVertex = boost::graph_traits<graph_t>::vertex_descriptor;

  graph_t boost_graph;

  for (auto v : g) {
    for (auto e : v) {
      boost::add_edge(g.source(e), g.target(e), std::get<2>(e), boost_graph);
    }
  }

  BoostVertex start = boost::vertex(0, boost_graph);

  // boost::write_graphviz(std::cout, boost_graph);

  ankerl::nanobench::Bench().run("dijkstra boost", [&]() {
    boost::dijkstra_shortest_paths(
        boost_graph, start,
        boost::distance_map(get(boost::vertex_distance, boost_graph)));
  });

  return 0;
}