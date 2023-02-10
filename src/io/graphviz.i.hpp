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
#include "io/graphviz.hpp"
#include "utils/string_utils.hpp"

#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

namespace graphxx::io {

template <Directedness D> std::string GraphvizTraits<D>::name() {
  return "digraph";
}
template <Directedness D> std::string GraphvizTraits<D>::delimiter() {
  return "->";
}

template <> struct GraphvizTraits<Directedness::UNDIRECTED> {
  static std::string name() { return "graph"; }
  static std::string delimiter() { return "--"; }
};

template <concepts::Graph G>
void graphviz_serialize(
    std::ostream &out, const G &graph,
    const std::function<GraphvizProperties(Vertex<G>)> &get_vertex_properties,
    const std::function<GraphvizProperties(Vertex<G>, Vertex<G>)>
        &get_edge_properties) {

  using Traits = GraphvizTraits<G::DIRECTEDNESS>;

  out << Traits::name() << " "
      << "{" << std::endl;

  // insert vertices properties
  for (auto vertex : get_sorted_vertices(graph)) {
    out << vertex;
    GraphvizProperties vertex_properties = get_vertex_properties(vertex);
    if (!vertex_properties.empty()) {
      out << " [";
      bool comma = false;
      for (const auto &[key, value] : vertex_properties) {
        if (comma) {
          out << ",";
        } else {
          comma = true;
        }
        out << key << "=\"" << value << "\"";
      }
      out << "]";
    }
    out << ";" << std::endl;
  }

  // insert edges
  std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;
  for (auto [source, target] : get_sorted_edges(graph)) {
    if (!inserted_edges.contains({source, target})) {

      if (G::DIRECTEDNESS == Directedness::UNDIRECTED)
        if (inserted_edges.contains({target, source}))
          continue;

      inserted_edges.insert({source, target});
      out << source << Traits::delimiter() << target;

      // insert edge properties
      GraphvizProperties edge_properties = get_edge_properties(source, target);
      if (!edge_properties.empty()) {
        out << " [";
        bool comma = false;
        for (const auto &[key, value] : edge_properties) {
          if (comma) {
            out << ",";
          } else {
            comma = true;
          }
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
void graphviz_serialize(
    std::ostream &out, const G &graph,
    const std::function<GraphvizProperties(Vertex<G>)> &get_vertex_properties) {
  GraphvizProperties empty_map;
  return graphviz_serialize(out, graph, get_vertex_properties,
                            [&](Vertex<G>, Vertex<G>) { return empty_map; });
}

template <concepts::Graph G>
void graphviz_serialize(std::ostream &out, const G &graph) {
  GraphvizProperties empty_map;
  return graphviz_serialize(
      out, graph, [&](Vertex<G>) { return empty_map; },
      [&](Vertex<G>, Vertex<G>) { return empty_map; });
}

GraphvizProperties parse_properties(std::string &attributes_list) {
  GraphvizProperties attributes;

  const std::string replacement_token("$parsed$");
  std::string attributes_body =
      utils::get_text_between_delimiters(attributes_list, "[", "]");

  std::vector<std::string> quote_values;
  size_t quote_start = 0;
  size_t quote_end = 0;
  while (utils::get_text_between_delimiters(attributes_body, quote_start,
                                            quote_end, "\"", "\"")) {
    quote_values.emplace_back(
        attributes_body.substr(quote_start + 1, quote_end - quote_start - 1));
    attributes_body.replace(quote_start, quote_end - quote_start + 1,
                            replacement_token);
  }

  std::vector<std::string> key_value_pairs = utils::split(attributes_body, ",");
  size_t quote_value = 0;
  for (const std::string &key_value_string : key_value_pairs) {
    std::vector<std::string> key_value_pair =
        utils::split(key_value_string, "=");
    if (key_value_pair.size() == 2) {
      attributes[utils::trim(key_value_pair[0])] =
          utils::trim(utils::contains(key_value_pair[1], replacement_token)
                          ? quote_values[quote_value++]
                          : key_value_pair[1]);
    }
  }

  return attributes;
}

template <concepts::Graph G>
void graphviz_deserialize(
    std::istream &in, G &graph,
    std::unordered_map<Vertex<G>, GraphvizProperties> &vertex_properties,
    std::unordered_map<Edge<G>, GraphvizProperties, XorTupleHash<Edge<G>>>
        &edge_properties) {
  using Traits = GraphvizTraits<G::DIRECTEDNESS>;

  const std::vector<std::string> STATEMENT_SEPARATORS = {";", "\r\n", "\n"};
  const std::vector<std::string> graph_types = {
      GraphvizTraits<Directedness::DIRECTED>::name(),
      GraphvizTraits<Directedness::UNDIRECTED>::name()};

  // Convert input stream in a string
  const std::string string_graph((std::istreambuf_iterator<char>(in)),
                                 std::istreambuf_iterator<char>());

  std::unordered_map<std::string, Vertex<G>> inserted_vertices;

  size_t body_start = 0;
  size_t body_end = 0;
  if (!utils::get_text_between_delimiters(string_graph, body_start, body_end,
                                          "{", "}")) {
    throw exceptions::BadGraphvizParseException();
  }

  // parse graph type
  size_t graph_type_index = 0;
  if (utils::find_first_of(string_graph, graph_types, graph_type_index) !=
      std::string::npos) {
    auto directedness = static_cast<Directedness>(graph_type_index);
    if (G::DIRECTEDNESS != directedness) {
      if (directedness == Directedness::DIRECTED) {
        throw exceptions::DirectedGraphParseException();
      }
      throw exceptions::UndirectedGraphParseException();
    }
  } else {
    throw exceptions::BadGraphvizParseException();
  }

  int vertices_count = 0;

  // parse statements
  std::vector<std::string> statements = utils::split(
      string_graph, STATEMENT_SEPARATORS, body_start + 1, body_end);

  for (std::string &statement : statements) {
    size_t attr_start = statement.find_first_of('[');
    size_t attr_end = statement.find_last_of(']');

    // get properties
    GraphvizProperties properties;
    if (attr_start != std::string::npos && attr_end != std::string::npos) {
      properties = parse_properties(statement);
    }

    const std::string statement_body = statement.substr(0, attr_start);
    std::vector<std::string> edge_vertices =
        utils::split(statement_body, Traits::delimiter());

    if (edge_vertices.size() <= 1) {
      // single vertex with properties
      std::string vertex_name = utils::trim(statement_body);
      if (!vertex_name.empty()) {
        if (!inserted_vertices.contains(vertex_name)) {
          Vertex<G> v = vertices_count++;
          graph.add_vertex();
          inserted_vertices[vertex_name] = v;
        }

        vertex_properties[inserted_vertices[vertex_name]] = properties;
      }
    } else {
      // edge with properties
      std::string source_vertex_name = utils::trim(edge_vertices.front());
      if (!source_vertex_name.empty()) {

        if (!inserted_vertices.contains(source_vertex_name)) {
          Vertex<G> source = vertices_count++;
          graph.add_vertex();
          inserted_vertices[source_vertex_name] = source;
        }

        for (size_t i = 1; i < edge_vertices.size(); ++i) {
          std::string target_vertex_name = utils::trim(edge_vertices[i]);
          if (!target_vertex_name.empty()) {

            if (!inserted_vertices.contains(target_vertex_name)) {
              Vertex<G> target = vertices_count++;
              graph.add_vertex();
              inserted_vertices[target_vertex_name] = target;
            }

            graph.add_edge(inserted_vertices[source_vertex_name],
                           inserted_vertices[target_vertex_name]);

            edge_properties[{inserted_vertices[source_vertex_name],
                             inserted_vertices[target_vertex_name]}] =
                properties;

            if (G::DIRECTEDNESS == Directedness::UNDIRECTED) {
              edge_properties[{inserted_vertices[target_vertex_name],
                               inserted_vertices[source_vertex_name]}] =
                  properties;
            }
          }
        }
      }
    }
  }
}

} // namespace graphxx::io