#define ANKERL_NANOBENCH_IMPLEMENT
#include "base.hpp"
#include "bfs.hpp"
#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

// #include <lemon/list_graph.h>

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

  ankerl::nanobench::Bench().run("bfs graph++",
                                 [&]() { bfs::visit(g, Vertex{0}); });

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

  ankerl::nanobench::Bench().run("bfs boost", [&]() {
    boost::breadth_first_search(
        boost_graph, start,
        boost::distance_map(get(boost::vertex_distance, boost_graph)));
  });

  // Lemon
  // ListDigraph lemon_graph;
  // ListDigraph::NodeMap<double> dist(lemon_graph);
  //
  // for (auto e : g.edges()) {
  //  ListDigraph::addArc(e.u, e.v);
  //}
  //
  // ankerl::nanobench::Bench().run("bfs lemon", [&]() {
  //  ListDigraph::bfs(lemon_graph).distMap(dist).run(s);
  //});
}