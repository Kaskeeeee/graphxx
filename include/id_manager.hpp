#pragma once
#include "base.hpp"

#include <set>

namespace graph::utils {
class IdManager {
public:
  IdManager(Id lower_bound, Id upper_bound);
  Id allocate();
  void free(Id);

private:
  Id _lower, _upper;
  Id _max_reached_id;
  bool _upper_bound_reached;
  std::set<Id> _pool;
};
} // namespace graph::utils