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

#include <string>
#include <vector>

namespace graphxx::utils {

/// @brief  Searches the input string for the first occurring word among those
/// specified as an argument.
/// @param input String to search in.
/// @param find List of strings to search for.
/// @param index Input list index of the first occurring string.
/// @param pos Index of character to search from (default 0).
/// @return The position of the first occurring string. If no matches are found,
/// the function returns string::npos.
size_t find_first_of(const std::string &input,
                     const std::vector<std::string> &find, size_t &index_found,
                     const size_t &pos = 0) {
  size_t first_found = std::string::npos;
  size_t current_found = std::string::npos;

  const size_t size = find.size();
  for (int64_t i = 0; i < size; ++i) {
    current_found = input.find(find[i], pos);
    if (current_found < first_found) {
      first_found = current_found;
      index_found = i;
    }
  }

  return first_found;
}

/// @brief  Removes whitespace from the end of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without trailing spaces.
std::string trim_right(const std::string &str) {
  int64_t pos = str.size() - 1;
  for (; pos > 0 && std::isspace(str[pos], std::locale::classic()); --pos) {
  };
  return str.substr(0, pos + 1);
}

/// @brief  Removes whitespace from the beginning of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without leading spaces.
std::string trim_left(const std::string &str) {
  size_t pos = 0;
  for (; pos < str.size() && std::isspace(str[pos], std::locale::classic());
       ++pos) {
  };
  return str.substr(pos);
}

/// @brief  Removes whitespace from both ends of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without leading and trailing spaces.
std::string trim(const std::string &str) { return trim_left(trim_right(str)); }

/// @brief Divides a string into a list of substrings by searching for the
/// separator.
/// @param str String to divide.
/// @param separator The pattern describing where each split should occur.
/// @param offset Index of character to search from (default 0).
/// @param end Index of the character to search within.
/// @return A list of strings, split at each point where the separator occurs in
/// the given string.
std::vector<std::string> split(const std::string &str,
                               const std::string &separator,
                               const size_t offset = 0,
                               const size_t end = std::string::npos) {
  std::vector<std::string> tokens;

  size_t pos = offset;
  size_t next = str.find(separator, offset);

  while (next >= pos) {
    if (next > pos && next <= end) {
      tokens.push_back(str.substr(pos, next - pos));
    }

    if (next >= end) {
      break;
    }

    pos = next + separator.size();
    next = str.find(separator, pos);
  }

  return tokens;
}

/// @brief Divides a string into a list of substrings by searching for the
/// separators.
/// @param str String to divide.
/// @param separators List of strings describing where each split should occur.
/// @param offset Index of character to search from (default 0).
/// @param end Index of the character to search within.
/// @return A list of strings, split at each point where a separator occurs in
/// the given string.
std::vector<std::string> split(const std::string &str,
                               const std::vector<std::string> &separators,
                               const size_t offset = 0,
                               const size_t end = std::string::npos) {
  std::vector<std::string> tokens;

  size_t separator_index = 0;
  size_t pos = offset;
  size_t next = find_first_of(str, separators, separator_index, pos);

  while (next >= pos) {
    if (next > pos && next <= end) {
      tokens.push_back(str.substr(pos, next - pos));
    }

    if (next >= end) {
      break;
    }

    pos = next + separators[separator_index].size();
    next = find_first_of(str, separators, separator_index, pos);
  }

  return tokens;
}

/// @brief Converts a string to lowercase letters and returns a new string,
/// without modifying the original string.
/// @param str Input string.
/// @return Input string converted to lowercase letters.
std::string to_lower(const std::string &str) {
  std::string out = str;
  std::transform(out.begin(), out.end(), out.begin(), ::tolower);
  return out;
}

/// @brief Converts a string to lowercase letters.
/// @param str String to convert.
void to_lower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

/// @brief Finds whether a string contains a specified string.
/// @param str String to search in.
/// @param find String to search for.
/// @param case_sensitive true if the method should be case sensitive.
/// @return True if a string contains a specified string, otherwise it returns
/// false.
bool contains(const std::string &str, const std::string &find,
              bool case_sensitive = true) {
  if (case_sensitive) {
    return str.find(find, 0) != std::string::npos;
  } else {
    return to_lower(str).find(to_lower(find), 0) != std::string::npos;
  }
}

/// @brief Search for a substring between two delimiters from an input string.
/// @param input Input string.
/// @param start Index of the first character of the substring.
/// @param end Index of the last character of the substring.
/// @param open_delimiter Initial delimiter.
/// @param close_delimiter Final delimiter.
/// @return True if there is a substring between the delimiters.
inline bool get_text_between_delimiters(const std::string &input, size_t &start,
                                        size_t &end,
                                        const std::string &open_delimiter,
                                        const std::string &close_delimiter) {
  const size_t min_open = input.find(open_delimiter, start);

  if (min_open == std::string::npos) {
    start = std::string::npos;
    end = std::string::npos;
    return false;
  }

  size_t max_close = min_open;

  if (open_delimiter == close_delimiter) {
    max_close = input.find(close_delimiter, min_open + close_delimiter.size());
  } else {
    const std::vector<std::string> delimiters = {open_delimiter,
                                                 close_delimiter};

    size_t count = 1;
    size_t pos = min_open;
    size_t index = 0;

    while (count > 0 && pos != std::string::npos) {
      pos = find_first_of(input, delimiters, index, pos + 1);

      if (pos == std::string::npos) {
        break;
      }

      if (index == 0) { // open tag
        ++count;
      } else if (index == 1) { // close tag
        max_close = pos;
        --count;
      }
    }
  }

  start = min_open;
  end = max_close;

  return min_open != std::string::npos && max_close != std::string::npos;
}

/// @brief Extracts a substring between two delimiters from an input string.
/// @param input Input strint.
/// @param open_delimiter Initial delimiter.
/// @param close_delimiter Final delimiter.
/// @return The substring found, if exists, otherwise an empty string.
inline std::string
get_text_between_delimiters(const std::string &input,
                            const std::string &open_delimiter,
                            const std::string &close_delimiter) {
  size_t start = 0;
  size_t end = 0;

  if (get_text_between_delimiters(input, start, end, open_delimiter,
                                  close_delimiter)) {
    return input.substr(start + open_delimiter.size(),
                        end - start - open_delimiter.size());
  }

  return std::string();
}

} // namespace graphxx::utils