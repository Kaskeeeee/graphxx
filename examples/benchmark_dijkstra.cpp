#define ANKERL_NANOBENCH_IMPLEMENT
#include "base.hpp"
#include "dijkstra.hpp"
#include "io/graphml.hpp"
#include "io/graphviz.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "utils/graph_generator.hpp"
#include <bits/stdc++.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// #include <boost/graph/graph_traits.hpp>
// #include <boost/graph/graphviz.hpp>

// #include <lemon/list_graph.h>

#include <iostream> // for std::cout
#include <nanobench.h>

using namespace graph;
using namespace graph::algorithms;

int main() {
  // Graph++
  AdjacencyListGraph<Directedness::DIRECTED> g{};
  std::unordered_map<int, double> weights;

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

  std::fstream input_file("../examples/cage4.mtx");
  io::matrix_market::deserialize(input_file, g, weights);

  ankerl::nanobench::Bench().run(
      "dijkstra graph++", [&]() { dijkstra::visit(g, Vertex{0}, weights); });

  // Boost
  typedef boost::adjacency_list<
      boost::vecS, boost::vecS, boost::directedS,
      boost::property<boost::vertex_distance_t, double>,
      boost::property<boost::edge_weight_t, double>>
      graph_t;
  typedef boost::graph_traits<graph_t>::vertex_descriptor BoostVertex;

  graph_t boost_graph;

  for (auto e : g.edges()) {
    boost::add_edge(e.u, e.v, weights[e], boost_graph);
  }

  BoostVertex start = boost::vertex(0, boost_graph);

  // boost::write_graphviz(std::cout, boost_graph);

  ankerl::nanobench::Bench().run("dijkstra boost", [&]() {
    boost::dijkstra_shortest_paths(
        boost_graph, start,
        boost::distance_map(get(boost::vertex_distance, boost_graph)));
  });

  // Lemon
  // ListDigraph lemon_graph;
  // ListDigraph::ArcMap<double> length(lemon_graph);
  // ListDigraph::NodeMap<double> dist(lemon_graph);
  //
  // for (auto e : g.edges()) {
  //  ListDigraph::Arc new_arc = ListDigraph::addArc(e.u, e.v);
  //  map[new_arc] = weights[e];
  //}
  //
  // ankerl::nanobench::Bench().run("dijkstra lemon", [&]() {
  //  ListDigraph::dijkstra(lemon_graph, length).distMap(dist).run(s);
  //});

  return 0;
}