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
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>
#include <functional>

namespace graph::io::graphml {

using GraphMLProperties = std::unordered_map<std::string, std::string>;

const std::string XML_HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
const std::string GRAPHML_ROOT_OPEN =
    "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
    "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
    "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">";

const std::string GRAPHML_ROOT_CLOSE = "</graphml>";

/**
 * @brief  Write a graph object into an output stream in the GraphML format. 
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 */
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

/**
 * @brief Write a graph object into an output stream in the GraphML format. 
 *        It's possible to assign properties to each vertex in the graph
 *        with `get_vertex_properties` function.
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 * @param[in] get_vertex_properties function that returns a propery map for each
 * vertex
 */
template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphMLProperties(Vertex)> get_vertex_properties);

/**
 * @brief Write a graph object into an output stream in the GraphML format. 
 *        It's possible to assign properties to each vertex in the graph
 *        with `get_vertex_properties` function. It's possible to assign
 *        properties to each edge in the graph with `get_edge_properties`
 *        function.
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 * @param[in] get_vertex_properties function that returns a propery map for each
 * vertex
 * @param[in] get_edge_properties function that returns a propery map for each
 * edge
 */
template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphMLProperties(Vertex)> get_vertex_properties,
               std::function<GraphMLProperties(Edge)> get_edge_properties);

template <concepts::Graph G>
void deserialize(
    std::istream &in, G &graph);

} // namespace graph::io::graphml



#include "io/graphml.i.hpp"