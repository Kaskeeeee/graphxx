#pragma once

#include "graph_concepts.hpp"

#include <limits>
#include <type_traits>

namespace graph {

/// @brief Checks if the sum of two numbers will overflow
/// @tparam T numeric type
/// @param lhs
/// @param rhs
/// @return true if the sum will overflow, false otherwise
template <concepts::Numeric T> bool sum_will_overflow(T lhs, T rhs) {
  return lhs > 0 ? (std::numeric_limits<T>::max() - lhs) < rhs
                 : (std::numeric_limits<T>::min() - lhs) > rhs;
}

/// @brief type that represents the decayed version of the type returned by
/// accessing an object through operator[]
/// @tparam Key
/// @tparam C
template <typename Key, concepts::Subscriptable<Key> C>
using DecaySubscriptValue = std::decay_t<decltype(std::declval<C>()[Key()])>;

} // namespace graph