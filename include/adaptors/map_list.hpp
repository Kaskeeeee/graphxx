
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