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
#include "utils.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace graphxx::io::matrix_market {

template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType = DecaySubscriptValue<DefaultIdType, C>>
void serialize(std::ostream &out, const G &graph, C &weights) {
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

  size_t num_vertices, num_edges;
  for (auto _ : graph.vertices()) {
    num_vertices++;
  }
  for (auto _ : graph.edges()) {
    num_edges++;
  }

  out << header << std::endl;
  out << num_vertices << " " << num_vertices << " " << num_edges << std::endl;

  for (auto edge : graph.edges()) {
    out << (edge.u + 1) << " " << (edge.v + 1) << " " << weights[edge]
        << std::endl;
  }
}

template <concepts::Graph G> void serialize(std::ostream &out, const G &graph) {
  const std::string header = "%%MatrixMarket matrix coordinate pattern general";

  size_t num_vertices, num_edges;
  for (auto _ : graph.vertices()) {
    num_vertices++;
  }
  for (auto _ : graph.edges()) {
    num_edges++;
  }

  out << header << std::endl;
  out << num_vertices << " " << num_vertices << " " << num_edges << std::endl;

  for (auto edge : graph.edges()) {
    out << (edge.u + 1) << " " << (edge.v + 1) << std::endl;
  }
}

template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType = DecaySubscriptValue<DefaultIdType, C>>
void deserialize(std::istream &in, G &graph, C &weights) {
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
    graph.add_vertex();
  }

  for (int64_t i = 0; i < entries; i++) {
    DefaultIdType source_id, target_id;
    WeightType weight{1.0};

    std::getline(in, input_string);
    if (weighted) {
      std::stringstream(input_string) >> source_id >> target_id >> weight;
    } else {
      std::stringstream(input_string) >> source_id >> target_id;
    }

    Edge e1 = graph.add_edge(Vertex{source_id - 1}, Vertex{target_id - 1});
    if (weighted) {
      weights[e1] = weight;
    }

    if (symmetric && (source_id != target_id)) {
      Edge e2 = graph.add_edge(Vertex{target_id - 1}, Vertex{source_id - 1});
      if (weighted) {
        weights[e2] = weight;
      }
    }
  }
}

} // namespace graphxx::io::matrix_market
