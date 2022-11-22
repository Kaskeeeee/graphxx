#pragma once

#include "graph_concepts.hpp"
#include <limits>

namespace graph {

template <concepts::IsNumberLike T> bool sum_will_overflow(T a, T b) {
  return a > 0 ? (std::numeric_limits<T>::max() - a) > b
               : (std::numeric_limits<T>::min() - a) < b;
}

} // namespace graph