/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
 *
 * @license{<blockquote>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </blockquote>}
 *
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "graph_utils.hpp"
#include "io/graphml.hpp"
#include "pugixml.hpp"
#include "string_utils.hpp"

#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

namespace graphxx::io {

template <concepts::Graph G>
void graphml_serialize(
    std::ostream &out, const G &graph,
    std::function<GraphMLProperties(Vertex<G>)> get_vertex_properties,
    std::function<GraphMLProperties(Vertex<G>, Vertex<G>)>
        get_edge_properties) {

  out << XML_HEADER << std::endl;
  out << GRAPHML_ROOT_OPEN << std::endl;

  int key_count = 0;
  std::unordered_map<std::string, std::string> vertex_key_ids;
  std::unordered_map<std::string, std::string> edge_key_ids;

  std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;

  // declaring GraphML-Attributes for nodes
  for (auto vertex : get_sorted_vertices(graph)) {
    GraphMLProperties vertex_properties = get_vertex_properties(vertex);
    for (const auto &[name, value] : vertex_properties) {
      if (vertex_key_ids.contains(name)) {
        continue;
      }

      std::string key_id = "k" + std::to_string(key_count++);
      vertex_key_ids[name] = key_id;
      out << "\t"
          << "<key id=\"" << key_id << "\" for=\"node\""
          << " attr.name=\"" << name << "\""
          << " attr.type=\"string\"/>" << std::endl;
    }
  }

  // declaring GraphML-Attributes for edges
  for (auto [source, target] : get_sorted_edges(graph)) {
    if (G::DIRECTEDNESS == Directedness::UNDIRECTED)
      if (inserted_edges.contains({target, source}))
        continue;

    inserted_edges.insert({source, target});

    GraphMLProperties edge_properties = get_edge_properties(source, target);
    for (const auto &[name, value] : edge_properties) {
      if (edge_key_ids.contains(name)) {
        continue;
      }

      std::string key_id = "k" + std::to_string(key_count++);
      edge_key_ids[name] = key_id;
      out << "\t"
          << "<key id=\"" << key_id << "\" for=\"edge\""
          << " attr.name=\"" << name << "\""
          << " attr.type=\"string\"/>" << std::endl;
    }
  }

  // graph default directedness
  std::string edgedefault =
      (G::DIRECTEDNESS == Directedness::UNDIRECTED) ? "undirected" : "directed";
  out << "\t"
      << "<graph id=\"G\" edgedefault=\"" << edgedefault << "\">" << std::endl;

  // declaring nodes
  for (auto vertex : get_sorted_vertices(graph)) {
    out << "\t\t"
        << "<node id=\"n" << vertex << "\">" << std::endl;

    // defining GraphML-Attribute values for nodes
    GraphMLProperties vertex_properties = get_vertex_properties(vertex);
    if (!vertex_properties.empty()) {
      for (const auto &[key, value] : vertex_properties) {
        out << "\t\t\t"
            << "<data key=\"" << vertex_key_ids[key] << "\">" << value
            << "</data>" << std::endl;
      }
    }

    out << "\t\t"
        << "</node>" << std::endl;
  }

  // declaring edges
  int edge_count = 0;
  inserted_edges.clear();
  for (auto [source, target] : get_sorted_edges(graph)) {
    if (!inserted_edges.contains({source, target})) {

      if (G::DIRECTEDNESS == Directedness::UNDIRECTED)
        if (inserted_edges.contains({target, source}))
          continue;

      inserted_edges.insert({source, target});

      out << "\t\t"
          << "<edge id=\"e" << edge_count++ << "\" source=\"n" << source
          << "\" target=\"n" << target << "\">" << std::endl;

      // defining GraphML-Attribute values for edges
      GraphMLProperties edge_properties = get_edge_properties(source, target);
      if (!edge_properties.empty()) {
        for (const auto &[key, value] : edge_properties) {
          out << "\t\t\t"
              << "<data key=\"" << edge_key_ids[key] << "\">" << value
              << "</data>" << std::endl;
        }
      }

      out << "\t\t"
          << "</edge>" << std::endl;
    }
  }

  out << "\t</graph>" << std::endl;
  out << GRAPHML_ROOT_CLOSE << std::endl;
}

template <concepts::Graph G>
void graphml_serialize(
    std::ostream &out, const G &graph,
    std::function<GraphMLProperties(Vertex<G>)> get_vertex_properties) {
  GraphMLProperties empty_map;
  return graphml_serialize(out, graph, get_vertex_properties,
                           [&](Vertex<G>, Vertex<G>) { return empty_map; });
}

template <concepts::Graph G>
void graphml_serialize(std::ostream &out, const G &graph) {
  GraphMLProperties empty_map;

  return graphml_serialize(
      out, graph, [&](Vertex<G>) { return empty_map; },
      [&](Vertex<G>, Vertex<G>) { return empty_map; });
}

template <concepts::Graph G>
void graphml_deserialize(
    std::istream &in, G &graph,
    std::unordered_map<Vertex<G>, GraphMLProperties> &vertex_properties,
    std::unordered_map<Edge<G>, GraphMLProperties, XorTupleHash<Edge<G>>>
        &edge_properties) {
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load(in);

  std::unordered_map<std::string, Vertex<G>> vertices_ids = {};
  std::unordered_map<std::string, std::string> attributes_names = {};

  pugi::xml_node graphml_node = doc.child("graphml");

  if (graphml_node == nullptr) {
    throw exceptions::BadGraphmlParseException();
  }

  pugi::xml_node graph_node = graphml_node.child("graph");
  pugi::xml_attribute directedness = graph_node.attribute("edgedefault");

  // check graph directedness
  if (G::DIRECTEDNESS == Directedness::UNDIRECTED &&
      std::string(directedness.value()) != "undirected") {
    throw exceptions::UndirectedGraphParseException();
  }

  if (G::DIRECTEDNESS == Directedness::DIRECTED &&
      std::string(directedness.value()) != "directed") {
    throw exceptions::DirectedGraphParseException();
  }

  // get GraphML-Attributes
  for (pugi::xml_node key_node : graphml_node.children("key")) {
    pugi::xml_attribute attribute_id = key_node.attribute("id");
    pugi::xml_attribute attribute_name = key_node.attribute("attr.name");
    attributes_names[attribute_id.value()] =
        std::string(attribute_name.value());
  }

  // get vertices
  int vertices_count = 0;
  for (pugi::xml_node vertex_node : graph_node.children("node")) {
    pugi::xml_attribute vertex_id = vertex_node.attribute("id");
    Vertex<G> v = vertices_count++;
    graph.add_vertex();
    vertices_ids[vertex_id.value()] = v;

    // vertices attributes
    for (pugi::xml_node attribute_node : vertex_node.children("data")) {
      pugi::xml_attribute attribute_key = attribute_node.attribute("key");
      std::string value = utils::trim(attribute_node.child_value());
      vertex_properties[v][attributes_names[attribute_key.value()]] = value;
    }
  }

  // get edges
  for (pugi::xml_node edge_node : graph_node.children("edge")) {
    pugi::xml_attribute source_id = edge_node.attribute("source");
    pugi::xml_attribute target_id = edge_node.attribute("target");

    Vertex<G> source = vertices_ids[source_id.value()];
    Vertex<G> target = vertices_ids[target_id.value()];
    graph.add_edge(source, target);

    // edges attributes
    for (pugi::xml_node attribute_node : edge_node.children("data")) {
      pugi::xml_attribute attribute_key = attribute_node.attribute("key");
      std::string value = utils::trim(attribute_node.child_value());
      edge_properties[{source, target}]
                     [attributes_names[attribute_key.value()]] = value;

      if (G::DIRECTEDNESS == Directedness::UNDIRECTED) {
        edge_properties[{target, source}]
                       [attributes_names[attribute_key.value()]] = value;
      }
    }
  }
}

} // namespace graphxx::io
