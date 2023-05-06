//
// Created by object on 21/01/23.
//

#ifndef AOC_2022_CPP_LEVEL4_HPP
#define AOC_2022_CPP_LEVEL4_HPP

#include <charconv>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "common.hpp"

namespace level4 {

auto parsePairs(const std::string_view &input)
{
  std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> pairs;
  const auto lines = splitLines(input);
  const auto parsePair = [](auto line, auto beginIndex, auto endIndex) {
    auto separator = line.find('-', static_cast<size_t>(beginIndex));
    if (separator == std::string_view::npos) {
      throw std::invalid_argument("can't find separator");
    }
    int rangeStart{};
    auto status = std::from_chars(line.data() + beginIndex, line.data() + separator, rangeStart);
    if (status.ec != std::errc{}) {
      throw std::invalid_argument(line.data());
    }
    int rangeEnd{};
    status = std::from_chars(line.data() + separator + 1, line.data() + endIndex, rangeEnd);
    if (status.ec != std::errc{}) {
      throw std::invalid_argument(line.data());
    }
    return std::pair<int, int>{ rangeStart, rangeEnd };
  };
  for (const auto &line : lines) {
    const auto comma = line.find(',');
    if (comma == std::string_view::npos) {
      throw std::invalid_argument("can't find comma");
    }
    pairs.emplace_back(parsePair(line, 0, comma), parsePair(line, comma + 1, line.length()));
  }
  return pairs;
}

auto part1(const std::string_view &input)
{
  auto pairs = parsePairs(input);
  int overlaps = 0;
  for (const auto &[left, right] : pairs) {
    if (left.first <= right.first && left.second >= right.second) {
      ++overlaps;
    } else if (right.first <= left.first && right.second >= left.second) {
      ++overlaps;
    }
  }
  return overlaps;
}

auto part2(const std::string_view &input)
{
  auto pairs = parsePairs(input);
  int overlaps = 0;
  for (const auto &[left, right] : pairs) {
    if (left.first > right.second || left.second < right.first) {
      continue;
    }
    ++overlaps;
  }
  return overlaps;
}

void run()
{
  const auto taskInput = readTaskInput(4);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
} // namespace level4

#endif // AOC_2022_CPP_LEVEL4_HPP
