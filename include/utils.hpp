#pragma once

#include "graph_concepts.hpp"
#include <limits>
#include <type_traits>

namespace graph {

template <concepts::Numeric T> bool sum_will_overflow(T lhs, T rhs) {
  return lhs > 0 ? (std::numeric_limits<T>::max() - lhs) < rhs
                 : (std::numeric_limits<T>::min() - lhs) > rhs;
}

template <typename Key, concepts::Subscriptable<Key> C>
using DecaySubscriptValue = std::decay_t<decltype(std::declval<C>()[Key()])>;

} // namespace graph