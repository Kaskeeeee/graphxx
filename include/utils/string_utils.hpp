#pragma once

#include <string>
#include <vector>

namespace graph::utils {

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
                     const size_t &pos = 0);

/// @brief  Removes whitespace from the end of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without trailing spaces.
std::string trim_right(const std::string &str);

/// @brief  Removes whitespace from the beginning of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without leading spaces.
std::string trim_left(const std::string &str);

/// @brief  Removes whitespace from both ends of a string and returns a new
/// string, without modifying the original string.
/// @param str Input string.
/// @return New string without leading and trailing spaces.
std::string trim(const std::string &str);

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
                               const size_t end = std::string::npos);

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
                               const size_t end = std::string::npos);

/// @brief Converts a string to lowercase letters and returns a new string,
/// without modifying the original string.
/// @param str Input string.
/// @return Input string converted to lowercase letters.
std::string to_lower(const std::string &str);

/// @brief Converts a string to lowercase letters.
/// @param str String to convert.
void to_lower(std::string &str);

/// @brief Finds whether a string contains a specified string.
/// @param str String to search in.
/// @param find String to search for.
/// @param case_sensitive true if the method should be case sensitive.
/// @return True if a string contains a specified string, otherwise it returns
/// false.
bool contains(const std::string &str, const std::string &find,
              bool case_sensitive = true);

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
                                        const std::string &close_delimiter);

/// @brief Extracts a substring between two delimiters from an input string.
/// @param input Input strint.
/// @param open_delimiter Initial delimiter.
/// @param close_delimiter Final delimiter.
/// @return The substring found, if exists, otherwise an empty string.
inline std::string
get_text_between_delimiters(const std::string &input,
                            const std::string &open_delimiter,
                            const std::string &close_delimiter);

} // namespace graph::utils