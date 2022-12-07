#include "io/graphviz.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <fstream>
#include <functional>
#include <string>
#include <map>

namespace graph::io::graphviz {

template <Directedness D> std::string GraphvizTraits<D>::name() { return "digraph"; }
template <Directedness D> std::string GraphvizTraits<D>::delimiter() { return "->"; }

template <> struct GraphvizTraits<Directedness::UNDIRECTED> {
  static std::string name() { return "graph"; }
  static std::string delimiter() { return "--"; }
};

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphvizProperties(Vertex)> get_vertex_properties,
               std::function<GraphvizProperties(Edge)> get_edge_properties) {

  using Traits = GraphvizTraits<G::directedness>;

  out << Traits::name() << " " << "{" << std::endl;

  // insert vertices properties
  for (auto vertex : graph.vertices()) {
    GraphvizProperties vertex_properties = get_vertex_properties(vertex);
    if (!vertex_properties.empty()) {
      out << vertex.id << " [";
      bool comma = false;
      for(const auto &[key, value] : vertex_properties) {
        if (comma) { 
          out << ","; 
        } else comma = true;
        out << key << "=\"" << value << "\"";
      }
      out << "]";
      out << ";" << std::endl;
    }
  }

  // insert edges
  std::set<Id> inserted_edges;
  for (auto edge : graph.edges()) {
    if (!inserted_edges.contains(edge)) {
    inserted_edges.insert(edge);
    out << edge.u.id << Traits::delimiter() << edge.v.id;

    // insert edge properties
    GraphvizProperties edge_properties = get_edge_properties(edge);
    if (!edge_properties.empty()) {
      out << " [";
      bool comma = false;
      for(const auto &[key, value] : edge_properties) {
        if (comma) { 
          out << ","; 
        } else comma = true;
        out << key << "=\"" << value << "\"";
      }
      out << "]";
    }
    out << ";" << std::endl;
    }
  }

  out << "}" << std::endl;
}

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphvizProperties(Vertex)> get_vertex_properties) {
  GraphvizProperties empty_map;
  return serialize(out, graph, get_vertex_properties, [&](Edge) { return empty_map; });
}

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  GraphvizProperties empty_map;

  return serialize(
      out, graph, 
      [&](Vertex) { return empty_map; },
      [&](Edge) { return empty_map; });
}

} // namespace graph::io::graphviz