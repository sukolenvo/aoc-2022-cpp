//
// Created by object on 3/12/22.
//

#ifndef AOC_2022_CPP_LEVEL2_HPP
#define AOC_2022_CPP_LEVEL2_HPP

#include <iostream>
#include <string_view>

#include "common.hpp"

namespace level2 {

struct LineInput
{
  int elvePlay;
  int secondInput;
};

int calculatePointsPart1(const LineInput &input)
{
  const int shapeBasedPoints = input.secondInput + 1;
  // we have 3 possible outcomes of `myPlay - elvePlay`:
  // -2, 1 - win
  // 0 - draw
  // -1, 2 - lost
  // adding 6 and mode 3 allows to merge different scenarios into the single outcome (eg -2, 1 => 1)
  const int outcomeBasedPoints = (input.secondInput - input.elvePlay + 6 + 1) % 3
                                 * 3;// NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  return shapeBasedPoints + outcomeBasedPoints;
}

int calculatePointsPart2(const LineInput &input)
{
  const int outcomeBasedPoints = input.secondInput * 3;
  const int shapeBasedPoints = (input.elvePlay + input.secondInput + 2) % 3 + 1;
  return outcomeBasedPoints + shapeBasedPoints;
}

int solve(const std::string_view &input, const auto &points_calculator)
{
  auto lines = splitLines(input);
  int totalScore = 0;
  for (const auto &line : lines) {
    if (line.begin() == line.end()) {
      continue;// skip empty lines
    }
    if (line.size() != 3) {
      throw std::invalid_argument(std::string{ "invalid line. Expecting 3 characters in line: " }.append(line.data()));
    }
    const LineInput lineInput{ *line.begin() - 'A', *(line.begin() + 2) - 'X' };

    totalScore += points_calculator(lineInput);
  }
  return totalScore;
}

void run()
{
  const auto *taskInput = readTaskInput(2);
  std::cout << solve(taskInput, &calculatePointsPart1) << '\n';
  std::cout << solve(taskInput, &calculatePointsPart2) << '\n';
}

}// namespace level2
#endif// AOC_2022_CPP_LEVEL2_HPP