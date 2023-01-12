#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "io/graphviz.hpp"
#include "utils/string_utils.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

namespace graph::io::graphviz {

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
void serialize(std::ostream &out, const G &graph,
               std::function<GraphvizProperties(Vertex)> get_vertex_properties,
               std::function<GraphvizProperties(Edge)> get_edge_properties) {

  using Traits = GraphvizTraits<G::DIRECTEDNESS>;

  out << Traits::name() << " "
      << "{" << std::endl;

  // insert vertices properties
  for (auto vertex : graph.vertices()) {
    GraphvizProperties vertex_properties = get_vertex_properties(vertex);
    if (!vertex_properties.empty()) {
      out << vertex.id << " [";
      bool comma = false;
      for (const auto &[key, value] : vertex_properties) {
        if (comma) {
          out << ",";
        } else
          comma = true;
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
        for (const auto &[key, value] : edge_properties) {
          if (comma) {
            out << ",";
          } else
            comma = true;
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
void serialize(
    std::ostream &out, const G &graph,
    std::function<GraphvizProperties(Vertex)> get_vertex_properties) {
  GraphvizProperties empty_map;
  return serialize(out, graph, get_vertex_properties,
                   [&](Edge) { return empty_map; });
}

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  GraphvizProperties empty_map;
  return serialize(
      out, graph, [&](Vertex) { return empty_map; },
      [&](Edge) { return empty_map; });
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
    if (key_value_pair.size() == 2)
      attributes[utils::trim(key_value_pair[0])] =
          utils::trim(utils::contains(key_value_pair[1], replacement_token)
                          ? quote_values[quote_value++]
                          : key_value_pair[1]);
  }

  return attributes;
}

template <concepts::Graph G>
void deserialize(std::istream &in, G &graph,
                 std::unordered_map<Id, GraphvizProperties> &vertex_properties,
                 std::unordered_map<Id, GraphvizProperties> &edge_properties) {
  using Traits = GraphvizTraits<G::DIRECTEDNESS>;

  const std::vector<std::string> STATEMENT_SEPARATORS = {";", "\r\n", "\n"};
  const std::vector<std::string> graph_types = {
      GraphvizTraits<Directedness::DIRECTED>::name(),
      GraphvizTraits<Directedness::UNDIRECTED>::name()};

  // Convert input stream in a string
  const std::string string_graph((std::istreambuf_iterator<char>(in)),
                                 std::istreambuf_iterator<char>());

  std::unordered_map<std::string, Id> inserted_vertices;

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
    Directedness directedness = static_cast<Directedness>(graph_type_index);
    if (G::DIRECTEDNESS != directedness) {
      if (directedness == Directedness::DIRECTED)
        throw exceptions::DirectedGraphParseException();
      else
        throw exceptions::UndirectedGraphParseException();
    }
  } else {
    throw exceptions::BadGraphvizParseException();
  }

  // parse statements
  std::vector<std::string> statements = utils::split(
      string_graph, STATEMENT_SEPARATORS, body_start + 1, body_end);

  for (std::string &statement : statements) {
    size_t attr_start = statement.find_first_of('[');
    size_t attr_end = statement.find_last_of(']');

    // get properties
    GraphvizProperties properties;
    if (attr_start != std::string::npos && attr_end != std::string::npos)
      properties = parse_properties(statement);

    const std::string statement_body = statement.substr(0, attr_start);
    std::vector<std::string> edge_vertices =
        utils::split(statement_body, Traits::delimiter());

    if (edge_vertices.size() <= 1) {
      // single vertex with properties
      std::string vertex_name = utils::trim(statement_body);
      if (!vertex_name.empty()) {
        if (!inserted_vertices.contains(vertex_name)) {
          Vertex v = graph.add_vertex();
          inserted_vertices[vertex_name] = v;
        }

        vertex_properties[inserted_vertices[vertex_name]] = properties;
      }
    } else {
      // edge with properties
      std::string source_vertex_name = utils::trim(edge_vertices.front());
      if (!source_vertex_name.empty()) {

        if (!inserted_vertices.contains(source_vertex_name)) {
          Vertex source = graph.add_vertex();
          inserted_vertices[source_vertex_name] = source;
        }

        for (int64_t i = 1; i < edge_vertices.size(); ++i) {
          std::string target_vertex_name = utils::trim(edge_vertices[i]);
          if (!target_vertex_name.empty()) {

            if (!inserted_vertices.contains(target_vertex_name)) {
              Vertex target = graph.add_vertex();
              inserted_vertices[target_vertex_name] = target;
            }

            Edge e =
                graph.add_edge(Vertex{inserted_vertices[source_vertex_name]},
                               Vertex{inserted_vertices[target_vertex_name]});
            edge_properties[e] = properties;
          }
        }
      }
    }
  }
}

} // namespace graph::io::graphviz