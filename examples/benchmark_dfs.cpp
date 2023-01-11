#include "algorithms/dfs.hpp"
#include "base.hpp"
#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"

#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <nanobench.h>
#include <unordered_map>

int main() {
  // Graphxx
  graph::AdjacencyListGraph<graph::Directedness::DIRECTED> g{};
  std::unordered_map<int, double> weights;

  std::fstream input_file("../data/cage4.mtx");
  graph::io::matrix_market::deserialize(input_file, g, weights);

  ankerl::nanobench::Bench().run("dfs graphxx", [&]() {
    graph::algorithms::dfs::visit(g, graph::Vertex{0});
  });

  // Boost
  using graph_t =
      boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                            boost::property<boost::vertex_distance_t, double>,
                            boost::no_property>;
  using BoostVertex = boost::graph_traits<graph_t>::vertex_descriptor;

  graph_t boost_graph;

  for (auto e : g.edges()) {
    boost::add_edge(e.u, e.v, boost_graph);
  }

  BoostVertex start = boost::vertex(0, boost_graph);

  // boost::write_graphviz(std::cout, boost_graph);

  ankerl::nanobench::Bench().run("dfs boost", [&]() {
    boost::depth_first_search(boost_graph, boost::root_vertex(start));
  });
}