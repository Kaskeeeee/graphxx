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

  C(RawContainer container, const Args &...args)
      : _container{container}, _args{args...} {};

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
  std::tuple<Args...> _args;
};

} // namespace container
} // namespace graph