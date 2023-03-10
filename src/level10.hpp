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

static const auto taskInput = R"(addx 1
noop
noop
noop
addx 5
addx 5
noop
noop
addx 9
addx -5
addx 1
addx 4
noop
noop
noop
addx 6
addx -1
noop
addx 5
addx -2
addx 7
noop
addx 3
addx -2
addx -38
noop
noop
addx 32
addx -22
noop
addx 2
addx 3
noop
addx 2
addx -2
addx 7
addx -2
noop
addx 3
addx 2
addx 5
addx 2
addx -5
addx 10
noop
addx 3
noop
addx -38
addx 1
addx 27
noop
addx -20
noop
addx 2
addx 27
noop
addx -22
noop
noop
noop
noop
addx 3
addx 5
addx 2
addx -11
addx 16
addx -2
addx -17
addx 24
noop
noop
addx 1
addx -38
addx 15
addx 10
addx -15
noop
addx 2
addx 26
noop
addx -21
addx 19
addx -33
addx 19
noop
addx -6
addx 9
addx 3
addx 4
addx -21
addx 4
addx 20
noop
addx 3
addx -38
addx 28
addx -21
addx 9
addx -8
addx 2
addx 5
addx 2
addx -9
addx 14
addx -2
addx -5
addx 12
addx 3
addx -2
addx 2
addx 7
noop
noop
addx -27
addx 28
addx -36
noop
addx 1
addx 5
addx -1
noop
addx 6
addx -1
addx 5
addx 5
noop
noop
addx -2
addx 20
addx -10
addx -3
addx 1
addx 3
addx 2
addx 4
addx 3
noop
addx -30
noop)";

void run() {
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
