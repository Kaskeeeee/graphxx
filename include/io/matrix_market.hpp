#pragma once

#include "base.hpp"
#include "utils.hpp"
#include "graph_concepts.hpp"

namespace graph::io::matrix_market {

template <concepts::Graph G, concepts::Subscriptable<Id> C, concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
void serialize(std::ostream &out, const G &graph, C &weights);

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

template <concepts::Graph G, concepts::Subscriptable<Id> C, concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
void deserialize(std::istream &in, G &graph, C &weights);

} // namespace graph::io::matrix_market

#include "io/matrix_market.i.hpp"