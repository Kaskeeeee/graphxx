#include <string>
#include <vector>
#include <locale>
#include <algorithm>

namespace graph::utils {

size_t find_first_of(const std::string &input,
                     const std::vector<std::string> &find, size_t &index_found,
                     const size_t &pos = 0) {
  size_t first_found = std::string::npos;
  size_t current_found = std::string::npos;

  const size_t size = find.size();
  for (size_t i = 0; i < size; ++i) {
    current_found = input.find(find[i], pos);
    if (current_found < first_found) {
      first_found = current_found;
      index_found = i;
    }
  }

  return first_found;
}

std::string trim_right(const std::string &str) {
  int64_t pos = str.size() - 1;
  for (; pos > 0 && std::isspace(str[pos], std::locale::classic()); --pos) {
  };
  return str.substr(0, pos + 1);
}

std::string trim_left(const std::string &str) {
  size_t pos = 0;
  for (; pos < str.size() && std::isspace(str[pos], std::locale::classic());
       ++pos) {
  };
  return str.substr(pos);
}

std::string trim(const std::string &str) { return trim_left(trim_right(str)); }

std::vector<std::string> split(const std::string &str,
                               const std::string &separator,
                               const size_t offset = 0,
                               const size_t end = std::string::npos) {
  std::vector<std::string> tokens;

  size_t pos = offset;
  size_t next = str.find(separator, offset);

  while (next >= pos) {
    if (next > pos && next <= end)
      tokens.push_back(str.substr(pos, next - pos));

    if (next >= end)
      break;

    pos = next + separator.size();
    next = str.find(separator, pos);
  }

  return tokens;
}

std::vector<std::string> split(const std::string &str,
                               const std::vector<std::string> &separators,
                               const size_t offset = 0,
                               const size_t end = std::string::npos) {
  std::vector<std::string> tokens;

  size_t separator_index = 0;
  size_t pos = offset;
  size_t next = find_first_of(str, separators, separator_index, pos);

  while (next >= pos) {
    if (next > pos && next <= end)
      tokens.push_back(str.substr(pos, next - pos));

    if (next >= end)
      break;

    pos = next + separators[separator_index].size();
    next = find_first_of(str, separators, separator_index, pos);
  }

  return tokens;
}

std::string to_lower(const std::string &str) {
  std::string out = str;
  std::transform(out.begin(), out.end(), out.begin(), ::tolower);
  return out;
}

void to_lower(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}


bool contains(const std::string &str, const std::string &find,
              bool case_sensitive = true) {
  if (case_sensitive)
    return str.find(find, 0) != std::string::npos;
  else
    return to_lower(str).find(to_lower(find), 0) != std::string::npos;
}

bool get_text_between_delimiters(const std::string &input, size_t &start,
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

      if (pos == std::string::npos)
        break;

      if (index == 0) // open tag
      {
        ++count;
      } else if (index == 1) // close tag
      {
        max_close = pos;
        --count;
      }
    }
  }

  start = min_open;
  end = max_close;

  return min_open != std::string::npos && max_close != std::string::npos;
}


std::string get_text_between_delimiters(const std::string &input,
                            const std::string &open_delimiter,
                            const std::string &close_delimiter) {
  size_t start = 0;
  size_t end = 0;

  if (get_text_between_delimiters(input, start, end, open_delimiter,
                                  close_delimiter))
    return input.substr(start + open_delimiter.size(),
                        end - start - open_delimiter.size());

  return std::string();
}

} // namespace graph::utils