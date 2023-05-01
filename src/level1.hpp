//
// Created by object on 3/12/22.
//

#ifndef AOC_2022_CPP_LEVEL1_HPP
#define AOC_2022_CPP_LEVEL1_HPP

#include <array>
#include <charconv>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "common.hpp"

namespace level1 {

void saveDwarf(auto &topDwarfs, int food)
{
  if (food < topDwarfs.at(topDwarfs.size() - 1)) {
    return;
  }
  for (size_t i = 1; i < topDwarfs.size(); ++i) {
    if (food > topDwarfs.at(topDwarfs.size() - 1 - i)) {
      continue;
    }
    for (size_t j = 1; j < i; ++j) {
      topDwarfs.at(topDwarfs.size() - j) = topDwarfs.at(topDwarfs.size() - j - 1);
    }
    topDwarfs.at(topDwarfs.size() - i) = food;
    return;
  }
  for (size_t j = 1; j < topDwarfs.size(); ++j) {
    topDwarfs.at(topDwarfs.size() - j) = topDwarfs.at(topDwarfs.size() - j - 1);
  }
  topDwarfs[0] = food;
}

template<auto N> int solve(const std::string_view &input)
{
  std::array<int, N> topDwarfs{};
  topDwarfs.fill(0);
  int current = 0;
  for (const auto &line : splitLines(input)) {
    if (line.begin() == line.end()) {
      saveDwarf(topDwarfs, current);
      current = 0;
      continue;
    }
    int value{};
    auto result = std::from_chars(line.data(), line.data() + line.size(), value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument(line.data());
    }
    current += value;
  }
  if (current > 0) {
    saveDwarf(topDwarfs, current);
  }
  return std::accumulate(topDwarfs.begin(), topDwarfs.end(), 0);
}

void run()
{
  const auto taskInput = readTaskInput(1);
  std::cout << solve<1>(taskInput) << '\n';
  std::cout << solve<3>(taskInput) << '\n';
}

}// namespace level1
#endif// AOC_2022_CPP_LEVEL1_HPP