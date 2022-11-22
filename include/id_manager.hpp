#pragma once
#include "base.hpp"

#include <set>

namespace graph::utils {

/// Class used to manage ids in graph structures
/// it can be used to allocate ids in the range [lowerbound, upperbound)
class IdManager {
public:
  IdManager(Id lower_bound, Id upper_bound);
  [[nodiscard]] Id allocate();
  void free(Id);

private:
  Id _lower, _upper;
  Id _countup;
  std::set<Id> _pool;
};

} // namespace graph::utils