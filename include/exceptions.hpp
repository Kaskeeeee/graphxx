/**
 * @file This file contains definitions of custom exceptions
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

#pragma once

#include <exception> // std::exception
#include <string>    // std::string

namespace graphxx::exceptions {

/// @brief Generic graph exceptions from which the other ones inherits
/// its extends std::exception by adding a custom message
struct GraphException : public std::exception {
  explicit GraphException(std::string message)
      : _message{std::move(message)} {};

  [[nodiscard]] const char *what() const noexcept override {
    return _message.c_str();
  };

protected:
  std::string _message;
};

/// @brief Exception thrown when trying to do some operations on vertices that
/// don't exist
struct NoSuchVertexException : GraphException {
  NoSuchVertexException()
      : GraphException("Vertex is missing from graph exception"){};
};

/// @brief Exception thrown when trying to do some operations on edges that
/// don't exist
struct NoSuchEdgeException : GraphException {
  NoSuchEdgeException()
      : GraphException("Edge is missing from graph exeception"){};
};

/// @brief Exception thrown when an invariant is violated
struct InvariantViolationException : GraphException {
  explicit InvariantViolationException(const std::string &message)
      : GraphException("Invariant violation exception: " + message){};
};

/// @brief Exception thrown when trying to parse a directed graph file into an
/// undirected graph
struct DirectedGraphParseException : GraphException {
  DirectedGraphParseException()
      : GraphException(
            "Tried to read a directed graph into an undirected graph"){};
};

/// @brief Exception thrown when trying to parse an undirected graph file into a
/// directed graph
struct UndirectedGraphParseException : GraphException {
  UndirectedGraphParseException()
      : GraphException(
            "Tried to read an undirected graph into a directed graph"){};
};

/// @brief Exception thrown when trying to parse a file that is not in the
/// correct graphviz format
struct BadGraphvizParseException : GraphException {
  BadGraphvizParseException() : GraphException("Bad graphviz file syntax"){};
};

/// @brief Exception thrown when trying to parse a file that is not in the
/// correct graphml format
struct BadGraphmlParseException : GraphException {
  BadGraphmlParseException() : GraphException("Bad graphml file syntax"){};
};

/// @brief Exception thrown when trying to parse a file that is not in the
/// correct matrix market format
struct BadMatrixMarketParseException : GraphException {
  BadMatrixMarketParseException()
      : GraphException("Bad matrix market file syntax"){};
};
} // namespace graphxx::exceptions