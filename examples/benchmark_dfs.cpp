#define ANKERL_NANOBENCH_IMPLEMENT
#include "base.hpp"
#include "dfs.hpp"
#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

#include <iostream> // for std::cout
#include <nanobench.h>

using namespace graph;
using namespace graph::algorithms;

int main() {
  // Graphxx
  AdjacencyListGraph<Directedness::DIRECTED> g{};
  std::unordered_map<int, double> weights;

  std::fstream input_file("../data/cage4.mtx");
  io::matrix_market::deserialize(input_file, g, weights);

  ankerl::nanobench::Bench().run("dfs graphxx",
                                 [&]() { dfs::visit(g, Vertex{0}); });

  // Boost
  typedef boost::adjacency_list<
      boost::vecS, boost::vecS, boost::directedS,
      boost::property<boost::vertex_distance_t, double>, boost::no_property>
      graph_t;
  typedef boost::graph_traits<graph_t>::vertex_descriptor BoostVertex;

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