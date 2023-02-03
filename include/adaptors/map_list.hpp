
#include <cstddef>  // std::ptrdiff_t
#include <iterator> // std::bidirectional_iterator_tag
#include <unordered_map>

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
  class iterator {
  public:
    using difference_type = int;
    using value_type = Value;
    using reference = Value &;
    using pointer = Value *;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator(){};
    iterator(typename Base::iterator it) : _it{it} {}

    reference operator*() const { return _it->second; }
    iterator &operator++() {
      ++_it;
      return *this;
    }
    iterator operator++(int) {
      iterator i = *this;
      ++_it;
      return i;
    }
    iterator &operator--() {
      --_it;
      return *this;
    }
    iterator operator--(int) {
      iterator i = *this;
      --_it;
      return i;
    }
    bool operator==(const iterator &other) const { return _it == other._it; }
    bool operator!=(const iterator &other) const { return !(*this == other); }

  private:
    typename Base::iterator _it;
  };

  class const_iterator {
  public:
    using difference_type = int;
    using value_type = Value;
    using reference = const Value &;
    using pointer = const Value *;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator(typename Base::const_iterator it) : _it{it} {}

    reference operator*() const { return _it->second; }
    const_iterator &operator++() {
      ++_it;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator i = *this;
      ++_it;
      return i;
    }
    const_iterator &operator--() {
      --_it;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator i = *this;
      --_it;
      return i;
    }
    bool operator==(const const_iterator &other) const {
      return _it == other._it;
    }
    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }

  private:
    typename Base::const_iterator _it;
  };

  iterator begin() { return iterator(Base::begin()); }
  iterator end() { return iterator(Base::end()); }
  const_iterator begin() const { return const_iterator(Base::begin()); }
  const_iterator end() const { return const_iterator(Base::end()); }
};

} // namespace graphxx