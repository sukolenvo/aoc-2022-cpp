//
// Created by object on 3/12/22.
//

#ifndef AOC_2022_CPP_COMMON_HPP
#define AOC_2022_CPP_COMMON_HPP

#include <string_view>
#include <vector>

inline std::vector<std::string_view> splitLines(const std::string_view &input)
{
  std::vector<std::string_view> lines;
  auto start = 0;
  auto index = 1;
  for (const auto &item : input) {
    if (item == '\n') {
      if (start != index - 1) {
        lines.emplace_back(input.begin() + start, input.begin() + index - 1);
      }
      start = index;
    }
    ++index;
  }
  if (start != index - 1) { lines.emplace_back(input.begin() + start, input.begin() + index - 1); }
  return lines;
}

#endif// AOC_2022_CPP_COMMON_HPP