#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>

namespace graph::io::graphviz {
template <concepts::Orientable GraphType> struct GraphvizTraits {
  static std::string name() { return "digraph"; }
  static std::string delimiter() { return "->"; }
};

template <> struct GraphvizTraits<UndirectedGraph> {
  static std::string name() { return "graph"; }
  static std::string delimiter() { return "--"; }
};

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

} // namespace graph::io::graphviz

#include "graphviz.i.hpp"