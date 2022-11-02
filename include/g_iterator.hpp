#pragma once
#include <concepts>
#include <type_traits>

namespace graph {
namespace it {
// iterator interface
template <typename Derived, typename Value, typename ItType> class MapIterator {
public:
  using value_type = std::remove_const<Value>::type;
  using reference = Value &;
  using pointer = Value *;

  MapIterator(ItType it) : _it{it} {};

  Value operator*() { return static_cast<Derived *>(this)->value(); };

  Derived &operator++() {
    static_cast<Derived *>(this)->next();
    return *static_cast<Derived *>(this);
  };

  Derived operator++(int) {
    auto tmp = *static_cast<Derived *>(this);
    static_cast<Derived *>(this)->next();
    return tmp;
  };

  bool operator==(const Derived &other) { return _it == other._it; }

  bool operator!=(const Derived &other) { return _it != other._it; }

  ItType get_original_iterator() { return _it; }

protected:
  using _Super = MapIterator;
  ItType _it;
};
} // namespace it
} // namespace graph