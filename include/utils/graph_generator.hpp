#pragma once

#include "base.hpp"
#include "chrono"
#include "graph_concepts.hpp"
#include "random"
#include "unordered_map"

namespace graph {

class GraphGenerator {
public:
  GraphGenerator();
  GraphGenerator(unsigned int seed);

  template <concepts::Graph G>
  void generate_random_graph(G &graph, int num_vertices, int num_edges,
                             int max_out_degree = -1, bool self_edges = true);

  template <concepts::Graph G, concepts::Numeric W>
  std::unordered_map<Id, W> generate_random_weights(const G &graph,
                                                    W min_weight, W max_weight);

private:
  unsigned int get_seed();

  unsigned int _seed;
  bool _fixed_seed;
};

} // namespace graph

#include "utils/graph_generator.i.hpp"