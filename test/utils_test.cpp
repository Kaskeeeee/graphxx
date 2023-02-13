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

#include "catch.hpp"
#include "tuple"
#include "tuple_utils.hpp"

namespace utils_test {
using namespace graphxx;
TEST_CASE("Tuple utilities test", "[tuple][utils]") {
  SECTION("get elements from index correctly") {
    std::tuple test{22, false, 3.5f};

    auto elements_from_0 = get_elements_from_index<0>(test);
    auto elements_from_1 = get_elements_from_index<1>(test);
    auto elements_from_2 = get_elements_from_index<2>(test);

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

    set_elements_from_index<1>(test, std::tuple{true, 12.3f});
    REQUIRE(std::get<0>(test) == 22);
    REQUIRE(std::get<1>(test) == true);
    REQUIRE(std::get<2>(test) == 12.3f);
  }
}
} // namespace utils_test
