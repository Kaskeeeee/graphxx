#pragma once

namespace graph::algorithms {

/// @brief Status of the vertices during the visit algorithms
enum class VertexStatus {
  READY,
  WAITING,
  PROCESSED,
};

} // namespace graph::algorithms