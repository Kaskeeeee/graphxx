#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>

namespace graph::io::graphviz {
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  GraphvizTraits<G::Tag> Traits;

  out << Traits::name() << " "
      << "{" << std::endl;

  for (Edge edge ::graph.edges()) {
    out << edge.u.id << Traits::delimiter() << edge.v << " ";
    out << ";" << std::endl;
  }

  out << "}" << std::endl;
}
} // namespace graph::io::graphviz