/**
 * @file This file contains a program that generates a random graph and prints
 * it in the Graphviz format
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "generators/graph_generator.hpp"
#include "io/graphviz.hpp"
#include "list_graph.hpp"

#include <iostream>

using namespace graphxx;
using namespace graphxx::io;

int main() {
  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  GraphGenerator generator;

  // Generate a random graph with 10 vertices and 15 edges
  generator.generate_random_graph(g, 10, 15);

  // Print the graph on std::cout
  graphviz_serialize(std::cout, g);

  return 0;
}