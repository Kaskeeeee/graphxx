#include "exceptions.hpp"
#include "base.hpp"

namespace graph::exceptions {
GraphException::GraphException(std::string message) : _message{message} {};

const char *GraphException::what() const noexcept { return _message.c_str(); };

OutOfIdsException::OutOfIdsException()
    : GraphException("Out of IDs exception"){};

NoSuchVertexException::NoSuchVertexException()
    : GraphException("Vertex is missing from graph exception"){};

NoSuchEdgeException::NoSuchEdgeException()
    : GraphException("Edge is missing from graph exeception"){};

InvariantViolationException::InvariantViolationException(std::string message)
    : GraphException("Invariant violation exception: " + message){};

} // namespace graph::exceptions