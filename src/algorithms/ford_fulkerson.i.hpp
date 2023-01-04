#include "algorithms/algorithms_base.hpp"
#include "algorithms/ford_fulkerson.hpp"
#include "exceptions.hpp"
#include "list_graph.hpp"
#include <cassert>
#include <limits>
#include <queue>

namespace graph::algorithms::ford_fulkerson {

template <concepts::Graph G, concepts::Graph DG, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
BFSTree bfs(const G &graph, const DG &digraph, const Vertex &source,
            C &&edge_capacity, const FlowMap<WeightType> &edges_flow) {
  BFSTree tree;
  for (auto vertex : graph.vertices()) {
    tree[vertex] = BFSVertex{VertexStatus::READY};
  }

  tree[source].status = VertexStatus::WAITING;
  tree[source].parent = -1;

  std::queue<Id> queue;
  queue.push(source);

  while (!queue.empty()) {
    Vertex vertex_id{queue.front()};
    queue.pop();

    for (Edge out_edge : graph.out_edges(vertex_id)) {
      if (edge_capacity[out_edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      Id adjacent = out_edge.v;

      WeightType cf;
      if (digraph.get_edge(out_edge) != INVALID_EDGE) {
        cf = edge_capacity[out_edge] - edges_flow.at(out_edge);
      } else {
        cf = edge_capacity[out_edge] + edges_flow.at(out_edge);
      }

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

  WeightType max_flow = 0;

  auto tree = bfs(g, graph, source, edges_capacity, flow);

  // Relax edges |nodes| - 1 times
  while (tree[sink].status == VertexStatus::PROCESSED) {
    WeightType path_flow = std::numeric_limits<WeightType>::max();
    for (int v = sink; v != source; v = tree[v].parent) {
      WeightType cf;
      if (graph.get_edge(tree[v].edge) != INVALID_EDGE) {
        cf = edges_capacity[tree[v].edge] - flow.at(tree[v].edge);
      } else {
        cf = edges_capacity[tree[v].edge] + flow.at(tree[v].edge);
      }
      path_flow = std::min(path_flow, cf);
    }

    for (int v = sink; v != source; v = tree[v].parent) {
      if (graph.get_edge(tree[v].edge) != INVALID_EDGE) {
        flow[tree[v].edge] += path_flow;
      } else {
        flow[tree[v].edge] -= path_flow;
      }

      // if (flow[tree[v].edge] < 0) {
      //   flow[tree[v].edge] = 0
      // }
    }

    // Adding the path flows
    max_flow += path_flow;

    tree = bfs(g, graph, source, edges_capacity, flow);
  }

  return max_flow;
}

} // namespace graph::algorithms::ford_fulkerson