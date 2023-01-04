#include "base.hpp"
#include "graph_concepts.hpp"
#include "io/graphml.hpp"

#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <string>

namespace graph::io::graphml {

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphMLProperties(Vertex)> get_vertex_properties,
               std::function<GraphMLProperties(Edge)> get_edge_properties) {

  out << XML_HEADER << std::endl;
  out << GRAPHML_ROOT_OPEN << std::endl;

  std::unordered_map<std::string, std::string> vertex_key_ids;
  std::unordered_map<std::string, std::string> edge_key_ids;
  int key_count = 0;

  // declaring GraphML-Attributes for nodes
  for (auto vertex : graph.vertices()) {
    GraphMLProperties vertex_properties = get_vertex_properties(vertex);
    for (const auto &[name, value] : vertex_properties) {
      if (vertex_key_ids.contains(name)) 
        continue;
      
      std::string key_id = "k" + std::to_string(key_count++);
      vertex_key_ids[name] = key_id;
      out << "\t" << "<key id=\"" << key_id << "\" for=\"node\""
          << " attr.name=\"" << name << "\""
          << " attr.type=\"string\"/>" 
          << std::endl;
    }
  }

  // declaring GraphML-Attributes for edges
  for (auto edge : graph.edges()) {
    GraphMLProperties edge_properties = get_edge_properties(edge);
    for (const auto &[name, value] : edge_properties) {
      if (edge_key_ids.contains(name)) 
        continue;
        
      std::string key_id = "k" + std::to_string(key_count++);
      edge_key_ids[name] = key_id;
      out << "\t" << "<key id=\"k" << key_id << "\" for=\"edge\""
          << " attr.name=\"" << name << "\""
          << " attr.type=\"string\"/>" 
          << std::endl;
    }
  }

  // graph default directedness
  std::string edgedefault = (G::directedness == Directedness::UNDIRECTED) ? "undirected" : "directed";
  out << "\t" << "<graph id=\"G\" edgedefault=\"" << edgedefault << "\"" << std::endl;

  // declaring nodes
  for (auto vertex : graph.vertices()) {
    out << "\t\t" << "<node id=\"n" << vertex.id << "\">" << std::endl;

    // defining GraphML-Attribute values for nodes
    GraphMLProperties vertex_properties = get_vertex_properties(vertex);
    if (!vertex_properties.empty()) {
      for (const auto &[key, value] : vertex_properties) {
        out << "\t\t\t"
            << "<data key=\"" << vertex_key_ids[key] << "\">" << value 
            << "</data>" << std::endl;
      }
    }

    out << "\t\t" << "</node>" << std::endl;
  }

  // declaring edges
  std::set<Id> inserted_edges;
  for (auto edge : graph.edges()) {
    if (!inserted_edges.contains(edge)) {
      inserted_edges.insert(edge);

      out << "\t\t" << "<edge id=\"e" << edge.id 
      << "\" source=\"n" << edge.u.id 
      << "\" target=\"n" << edge.v.id 
      << "\">" << std::endl ;

      // defining GraphML-Attribute values for edges
      GraphMLProperties edge_properties = get_edge_properties(edge);
      if (!edge_properties.empty()) {
        for (const auto &[key, value] : edge_properties) {
          out << "\t\t\t"
              << "<data key=\"" << edge_key_ids[key] << "\">" << value 
              << "</data>" << std::endl;
        }
      }

      out << "\t\t" << "</edge>" << std::endl;
    }
  }

  out << "\t</graph>" << std::endl;
  out << GRAPHML_ROOT_CLOSE << std::endl;
}

template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphMLProperties(Vertex)> get_vertex_properties) {
  GraphMLProperties empty_map;
  return serialize(out, graph, get_vertex_properties,
                   [&](Edge) { return empty_map; });
}

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  GraphMLProperties empty_map;

  return serialize(
      out, graph, [&](Vertex) { return empty_map; },
      [&](Edge) { return empty_map; });
}

} // namespace graph::io::graphml