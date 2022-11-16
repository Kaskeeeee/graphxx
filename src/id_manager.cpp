#include "id_manager.hpp"
#include <cassert>
#include <limits>

namespace graph::utils {
IdManager::IdManager(Id lower_bound, Id upper_bound)
    : _lower{lower_bound}, _upper{upper_bound}, _max_reached_id{lower_bound},
      _upper_bound_reached{false} {};

Id IdManager::allocate() {
  auto id = _max_reached_id;

  if (!_pool.empty()) {
    id = *_pool.begin();
    _pool.erase(_pool.begin());
    return id;
  }

  assert(!_upper_bound_reached);

  if (_max_reached_id == _upper) {
    _upper_bound_reached = true;
  }

  ++_max_reached_id;
  return id;
}

void IdManager::free(Id id) {
  assert(id < _max_reached_id);
  _pool.insert(id);
}
} // namespace graph::utils