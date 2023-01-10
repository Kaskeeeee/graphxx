#include "exceptions.hpp"
#include "base.hpp"
#include <string>

namespace graph::exceptions {
GraphException::GraphException(std::string message) : _message{std::move(message)} {};

const char *GraphException::what() const noexcept { return _message.c_str(); };

OutOfIdsException::OutOfIdsException()
    : GraphException("Out of IDs exception"){};

NoSuchVertexException::NoSuchVertexException()
    : GraphException("Vertex is missing from graph exception"){};

NoSuchEdgeException::NoSuchEdgeException()
    : GraphException("Edge is missing from graph exeception"){};

InvariantViolationException::InvariantViolationException(const std::string &message)
    : GraphException("Invariant violation exception: " + message){};

DirectedGraphParseException::DirectedGraphParseException() 
    : GraphException("Tried to read a directed graph into an undirected graph"){};

UndirectedGraphParseException::UndirectedGraphParseException()
    : GraphException("Tried to read an undirected graph into a directed graph"){};

BadGraphvizParseException::BadGraphvizParseException()
    : GraphException("Bad graphviz file syntax"){};

BadGraphmlParseException::BadGraphmlParseException()
    : GraphException("Bad graphml file syntax"){};

BadMatrixMarketParseException::BadMatrixMarketParseException()
    : GraphException("Bad matrix market file syntax"){};

} // namespace graph::exceptions