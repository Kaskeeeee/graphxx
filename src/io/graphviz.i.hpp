#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>
#include <functional>
#include <string>

namespace graph::io::graphviz {

template <Directedness D> std::string GraphvizTraits<D>::name() { return "digraph"; }
template <Directedness D> std::string GraphvizTraits<D>::delimiter() { return "->"; }

template <> struct GraphvizTraits<Directedness::UNDIRECTED> {
  static std::string name() { return "graph"; }
  static std::string delimiter() { return "--"; }
};

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<std::string(Vertex)> label_vertex,
               std::function<std::string(Edge)> label_edge) {

  using Traits = GraphvizTraits<G::directedness>;

  out << Traits::name() << " " << "{" << std::endl;

  for (auto vertex : graph.vertices()) {
    std::string vertex_label = label_vertex(vertex);
    if (vertex_label != "") {
      out << vertex.id << " [label=\"" << vertex_label << "\"]" << ";" << std::endl;
    }
  }

  for (auto edge : graph.edges()) {
    out << edge.u.id << Traits::delimiter() << edge.v.id;
    std::string edge_label = label_edge(edge);
    if (edge_label != "") {
      out << " [label=\"" << edge_label << "\"]";
    }
    out << ";" << std::endl;
  }

  out << "}" << std::endl;
}

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<std::string(Vertex)> label_vertex) {
  return serialize(out, graph, label_vertex, [](Edge) { return ""; });
}

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  return serialize(
      out, graph, 
      [](Vertex v) { return ""; },
      [](Edge) { return ""; });
}

} // namespace graph::io::graphviz