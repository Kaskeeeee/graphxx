#include "algorithms/ford_fulkerson.hpp"
#include "exceptions.hpp"
#include "list_graph.hpp"
#include <cassert>
#include <limits>

namespace graph::algorithms::ford_fulkerson {

struct BFSVertex {
  VertexStatus status;
  int parent = -1;
  int edge = -1;
};

using BFSTree = std::unordered_map<Id, BFSVertex>;

template <concepts::Numeric WeightType>
using FlowMap = std::unordered_map<Id, WeightType>;

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
BFSTree bfs(const G &graph, const Vertex source, const C &&edge_capacity,
            const C &&edges_flow) {
  BFSTree tree;
  for (auto vertex : graph.vertices()) {
    tree[vertex] = BFSVertex{VertexStatus::READY};
  }

  tree[source].status = VertexStatus::WAITING;
  tree[source].parent = -1;

  std::queue<Id> queue;
  queue.push(source);

  while (!queue.empty()) {
    Id vertex_id = queue.front();
    queue.pop();

    for (Edge out_edge : graph.out_edges(Vertex{vertex_id})) {
      Id adjacent = out_edge.v;

      auto cf = edge_capacity[out_edge] - edges_flow[out_edge];

      if (tree[adjacent].status == VertexStatus::READY && cf > 0) {
        tree[adjacent].status = VertexStatus::WAITING;
        tree[adjacent].parent = vertex_id;
        tree[adjacent].edge = out_edge;
        queue.push(adjacent);
      }
    }

    tree[vertex_id].status = VertexStatus::PROCESSED;
  }

  return tree;
}

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
WeightType visit(const G &graph, const Vertex &source, const Vertex &sink,
                       C &&edges_capacity) {

  FlowMap<WeightType> flow;

  AdjacencyListGraph<Directedness::UNDIRECTED> g{graph};

  for (auto edge : g.edges()) {
    flow[edge] = 0;
  }

  Vertex s = source;
  Vertex t = sink;
  Vertex u;

  WeightType max_flow = 0;

  auto tree = bfs(g, s, t, edges_capacity, flow);

  // Relax edges |nodes| - 1 times
  while (tree[t].status == VertexStatus::PROCESSED) {
    WeightType path_flow = std::numeric_limits<WeightType>::max();
    for (Vertex v = t; v != s; v = tree[v].parent) {
      u = tree[v].parent;
      auto cf = edges_capacity[tree[v].edge] - flow[tree[v].edge];
      path_flow = min(path_flow, cf);
    }

    for (Vertex v = t; v != s; v = tree[v].parent) {
      u = tree[v].parent;
      flow[tree[v].edge] += path_flow;
    }

    // Adding the path flows
    max_flow += path_flow;

    tree = bfs(g, s, t, edges_capacity);
  }

  return tree;
}

} // namespace graph::algorithms::ford_fulkerson