#pragma once
#include <tuple>

namespace graphxx {
template <size_t StartingIndex, typename... Elements>
auto elements_from_index(std::tuple<Elements...> t) {
  return [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
    return std::tuple{std::get<StartingIndex + Offsets>(t)...};
  }
  (std::make_index_sequence<sizeof...(Elements) - StartingIndex>());
}
} // namespace graphxx