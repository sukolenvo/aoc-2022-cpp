//
// Created by object on 3/12/22.
//

#ifndef AOC_2022_CPP_COMMON_HPP
#define AOC_2022_CPP_COMMON_HPP

#include <array>
#include <cstdio>
#include <exception>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>

inline std::vector<std::string_view> splitLines(const std::string_view &input)
{
  std::vector<std::string_view> lines;
  size_t start = 0;
  while (start < input.size()) {
    auto sepratorIndex = input.find('\n', start);
    if (sepratorIndex == std::string_view::npos) {
      if (start < input.size()) {
        lines.emplace_back(input.begin() + static_cast<std::string_view::difference_type>(start), input.end());
      }
      break;
    }
    lines.emplace_back(input.begin() + static_cast<std::string_view::difference_type>(start),
      input.begin() + static_cast<std::string_view::difference_type>(sepratorIndex));
    start = sepratorIndex + 1;
  }
  return lines;
}

inline auto readNumber(auto start, auto end)
{
  auto value = 0;
  auto negative = false;
  if (*start == '-') {
    negative = true;
    ++start;
  }
  while (start != end) {
    value *= 10;
    value += *start++ - '0';
  }
  return negative ? value * -1 : value;
}

inline auto readFile(auto filename)
{
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::stringstream buffer;
    buffer << stream.rdbuf();
    stream.close();
    return buffer.str();
  } else {
    throw std::runtime_error("Failed to open task input file");
  }
}

inline auto readTaskInput(auto level)
{
  std::array<char, 100> filename{};
  filename.fill(0);
  auto status = std::snprintf(filename.data(), filename.max_size(), "input/task%d.txt", level);
  if (status < 0 || static_cast<size_t>(status) >= filename.max_size()) {
    throw std::runtime_error("Failed to construct task input filename");
  }
  return readFile(filename.data());
}

#endif// AOC_2022_CPP_COMMON_HPP