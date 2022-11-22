#pragma once
namespace graph {

namespace algorithms {

enum class VertexStatus {
  READY,
  WAITING,
  PROCESSED,
};

template <typename T>
concept IsNumberLike = std::is_arithmetic_v<T>;

} // namespace algorithms

} // namespace graph