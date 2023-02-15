/**
 * @file This file contains utility functions to manage tuples
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include <cstdint>    // size_t
#include <functional> // std::function
#include <tuple>      // std::tuple

/// utils namespace contains all the utilities functions used throughout the project
namespace graphxx::utils {

/// @brief Extracts from a tuple all the elements starting from an index and
/// returns a new tuple with the extracted elements
/// @tparam ...Elements type of the elements of the input tuple
/// @tparam StartingIndex index of the starting element
/// @param t input tuple
/// @return new tuple with the extracted elements
template <size_t StartingIndex, typename... Elements>
auto get_elements_from_index(const std::tuple<Elements...> &t) {
  return [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
    return std::tuple{std::get<StartingIndex + Offsets>(t)...};
  }
  (std::make_index_sequence<sizeof...(Elements) - StartingIndex>());
}

/// @brief Modifies the elements of a tuple starting from an index
/// @tparam ...Elements type of the elements of the input tuple
/// @tparam ...AttributesType type of the elements to modify
/// @tparam StartingIndex index of the starting element
/// @param t input tuple
/// @param attributes tuple with new elements to replace those in the input
/// tuple
template <size_t StartingIndex, typename... Elements,
          typename... AttributesType>
void set_elements_from_index(std::tuple<Elements...> &t,
                             const std::tuple<AttributesType...> &attributes) {
  [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
    ((std::get<StartingIndex + Offsets>(t) = std::get<Offsets>(attributes)),
     ...);
  }
  (std::make_index_sequence<sizeof...(Elements) - StartingIndex>());
}

/// @brief Provides a function that computes the hash of a tuple
template <typename Tuple> struct XorTupleHash {
  size_t operator()(const Tuple &k) const {
    return [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
      return (std::get<Offsets>(k) ^ ...);
    }
    (std::make_index_sequence<std::tuple_size_v<Tuple>>());
  }
};

} // namespace graphxx::utils