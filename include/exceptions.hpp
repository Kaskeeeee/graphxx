#pragma once
#include "base.hpp"
#include <exception>
#include <string>

namespace graph::exceptions {

struct GraphException : public std::exception {
  GraphException(std::string message);

  const char *what() const noexcept override;

protected:
  std::string _message;
};

struct OutOfIdsException : GraphException {
  OutOfIdsException();
};

struct NoSuchVertexException : GraphException {
  NoSuchVertexException();
};

struct NoSuchEdgeException : GraphException {
  NoSuchEdgeException();
};

struct InvariantViolationException : GraphException {
  InvariantViolationException(std::string message);
};

} // namespace graph::exceptions