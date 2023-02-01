#pragma once
#include <tuple>

namespace graphxx {

template <size_t StartingIndex, typename... Elements>
auto get_elements_from_index(std::tuple<Elements...> t) {
  return [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
    return std::tuple{std::get<StartingIndex + Offsets>(t)...};
  }
  (std::make_index_sequence<sizeof...(Elements) - StartingIndex>());
}

template <size_t StartingIndex, typename... Elements, typename... AttributesType>
void set_elements_from_index(std::tuple<Elements...> &t, std::tuple<AttributesType...> &attributes) {
  [&]<size_t... Offsets>(std::index_sequence<Offsets...>) {
    ((std::get<StartingIndex + Offsets>(t) = std::get<Offsets>(attributes)), ...)
  }
  (std::make_index_sequence<sizeof...(Elements) - StartingIndex>());
}

} // namespace graphxx