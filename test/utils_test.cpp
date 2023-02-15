/**
 * @file This file contains the unit tests for the utility functions
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

#include "build_path.hpp"
#include "catch.hpp"
#include "exceptions.hpp"
#include "string_utils.hpp"
#include "tuple"
#include "tuple_utils.hpp"

namespace utils_test {
using namespace graphxx;
TEST_CASE("Tuple utilities test", "[tuple][utils]") {
  SECTION("get elements from index correctly") {
    std::tuple test{22, false, 3.5f};

    auto elements_from_0 = utils::get_elements_from_index<0>(test);
    auto elements_from_1 = utils::get_elements_from_index<1>(test);
    auto elements_from_2 = utils::get_elements_from_index<2>(test);

    REQUIRE(std::get<0>(elements_from_0) == std::get<0>(test));
    REQUIRE(std::get<1>(elements_from_0) == std::get<1>(test));
    REQUIRE(std::get<2>(elements_from_0) == std::get<2>(test));

    REQUIRE(std::get<0>(elements_from_1) == std::get<1>(test));
    REQUIRE(std::get<1>(elements_from_1) == std::get<2>(test));

    REQUIRE(std::get<0>(elements_from_2) == std::get<2>(test));
  }

  SECTION("set elements from index") {
    std::tuple test{22, false, 3.5f};
    std::tuple elements{true, 14.2f};

    utils::set_elements_from_index<1>(test, std::tuple{true, 12.3f});
    REQUIRE(std::get<0>(test) == 22);
    REQUIRE(std::get<1>(test) == true);
    REQUIRE(std::get<2>(test) == 12.3f);
  }
}

TEST_CASE("Exceptions test", "[exceptions]") {
  try {
    throw exceptions::GraphException("exceptions test");
  } catch (const exceptions::GraphException &e) {
    REQUIRE(std::string(e.what()) == "exceptions test");
  }
}

TEST_CASE("String utils", "[string_utils]") {
  SECTION("no string between delimiters") {
    REQUIRE(graphxx::utils::get_text_between_delimiters("1abcdef", "1", "0") ==
            "");
  }

  SECTION("ignores second open delimiter") {
    REQUIRE(graphxx::utils::get_text_between_delimiters("{{abcdef}", "{",
                                                        "}") == "{abcdef");
  }
}

TEST_CASE("build correct path", "[build_path]") {
  SECTION("finds correct path") {
    struct node {
      unsigned int parent;
    };

    std::vector<node> v{node{.parent = 0u}, node{.parent = 0u},  // 0, 1
                        node{.parent = 2u}, node{.parent = 4u},  // 2, 3
                        node{.parent = 2u}, node{.parent = 3u}}; // 4, 5
    std::vector<node> res = algorithms::build_path(v, 2u, 5u);
    REQUIRE(res[0].parent == 2);
    REQUIRE(res[1].parent == 2);
    REQUIRE(res[2].parent == 4);
    REQUIRE(res[3].parent == 3);
  }
}
} // namespace utils_test
