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

#include <exception>
#include <string>

namespace graph::exceptions {

/// @brief Generic graph exceptions from which the other ones inherits
/// its extends std::exception by adding a custom message
struct GraphException : public std::exception {
  explicit GraphException(std::string message);

  [[nodiscard]] const char *what() const noexcept override;

protected:
  std::string _message;
};

/// @brief Exception thrown when all ids are used
struct OutOfIdsException : GraphException {
  OutOfIdsException();
};

/// @brief Exception thrown when trying to do some operations on vertices that
/// don't exist
struct NoSuchVertexException : GraphException {
  NoSuchVertexException();
};

/// @brief Exception thrown when trying to do some operations on edges that
/// don't exist
struct NoSuchEdgeException : GraphException {
  NoSuchEdgeException();
};

/// @brief Exception thrown when an invariant is violated
struct InvariantViolationException : GraphException {
  explicit InvariantViolationException(const std::string &message);
};

struct DirectedGraphParseException : GraphException {
  DirectedGraphParseException();
};

struct UndirectedGraphParseException : GraphException {
  UndirectedGraphParseException();
};

struct BadGraphvizParseException : GraphException {
  BadGraphvizParseException();
};

struct BadGraphmlParseException : GraphException {
  BadGraphmlParseException();
};

struct BadMatrixMarketParseException : GraphException {
  BadMatrixMarketParseException();
};

} // namespace graph::exceptions