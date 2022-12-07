#pragma once
#include "base.hpp"
#include <exception>
#include <string>

namespace graph::exceptions {

/// @brief Generic graph exceptions from which the other ones inherits
/// its extends std::exception by adding a custom message
struct GraphException : public std::exception {
  GraphException(std::string message);

  const char *what() const noexcept override;

protected:
  std::string _message;
};

/// @brief Exception thrown when all ids are used
struct OutOfIdsException : GraphException {
  OutOfIdsException();
};

/// @brief Exception thrown when trying to do some operations on vertices that
/// don't exist
struct NoSuchVertexException : GraphException {
  NoSuchVertexException();
};

/// @brief Exception thrown when trying to do some operations on edges that
/// don't exist
struct NoSuchEdgeException : GraphException {
  NoSuchEdgeException();
};

/// @brief Exception thrown when an invariant is violated
struct InvariantViolationException : GraphException {
  InvariantViolationException(std::string message);
};

} // namespace graph::exceptions