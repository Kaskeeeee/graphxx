/**
 * @file
 * 
 * @copyright Copyright © 2022 Graph++. All rights reserved.
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
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea Cinelli
 * @date December, 2022
 * @version v1.0
 *
 */

#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>
#include <functional>

namespace graph::io::graphviz {

/**
 * @brief get the correct labels for dot language based on graph directedness
 * @tparam D directedness of graph object (directed/undirected)
*/
template <Directedness D> struct GraphvizTraits {
  static std::string name();
  static std::string delimiter();
};

/**
 * @brief writes a graph object into an output stream in graphviz DOT format
 *        so that users can make use of graphviz to draw a picture with nice layout 
 * @tparam G type of input graph
 * @param[in] graph input graph object
 * @param[out] out output stream
*/
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

/// @brief 
/// @tparam G 
/// @param out 
/// @param graph 
/// @param label_vertex 
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph, std::function<std::string(Vertex)> label_vertex);

/// @brief 
/// @tparam G 
/// @param out 
/// @param graph 
/// @param label_vertex 
/// @param label_edge 
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph, std::function<std::string(Vertex)> label_vertex, std::function<std::string(Edge)> label_edge);

/**
 * @brief interprets a graph described using the graphviz DOT language and builds 
 *        a graph object that captures that description
 * @tparam G type of input graph
 * @param[in] in input stream
*/
template <concepts::Graph G> G deserialize(std::istream& in);

} // namespace graph::io::graphviz

#include "io/graphviz.i.hpp"