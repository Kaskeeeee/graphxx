#pragma once
#include "base.hpp"
#include <exception>
#include <string>

namespace graph::exceptions {

/// @brief Generic graph exceptions from which the other ones inherits
/// its extends std::exception by adding a custom message
struct GraphException : public std::exception {
  explicit GraphException(std::string message);

  [[nodiscard]] const char *what() const noexcept override;

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
  explicit InvariantViolationException(const std::string &message);
};

struct DirectedGraphParseException: GraphException {
  DirectedGraphParseException();
};

struct UndirectedGraphParseException: GraphException {
  UndirectedGraphParseException();
};

struct BadGraphvizParseException: GraphException {
  BadGraphvizParseException();
};

struct BadGraphmlParseException: GraphException {
  BadGraphmlParseException();
};

struct BadMatrixMarketParseException: GraphException {
  BadMatrixMarketParseException();
};

} // namespace graph::exceptions