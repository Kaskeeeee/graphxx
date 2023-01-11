#pragma once

#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graph::algorithms::ford_fulkerson {

struct BFSVertex {
  VertexStatus status;
  int parent = -1;
  int edge = -1;
  int residual_capacity = -1;
};

using BFSTree = std::unordered_map<Id, BFSVertex>;

/// @brief 
/// @tparam WeightType 
template <concepts::Numeric WeightType>
using FlowMap = std::unordered_map<Id, WeightType>;

/// @brief 
/// @tparam WeightType 
template <concepts::Numeric WeightType> struct FFpair {
  FlowMap<WeightType> flow;
  WeightType max_flow;
};

/// @brief Implementation of ford_fulkerson algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param sink sink vertex
/// @param edges_capacity edges capacity
/// @return maximum flow from source to sink in the given graph
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
FFpair<WeightType> visit(const G &graph, const Vertex &source,
                         const Vertex &sink, C &&edges_capacity);

} // namespace graph::algorithms::ford_fulkerson

#include "algorithms/ford_fulkerson.i.hpp"