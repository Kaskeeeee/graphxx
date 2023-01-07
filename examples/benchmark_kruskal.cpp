#include "base.hpp"
#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "kruskal.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include <iostream> // for std::cout
#include <nanobench.h>

using namespace graph;
using namespace graph::algorithms;

int main() {
  // Graph++
  AdjacencyListGraph<Directedness::DIRECTED> g{};
  std::unordered_map<int, double> weights;

  std::fstream input_file("../examples/cage4.mtx");
  io::matrix_market::deserialize(input_file, g, weights);

  ankerl::nanobench::Bench().run("kruskal graph++",
                                 [&]() { kruskal::visit(g, weights); });

  // Boost

  typedef boost::adjacency_list<
      boost::vecS, boost::vecS, boost::directedS,
      boost::property<boost::vertex_distance_t, double>,
      boost::property<boost::edge_weight_t, double>>
      graph_t;
  typedef boost::graph_traits<graph_t>::edge_descriptor BoostEdge;

  graph_t boost_graph;
  std::vector<BoostEdge> spanning_tree;

  for (auto e : g.edges()) {
    boost::add_edge(e.u, e.v, weights[e], boost_graph);
  }

  ankerl::nanobench::Bench().run("kruskal boost", [&]() {
    boost::kruskal_minimum_spanning_tree(boost_graph,
                                         std::back_inserter(spanning_tree));
  });
}