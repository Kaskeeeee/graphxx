#pragma once

#include "graph_concepts.hpp"
#include <limits>

namespace graph {

template <concepts::Numeric T> bool sum_will_overflow(T lhs, T rhs) {
  return lhs > 0 ? (std::numeric_limits<T>::max() - lhs) < rhs
                 : (std::numeric_limits<T>::min() - lhs) > rhs;
}

} // namespace graph