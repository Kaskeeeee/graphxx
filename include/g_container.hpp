#pragma once

#include <memory>
#include <tuple>

namespace graph {
namespace container {

template <int...> struct seq {};
template <int N, int... S> struct gens : gens<N - 1, N - 1, S...> {};
template <int... S> struct gens<0, S...> {
  typedef seq<S...> type;
};

template <typename MapIt, typename RawContainer, typename... Args> class C {
public:
  MapIt begin() { return dispatch(_container.begin()); }
  MapIt end() { return dispatch(_container.end()); }

  bool empty() const {
    if (!_needs_manual_count) {
      return _container.empty();
    }
    return size() == 0;
  }

  size_t size() const {
    if (!_needs_manual_count) {
      return _container.size();
    }

    size_t count = 0;
    for (auto it = begin(); it != end(); ++it) {
      ++count;
    }
    return count;
  }

  MapIt erase(MapIt it) {
    return MapIt{_container.erase(it.get_original_iterator())};
  }

  C(RawContainer container, bool needs_manual_count, const Args &...args)
      : _container{container},
        _needs_manual_count{needs_manual_count}, _args{args...} {};

private:
  MapIt dispatch(decltype(std::declval<RawContainer>().begin()) position) {
    return construct(position, (typename gens<sizeof...(Args)>::type()));
  }

  template <int... S>
  MapIt construct(decltype(std::declval<RawContainer>().begin()) position,
                  seq<S...>) {
    return MapIt(position, std::get<S>(_args)...);
  }

  RawContainer _container;
  bool _needs_manual_count;
  std::tuple<Args...> _args;
};

} // namespace container
} // namespace graph