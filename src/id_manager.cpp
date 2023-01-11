#include "id_manager.hpp"
#include "base.hpp"
#include "exceptions.hpp"

namespace graph::utils {
IdManager::IdManager(Id lower_bound, Id upper_bound)
    : _lower{lower_bound}, _upper{upper_bound}, _countup{lower_bound} {};

[[nodiscard]] Id IdManager::allocate() {
  if (!_pool.empty()) {
    auto id = *_pool.begin();
    _pool.erase(_pool.begin());
    return id;
  }

  [[unlikely]] if (_countup == _upper) {
    throw exceptions::OutOfIdsException();
  }

  return _countup++;
}

void IdManager::free(Id id) { _pool.insert(id); }
} // namespace graph::utils