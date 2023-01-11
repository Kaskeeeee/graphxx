#pragma once

#include "base.hpp"

#include <set>

namespace graph::utils {

/// @brief Class used to manage ids in graph structures
/// it can be used to allocate ids in the range [lowerbound, upperbound)
class IdManager {
public:
  IdManager(Id lower_bound, Id upper_bound);

  /// @brief it allocates the next available id
  /// @returns an Id
  [[nodiscard]] Id allocate();

  /// @brief it frees an allocated id
  /// @param _ the id to free
  void free(Id);

private:
  Id _lower, _upper;
  Id _countup;
  std::set<Id> _pool;
};

} // namespace graph::utils