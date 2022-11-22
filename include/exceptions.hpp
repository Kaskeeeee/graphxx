#pragma once
#include "base.hpp"
#include <exception>

namespace graph::exceptions {

struct OutOfIdsException : std::exception {
  char *what();
};

struct NoSuchVertexException : std::exception {
  char *what();
};

struct NoSuchEdgeException : std::exception {
  char *what();
};

struct NegativeCycleException : std::exception {
  char *what();
};

} // namespace graph::exceptions