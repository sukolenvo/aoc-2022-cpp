//
// Created by object on 31/12/22.
//

#include "common.hpp"

std::vector<std::string_view> splitLines(const std::string_view &input)
{
  std::vector<std::string_view> lines;
  auto start = 0;
  auto index = 1;
  for (const auto &item : input) {
    if (item == '\n') {
      lines.emplace_back(input.begin() + start, input.begin() + index - 1);
      start = index;
    }
    ++index;
  }
  if (start != index - 1) { lines.emplace_back(input.begin() + start, input.begin() + index - 1); }
  return lines;
}