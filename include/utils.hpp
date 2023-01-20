/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
 *
 * @license{<blockquote>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </blockquote>}
 *
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

#pragma once

#include "graph_concepts.hpp"

#include <limits>
#include <type_traits>
#include <vector>

namespace graphxx {

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

template <typename T> void fill_vector(std::vector<T> &v, size_t id, T &&t) {
  for (auto i = v.size(); i <= id; ++i) {
    v.push_back(t);
  }
}

} // namespace graphxx