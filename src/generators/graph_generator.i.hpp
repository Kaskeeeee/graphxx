#include "generators/graph_generator.hpp"

#if 0

#include <chrono>
#include <random>
#include <unordered_map>

namespace graphxx {
GraphGenerator::GraphGenerator() : _seed{0}, _fixed_seed{false} {};
GraphGenerator::GraphGenerator(unsigned int seed)
    : _seed{seed}, _fixed_seed{true} {};

template <concepts::Graph G>
void GraphGenerator::generate_random_graph(G &graph, int num_vertices,
                                           int num_edges, int max_out_degree,
                                           bool self_edges) {
  for (int i = 0; i < num_vertices; i++) {
    graph.add_vertex();
  }

  if (max_out_degree < 0) {
    max_out_degree = num_vertices;
  }

  if (!_fixed_seed) {
    _seed = get_seed();
  }

  std::default_random_engine generator(_seed);
  std::uniform_int_distribution<Id> distribution(0, num_vertices - 1);

  std::unordered_map<Id, int> out_degree;

  for (int i = 0; i < num_edges; i++) {
    auto source_id = distribution(generator);
    auto target_id = distribution(generator);

    if (out_degree[source_id] == max_out_degree) {
      bool found = false;

      for (int i = 0; i < num_vertices; i++) {
        source_id = (source_id + 1) % num_vertices;
        if (out_degree[source_id] < max_out_degree) {
          found = true;
          break;
        }
      }

      if (!found) {
        return;
      }
    }

    if (!self_edges) {
      while (source_id == target_id) {
        if (num_vertices <= 1) {
          return;
        }

        target_id = distribution(generator);
      }
    }

    graph.add_edge(Vertex{source_id}, Vertex{target_id});
    out_degree[source_id]++;
  }
}

template <concepts::Graph G, concepts::Numeric W>
std::unordered_map<Id, W>
GraphGenerator::generate_random_weights(const G &graph, W min_weight,
                                        W max_weight) {
  std::unordered_map<Id, W> weights;

  if (min_weight > max_weight) {
    W tmp = min_weight;
    min_weight = max_weight;
    max_weight = tmp;
  }

  if (!_fixed_seed) {
    _seed = get_seed();
  }

  std::default_random_engine generator(_seed);
  std::uniform_int_distribution<W> distribution(min_weight, max_weight);

  for (auto edge : graph.edges()) {
    weights[edge.id] = distribution(generator);
  }

  return weights;
}

unsigned int GraphGenerator::get_seed() {
  return std::chrono::system_clock::now().time_since_epoch().count();
}

} // namespace graph

#endif