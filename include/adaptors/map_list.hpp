/**
 * @file This file contains an adapater to iterate over a map
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

#include <iterator>      // std::bidirectional_iterator_tag
#include <unordered_map> // std::unordered_map

namespace graphxx {

/// @brief A map adapter which provides an iterator on the values instead
///       of iterating on key value pairs.
/// @tparam Key Type of the key
/// @tparam Value Type of the value
template <typename Key, typename Value>
class MapList : public std::unordered_map<Key, Value> {
public:
  using Base = std::unordered_map<Key, Value>;

  /// @brief custom iterator that returns values instead of key value pairs
  class Iterator {
  public:
    using difference_type = int;
    using value_type = Value;
    using reference = Value &;
    using pointer = Value *;
    using iterator_category = std::bidirectional_iterator_tag;

    Iterator() = default;
    Iterator(typename Base::iterator it) : _it{it} {}

    reference operator*() const { return _it->second; }
    Iterator &operator++() {
      ++_it;
      return *this;
    }
    Iterator operator++(int) {
      Iterator i = *this;
      ++_it;
      return i;
    }
    Iterator &operator--() {
      --_it;
      return *this;
    }
    Iterator operator--(int) {
      Iterator i = *this;
      --_it;
      return i;
    }
    bool operator==(const Iterator &other) const { return _it == other._it; }
    bool operator!=(const Iterator &other) const { return !(*this == other); }

  private:
    typename Base::iterator _it;
  };

  /// @brief custom const iterator that returns values instead of key value
  /// pairs
  class ConstIterator {
  public:
    using difference_type = int;
    using value_type = Value;
    using reference = const Value &;
    using pointer = const Value *;
    using iterator_category = std::bidirectional_iterator_tag;

    ConstIterator() = default;
    ConstIterator(typename Base::const_iterator it) : _it{it} {}

    reference operator*() const { return _it->second; }
    ConstIterator &operator++() {
      ++_it;
      return *this;
    }
    ConstIterator operator++(int) {
      ConstIterator i = *this;
      ++_it;
      return i;
    }
    ConstIterator &operator--() {
      --_it;
      return *this;
    }
    ConstIterator operator--(int) {
      ConstIterator i = *this;
      --_it;
      return i;
    }
    bool operator==(const ConstIterator &other) const {
      return _it == other._it;
    }
    bool operator!=(const ConstIterator &other) const {
      return !(*this == other);
    }

  private:
    typename Base::const_iterator _it;
  };

  Iterator begin() { return Iterator(Base::begin()); }
  Iterator end() { return Iterator(Base::end()); }
  ConstIterator begin() const { return ConstIterator(Base::begin()); }
  ConstIterator end() const { return ConstIterator(Base::end()); }
};

} // namespace graphxx