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

#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include <fstream>
#include <functional>

namespace graph::io::graphviz {

using GraphvizProperties = std::unordered_map<std::string, std::string>;

/**
 * @brief Get the correct labels for DOT language based on graph directedness.
 * @tparam D directedness of graph object (directed/undirected)
 */
template <Directedness D> struct GraphvizTraits {
    static std::string name();
    static std::string delimiter();
};

/**
 * @brief Writes a graph object into an output stream in graphviz DOT format
 *        so that users can make use of graphviz to draw a picture with nice
 *        layout.
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 */
template <concepts::Graph G> void serialize(std::ostream &out, const G &graph);

/**
 * @brief Writes a graph object into an output stream in graphviz DOT format
 *        so that users can make use of graphviz to draw a picture with nice
 *        layout. It's possible to assign properties to each vertex in the graph
 *        with `get_vertex_properties` function.
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 * @param[in] get_vertex_properties function that returns a propery map for each
 *                                  vertex
 */
template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphvizProperties(Vertex)> get_vertex_properties);

/**
 * @brief Writes a graph object into an output stream in graphviz DOT format
 *        so that users can make use of graphviz to draw a picture with nice
 *        layout. It's possible to assign properties to each vertex in the graph
 *        with `get_vertex_properties` function. It's possible to assign
 *        properties to each edge in the graph with `get_edge_properties`
 *        function.
 * @tparam G type of input graph
 * @param[out] out output stream
 * @param[in] graph input graph object
 * @param[in] get_vertex_properties function that returns a propery map for each
 *                                  vertex
 * @param[in] get_edge_properties function that returns a propery map for each
 *                                edge
 */
template <concepts::Graph G>
void serialize(std::ostream &out, const G &graph,
               std::function<GraphvizProperties(Vertex)> get_vertex_properties,
               std::function<GraphvizProperties(Edge)> get_edge_properties);

/**
 * @brief Interprets a graph described using the graphviz DOT language and
 *        builds a graph object that captures that description.
 *        You must pass an undirected graph when reading an undirected graph,
 *        the same is true for directed graphs.
 * @tparam G type of output graph
 * @param[in]  in input stream
 * @param[out] graph refrence to output graph
 * @param[out] vertex_properties reference to map in which store the attributes
 *                               of the vertices
 * @param[out] edge_properties reference to map in which store the attributes of
 *                             the edges
 */
template <concepts::Graph G>
void deserialize(std::istream &in, G &graph,
                 std::unordered_map<Id, GraphvizProperties> &vertex_properties,
                 std::unordered_map<Id, GraphvizProperties> &edge_properties);

} // namespace graph::io::graphviz

#include "io/graphviz.i.hpp"