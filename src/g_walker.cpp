#include "g_walker.hpp"

namespace graph {
namespace walker {
template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
Walker<GraphType, Output, ItConstructorParamType>::It
Walker<GraphType, Output, ItConstructorParamType>::begin() {
  return {WM::get_begin(_graph)};
};

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
Walker<GraphType, Output, ItConstructorParamType>::It
Walker<GraphType, Output, ItConstructorParamType>::end() {
  return {WM::get_end(_graph)};
};

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
Output Walker<GraphType, Output, ItConstructorParamType>::It::operator*() {
  return WM::get_output(_graph, _it);
};

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
Walker<GraphType, Output, ItConstructorParamType>::It &
Walker<GraphType, Output, ItConstructorParamType>::It::operator++() {
  return WM::next(_graph, _it);
};

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
Walker<GraphType, Output, ItConstructorParamType>::It
Walker<GraphType, Output, ItConstructorParamType>::It::operator++(int) {
  It tmp = *this;
  WalkerManager::next(_graph, _it);
  return tmp;
};

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
bool Walker<GraphType, Output, ItConstructorParamType>::It::operator==(
    const It &other) {
  return _it == other._it;
}

template <graph::concepts::Graph GraphType, typename Output,
          typename ItConstructorParamType>
bool Walker<GraphType, Output, ItConstructorParamType>::It::operator!=(
    const It &other) {
  return _it != other._it;
}
} // namespace walker
} // namespace graph