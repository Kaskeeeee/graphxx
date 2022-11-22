#include "exceptions.hpp"
#include "base.hpp"

namespace graph::exceptions {
char *OutOfIdsException::what() { return "Out of IDs exception"; };

char *NoSuchVertexException::what() {
  return "Vertex is missing from graph exception";
};

char *NoSuchEdgeException::what() {
  return "Edge is missing from graph exeception";
};

char *NegativeCycleException::what() {
  return "Found negative cycle in the graph";
};

} // namespace graph::exceptions