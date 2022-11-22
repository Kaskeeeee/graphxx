#pragma once
#include "base.hpp"
#include <exception>

namespace graph {
namespace exceptions {

struct OutOfIdsException : std::exception {
  char *what();
};

struct NoSuchVertexException : std::exception {
  char *what();
};

struct NoSuchEdgeException : std::exception {
  char *what();
};

} // namespace exceptions
} // namespace graph