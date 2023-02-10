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

#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace graphxx::io {

template <concepts::Graph G, concepts::Numeric WeightType>
void mm_serialize(std::ostream &out, const G &graph,
                  std::function<WeightType(Edge<G>)> &get_weight) {

  std::string number_format = "";
  if (std::is_integral_v<WeightType>) {
    number_format = "integer";
  } else if (std::is_floating_point_v<WeightType>) {
    number_format = "real";
  } else {
    throw exceptions::InvariantViolationException(
        "Weight type must be numerical type");
  }

  const std::string header =
      "%%MatrixMarket matrix coordinate " + number_format + " general";

  out << header << std::endl;
  out << graph.num_vertices() << " " << graph.num_vertices() << " "
      << graph.num_edges() << std::endl;

  for (auto vertex : graph) {
    for (auto edge : vertex) {
      out << (graph.get_source(edge) + 1) << " " << (graph.get_target(edge) + 1)
          << " " << get_weight(edge) << std::endl;
    }
  }
}

template <concepts::Graph G>
void mm_serialize(std::ostream &out, const G &graph) {
  const std::string header = "%%MatrixMarket matrix coordinate pattern general";

  out << header << std::endl;
  out << graph.num_vertices() << " " << graph.num_vertices() << " "
      << graph.num_edges() << std::endl;

  for (auto vertex : graph) {
    for (auto edge : vertex) {
      out << (graph.get_source(edge) + 1) << " " << (graph.get_target(edge) + 1)
          << std::endl;
    }
  }
}

template <concepts::Graph G, concepts::Numeric WeightType>
void mm_deserialize(std::istream &in, G &graph) {
  std::string input_string;
  bool symmetric = false;
  bool weighted = false;

  // get header line
  // %%MatrixMarket matrix coordinate number_format symmetries
  std::vector<std::string> header(5);
  std::getline(in, input_string);
  std::stringstream header_string(input_string);
  for (auto &s : header) {
    header_string >> s;
  }

  if (header[0] != "%%MatrixMarket") {
    throw exceptions::BadMatrixMarketParseException();
  }

  // check if graph is weighted
  if (header[3] == "pattern") {
    weighted = false;
  } else {
    weighted = true;
  }

  // check eventual symmetries
  if (header[4] == "general") {
    symmetric = false;
  } else if (header[4] == "symmetric") {
    symmetric = true;
  } else {
    throw exceptions::BadMatrixMarketParseException();
  }

  // skip comment lines
  while (std::getline(in, input_string)) {
    if (input_string[0] != '%') {
      break;
    }
  }

  // rows, columns, entries
  size_t rows, columns, entries;
  std::stringstream(input_string) >> rows >> columns >> entries;

  size_t num_vertices = rows * (symmetric ? 2 : 1);
  for (int64_t i = 0; i < num_vertices; i++) {
    graph.add_vertex(i);
  }

  for (int64_t i = 0; i < entries; i++) {
    DefaultIdType source_id, target_id;
    WeightType weight{1.0};

    std::getline(in, input_string);
    auto string_stream = std::stringstream(input_string);
    string_stream >> source_id >> target_id;
    if (weighted) {
      string_stream >> weight;
      graph.add_edge(source_id - 1, target_id - 1, {weight});
    } else {
      graph.add_edge(source_id - 1, target_id - 1);
    }

    if (symmetric && (source_id != target_id)) {
      if (weighted) {
        graph.add_edge(target_id - 1, source_id - 1, {weight});
      } else {
        graph.add_edge(target_id - 1, source_id - 1);
      }
    }
  }
}

} // namespace graphxx::io