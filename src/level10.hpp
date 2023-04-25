//
// Created by object on 10/03/23.
//

#ifndef AOC_2022_CPP_LEVEL10_HPP
#define AOC_2022_CPP_LEVEL10_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <utility>

#include "common.hpp"

namespace level10
{

auto part1(const auto &input) {
  auto lines = splitLines(input);
  int cycle = 1;
  int regX = 1;
  auto result = 0;
  for (const auto &line : lines) {
    int duration = 0;
    int increment = 0;
    if (line == "noop") {
      duration = 1;
    } else if (line.starts_with("addx ")) {
      duration = 2;
      increment = readNumber(line.begin() + 5, line.end());
    } else {
      throw std::invalid_argument("unexpected command");
    }
    for (int i = 0; i < duration; ++i, ++cycle) {
      if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220) {
        result += cycle * regX;
      }
    }
    regX += increment;
  }
  return result;
}

auto part2(const auto &input) {
  auto lines = splitLines(input);
  auto regX = 1;
  auto cycle = 0;
  std::vector<bool> result;
  for (const auto &line : lines) {
    int duration = 0;
    int increment = 0;
    if (line == "noop") {
      duration = 1;
    } else if (line.starts_with("addx ")) {
      duration = 2;
      increment = readNumber(line.begin() + 5, line.end());
    } else {
      throw std::invalid_argument("unexpected command");
    }
    for (int i = 0; i < duration; ++i, ++cycle) {
      auto x = cycle % 40;
      result.push_back(std::abs(regX - x) <= 1);
    }
    regX += increment;
  }
  return result;
}

void run() {
  const auto taskInput = readTaskInput(10);
  std::cout << part1(taskInput) << '\n';
  const auto result2 = part2(taskInput);
  for (size_t i = 0; i < result2.size(); ++i) {
    if (i % 40 == 0) {
      std::cout << '\n';
    }
    std::cout << (result2.at(i) ? '#' : '.');
  }

}
}

#endif// AOC_2022_CPP_LEVEL10_HPP
