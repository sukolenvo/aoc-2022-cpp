//
// Created by object on 8/03/23.
//

#ifndef AOC_2022_CPP_LEVEL9_HPP
#define AOC_2022_CPP_LEVEL9_HPP

#include <array>
#include <cmath>
#include <exception>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "common.hpp"

namespace level9 {

template<auto ropeLength> auto solve(const auto &input)
{
  auto lines = splitLines(input);
  static const auto hashFunction = [](const auto &item) {
    return std::hash<int>()(item.first * ropeLength + item.second);
  };
  std::unordered_set<std::pair<int, int>, decltype(hashFunction)> visited;
  std::array<std::pair<int, int>, ropeLength> rope;
  rope.fill({ 0, 0 });
  for (const auto &line : lines) {
    if (line.empty()) {
      continue;
    }
    auto action = line[0];
    int distance = 0;
    for (auto c = line.begin() + 2; c != line.end(); ++c) {
      distance *= 10;
      distance += *c - '0';
    }
    if (distance < 1 || distance > 100) {
      throw std::invalid_argument("unexpected distance");
    }
    for (int i = 0; i < distance; ++i) {
      if (action == 'R') {
        ++rope[0].first;
      } else if (action == 'L') {
        --rope[0].first;
      } else if (action == 'U') {
        --rope[0].second;
      } else if (action == 'D') {
        ++rope[0].second;
      } else {
        throw std::invalid_argument("unexpected action");
      }
      for (size_t j = 1; j < ropeLength; ++j) {
        auto deltaX = rope.at(j - 1).first - rope.at(j).first;
        auto deltaY = rope.at(j - 1).second - rope.at(j).second;
        if (std::abs(deltaX) <= 1 && std::abs(deltaY) <= 1) {
          continue;
        }
        if (deltaX == 0) {
          rope.at(j).second += deltaY / 2;
        } else if (deltaY == 0) {
          rope.at(j).first += deltaX / 2;
        } else {
          rope.at(j).first += deltaX > 0 ? 1 : -1;
          rope.at(j).second += deltaY > 0 ? 1 : -1;
        }
      }
      visited.insert(rope[ropeLength - 1]);
    }
  }
  return visited.size();
}

void run()
{
  const auto taskInput = readTaskInput(9);
  std::cout << solve<2>(taskInput) << '\n';
  std::cout << solve<10>(taskInput) << '\n';
}

}// namespace level9

#endif// AOC_2022_CPP_LEVEL9_HPP
