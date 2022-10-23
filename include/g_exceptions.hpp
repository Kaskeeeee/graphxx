#pragma once
#include <execution>

namespace graph {
namespace exceptions {
class OutOfIdsException : std::exception {
public:
  char *what();
};
} // namespace exceptions
} // namespace graph