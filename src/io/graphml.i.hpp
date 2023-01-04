#include "base.hpp"
#include "graph_concepts.hpp"
#include "io/graphml.hpp"
#include "pugixml.hpp"

#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <string.h>
#include <unordered_map>

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

template <concepts::Graph G>
void deserialize(
    std::istream &in, G &graph) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(in);

    std::unordered_map<std::string, Vertex> vertices_ids = {};
    std::unordered_map<std::string, std::string> attributes_names = {};

    // pugi::xml_node graphml_node = doc.child("graphml");

    // if (!graphml_node)
    //   ; // TODO: unsupported format

    // pugi::xml_node graph_node = graphml_node.child("graph");
    // pugi::xml_attribute directedness = graph_node.attribute("edgedefault");

    // // get GraphML-Attributes
    // for (pugi::xml_node key_node: graphml_node.children("key"))
    // {
    //   pugi::xml_attribute attribute_id = key_node.attribute("id");
    //   pugi::xml_attribute attribute_name = key_node.attribute("attr.name");
    //   attributes_names[attribute_id] = attribute_name;
    // }

    // // get vertices
    // for (pugi::xml_node vertex_node: graph_node.children("node"))
    // {
    //     pugi::xml_attribute vertex_id = vertex_node.attribute("id");
    //     Vertex v = graph.add_vertex();
    //     vertices_ids[vertex_id.value()] = v;

    //     // vertices attributes
    //     for (pugi::xml_node attribute_node: vertex_node.children("data"))
    //     {
    //       pugi::xml_attribute attribute_key = attribute_node.attribute("key");
    //       std::string value = attribute_node.value();
          
    //       // INSERT KEY
    //     }
    // }

    // // get edges
    // for (pugi::xml_node edge_node: graph_node.children("edge"))
    // {
    //     pugi::xml_attribute source_id = edge_node.attribute("source");
    //     pugi::xml_attribute target_id = edge_node.attribute("target");
    //     Edge e = graph.add_edge(vertices_ids[source_id.value()], vertices_ids[target_id.value()]);

    //     // edges attributes
    //     for (pugi::xml_node attribute_node: edge_node.children("data"))
    //     {
    //       pugi::xml_attribute attribute_key = attribute_node.attribute("key");
    //       std::string value = attribute_node.value();

    //       // INSERT KEY
    //     }
    // }
}

} // namespace graph::io::graphml