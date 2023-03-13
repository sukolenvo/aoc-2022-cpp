//
// Created by object on 29/01/23.
//

#ifndef AOC_2022_CPP_LEVEL5_HPP
#define AOC_2022_CPP_LEVEL5_HPP

#include <algorithm>
#include <exception>
#include <stack>
#include <iostream>
#include <iterator>

#include "common.hpp"

namespace level5
{

struct Task
{
  std::vector<std::vector<char>> stacks;
  std::vector<std::array<unsigned int, 3>> operations;
};

auto parseInput(const auto &input)
{
  auto lines = splitLines(input);
  auto iterator = std::find_if(lines.begin(), lines.end(), [](auto element) { return element.size() == 0; });
  if (iterator == std::end(lines)) {
    throw std::invalid_argument("can't find divider line");
  }
  const auto dividerIndex = static_cast<size_t>(std::distance(lines.begin(), iterator));
  const auto &indexLine = lines.at(dividerIndex - 1);
  Task task;
  int expectedIndex = 1;
  for (size_t i = 0; i < indexLine.size(); ++i) {
    if (indexLine[i] != ' ') {
      if (expectedIndex + '0' == indexLine[i]) {
        auto &stack = task.stacks.emplace_back();
        for (size_t j = 0; j <= dividerIndex - 2; ++j) {
          auto &taskLevel = lines.at(dividerIndex - 2 - j);
          if (taskLevel.size() > i && taskLevel[i] != ' ') {
            stack.push_back(taskLevel[i]);
          }
        }
        ++expectedIndex;
      } else {
        throw std::invalid_argument("unexpected block index");
      }
    }
  }
  const auto readDigits = [](auto &start) {
    unsigned int result = 0;
    char next = *start;
    while (next >= '0' && next <= '9') {
      result *= 10;
      result += static_cast<unsigned int>(next - '0');
      next = *++start;
    }
    return result;
  };
  for (auto it = lines.begin() + static_cast<long>(dividerIndex) + 1; it != lines.end(); ++it) {
    auto line = it->begin();
    task.operations.push_back(std::array<unsigned int, 3>{
      readDigits(line = line + sizeof("move ") - 1),
      readDigits(line = line + sizeof(" from ") - 1),
      readDigits(line = line + sizeof(" to ") - 1)
    });
  }
  return task;
}


auto runCrane9000(const auto &input) {
  auto task = parseInput(input);
  for (const auto &operation : task.operations) {
    for (size_t i = 0; i < operation[0]; ++i) {
      auto &fromStack = task.stacks[operation[1] - 1];
      auto &toStack = task.stacks[operation[2] - 1];
      toStack.push_back(fromStack.back());
      fromStack.pop_back();
    }
  }
  std::vector<char> result;
  std::transform(task.stacks.cbegin(), task.stacks.cend(), std::back_inserter(result), [](const auto &stack) {
    return stack.back();
  });
  return result;
}

auto runCrane9001(const auto &input) {
  auto task = parseInput(input);
  for (const auto &operation : task.operations) {
    auto &fromStack = task.stacks[operation[1] - 1];
    auto &toStack = task.stacks[operation[2] - 1];
    std::copy(fromStack.end() - operation[0], fromStack.end(), std::back_inserter(toStack));
    for (size_t i = 0; i < operation[0]; ++i) {
      fromStack.pop_back();
    }
  }
  std::vector<char> result;
  std::transform(task.stacks.cbegin(), task.stacks.cend(), std::back_inserter(result), [](const auto &stack) {
    return stack.back();
  });
  return result;
}

static const auto taskInput = R"(    [H]         [H]         [V]
    [V]         [V] [J]     [F] [F]
    [S] [L]     [M] [B]     [L] [J]
    [C] [N] [B] [W] [D]     [D] [M]
[G] [L] [M] [S] [S] [C]     [T] [V]
[P] [B] [B] [P] [Q] [S] [L] [H] [B]
[N] [J] [D] [V] [C] [Q] [Q] [M] [P]
[R] [T] [T] [R] [G] [W] [F] [W] [L]
 1   2   3   4   5   6   7   8   9

move 3 from 3 to 7
move 4 from 1 to 9
move 5 from 6 to 3
move 6 from 9 to 8
move 2 from 9 to 5
move 4 from 3 to 7
move 1 from 3 to 6
move 3 from 5 to 7
move 1 from 2 to 1
move 4 from 7 to 8
move 17 from 8 to 7
move 1 from 6 to 2
move 2 from 6 to 7
move 1 from 8 to 3
move 2 from 4 to 6
move 3 from 9 to 6
move 5 from 6 to 3
move 5 from 2 to 1
move 9 from 3 to 4
move 20 from 7 to 3
move 7 from 5 to 3
move 6 from 3 to 5
move 4 from 1 to 9
move 4 from 5 to 6
move 2 from 1 to 8
move 2 from 7 to 3
move 3 from 6 to 3
move 2 from 5 to 8
move 2 from 9 to 3
move 1 from 9 to 6
move 2 from 2 to 4
move 26 from 3 to 4
move 28 from 4 to 6
move 8 from 6 to 1
move 4 from 8 to 6
move 1 from 9 to 3
move 2 from 3 to 6
move 1 from 3 to 9
move 26 from 6 to 9
move 2 from 7 to 3
move 5 from 1 to 4
move 1 from 1 to 4
move 1 from 6 to 5
move 1 from 2 to 5
move 2 from 3 to 7
move 2 from 5 to 8
move 10 from 4 to 5
move 1 from 6 to 1
move 1 from 8 to 5
move 2 from 7 to 2
move 4 from 4 to 3
move 4 from 7 to 2
move 2 from 1 to 8
move 12 from 9 to 2
move 5 from 2 to 3
move 3 from 3 to 1
move 1 from 1 to 7
move 6 from 3 to 8
move 1 from 5 to 3
move 10 from 9 to 1
move 7 from 8 to 7
move 1 from 3 to 9
move 7 from 7 to 2
move 3 from 2 to 9
move 6 from 2 to 9
move 5 from 9 to 1
move 7 from 2 to 1
move 21 from 1 to 7
move 2 from 1 to 2
move 5 from 2 to 3
move 2 from 4 to 3
move 10 from 5 to 4
move 11 from 4 to 7
move 5 from 3 to 1
move 1 from 4 to 2
move 2 from 8 to 3
move 7 from 9 to 3
move 3 from 9 to 1
move 9 from 7 to 9
move 1 from 3 to 4
move 3 from 9 to 4
move 5 from 9 to 3
move 4 from 3 to 8
move 22 from 7 to 8
move 10 from 3 to 5
move 1 from 9 to 4
move 8 from 1 to 5
move 3 from 4 to 9
move 1 from 3 to 6
move 2 from 1 to 7
move 1 from 4 to 3
move 1 from 4 to 7
move 1 from 2 to 1
move 1 from 6 to 9
move 1 from 3 to 7
move 1 from 1 to 7
move 4 from 9 to 3
move 22 from 8 to 5
move 37 from 5 to 9
move 37 from 9 to 6
move 3 from 7 to 9
move 2 from 8 to 6
move 1 from 9 to 3
move 2 from 5 to 1
move 1 from 2 to 5
move 7 from 6 to 4
move 18 from 6 to 2
move 1 from 3 to 7
move 1 from 5 to 4
move 1 from 8 to 5
move 9 from 2 to 5
move 3 from 4 to 6
move 2 from 2 to 7
move 5 from 2 to 3
move 1 from 8 to 1
move 1 from 9 to 4
move 2 from 7 to 8
move 7 from 3 to 7
move 3 from 1 to 3
move 1 from 9 to 5
move 17 from 6 to 2
move 12 from 7 to 9
move 1 from 4 to 8
move 1 from 8 to 4
move 4 from 5 to 2
move 2 from 8 to 9
move 3 from 4 to 2
move 3 from 3 to 7
move 2 from 4 to 3
move 8 from 9 to 1
move 1 from 4 to 2
move 24 from 2 to 1
move 6 from 5 to 1
move 1 from 7 to 4
move 3 from 2 to 8
move 3 from 3 to 7
move 1 from 4 to 6
move 2 from 8 to 5
move 3 from 9 to 4
move 1 from 5 to 3
move 1 from 3 to 5
move 3 from 9 to 8
move 1 from 5 to 7
move 5 from 7 to 9
move 2 from 8 to 4
move 1 from 3 to 2
move 1 from 7 to 3
move 1 from 8 to 5
move 1 from 2 to 9
move 1 from 6 to 2
move 2 from 9 to 8
move 1 from 3 to 7
move 24 from 1 to 3
move 1 from 7 to 6
move 3 from 5 to 1
move 1 from 4 to 3
move 1 from 8 to 6
move 1 from 6 to 4
move 1 from 5 to 4
move 1 from 8 to 5
move 1 from 5 to 7
move 1 from 2 to 5
move 1 from 6 to 3
move 1 from 4 to 9
move 1 from 5 to 7
move 2 from 9 to 2
move 3 from 4 to 8
move 2 from 4 to 3
move 11 from 1 to 9
move 7 from 9 to 1
move 9 from 1 to 9
move 1 from 3 to 7
move 3 from 7 to 4
move 2 from 2 to 6
move 2 from 4 to 1
move 1 from 6 to 7
move 22 from 3 to 7
move 1 from 3 to 5
move 1 from 5 to 2
move 1 from 6 to 7
move 5 from 1 to 9
move 1 from 8 to 5
move 1 from 2 to 1
move 15 from 9 to 4
move 6 from 9 to 6
move 14 from 4 to 1
move 5 from 6 to 2
move 1 from 5 to 1
move 9 from 1 to 4
move 5 from 1 to 3
move 3 from 2 to 6
move 2 from 8 to 1
move 5 from 1 to 9
move 10 from 7 to 8
move 3 from 3 to 8
move 2 from 8 to 7
move 5 from 4 to 9
move 3 from 3 to 5
move 1 from 6 to 9
move 1 from 3 to 9
move 1 from 3 to 6
move 1 from 3 to 7
move 2 from 6 to 9
move 2 from 4 to 1
move 13 from 9 to 8
move 2 from 1 to 4
move 6 from 4 to 9
move 1 from 6 to 2
move 1 from 2 to 3
move 3 from 5 to 3
move 4 from 3 to 2
move 7 from 9 to 2
move 1 from 6 to 4
move 4 from 2 to 5
move 2 from 2 to 1
move 4 from 5 to 8
move 1 from 4 to 2
move 6 from 2 to 1
move 2 from 2 to 1
move 22 from 8 to 2
move 16 from 7 to 4
move 14 from 2 to 7
move 7 from 8 to 2
move 4 from 7 to 1
move 14 from 2 to 1
move 10 from 7 to 1
move 1 from 7 to 3
move 1 from 3 to 4
move 1 from 2 to 5
move 25 from 1 to 5
move 1 from 5 to 3
move 4 from 4 to 2
move 13 from 4 to 6
move 4 from 2 to 1
move 3 from 6 to 2
move 9 from 1 to 2
move 22 from 5 to 4
move 1 from 2 to 7
move 8 from 1 to 5
move 1 from 4 to 5
move 15 from 4 to 3
move 11 from 2 to 1
move 1 from 7 to 3
move 2 from 5 to 1
move 13 from 3 to 5
move 10 from 6 to 7
move 1 from 3 to 4
move 1 from 3 to 6
move 1 from 3 to 9
move 1 from 9 to 5
move 1 from 6 to 2
move 6 from 4 to 9
move 1 from 3 to 7
move 1 from 5 to 1
move 3 from 5 to 6
move 1 from 4 to 3
move 12 from 5 to 6
move 1 from 2 to 8
move 4 from 1 to 7
move 1 from 3 to 2
move 1 from 2 to 6
move 9 from 6 to 4
move 1 from 8 to 7
move 3 from 1 to 2
move 2 from 2 to 5
move 5 from 4 to 6
move 1 from 4 to 6
move 6 from 7 to 3
move 6 from 5 to 7
move 12 from 7 to 4
move 1 from 2 to 8
move 6 from 9 to 5
move 1 from 8 to 9
move 1 from 3 to 6
move 4 from 4 to 1
move 1 from 7 to 9
move 4 from 4 to 6
move 2 from 9 to 7
move 7 from 5 to 1
move 3 from 1 to 4
move 4 from 3 to 1
move 10 from 6 to 9
move 1 from 3 to 5
move 8 from 4 to 6
move 2 from 5 to 2
move 4 from 7 to 4
move 1 from 5 to 9
move 5 from 4 to 7
move 1 from 4 to 8
move 2 from 2 to 6
move 1 from 5 to 3
move 4 from 9 to 6
move 11 from 6 to 8
move 1 from 1 to 4
move 1 from 4 to 1
move 1 from 3 to 1
move 10 from 1 to 4
move 3 from 9 to 5
move 1 from 9 to 3
move 2 from 7 to 4
move 3 from 9 to 4
move 3 from 5 to 8
move 1 from 3 to 5
move 15 from 8 to 2
move 8 from 1 to 4
move 2 from 1 to 2
move 10 from 2 to 3
move 1 from 5 to 7
move 3 from 7 to 8
move 10 from 3 to 5
move 4 from 4 to 2
move 7 from 4 to 1
move 2 from 7 to 4
move 1 from 8 to 9
move 5 from 1 to 6
move 13 from 6 to 2
move 2 from 1 to 4
move 15 from 4 to 5
move 1 from 9 to 3
move 1 from 3 to 4
move 2 from 8 to 3
move 20 from 2 to 6
move 3 from 2 to 8
move 2 from 3 to 8
move 9 from 5 to 2
move 6 from 5 to 9
move 2 from 4 to 1
move 8 from 5 to 4
move 2 from 8 to 1
move 5 from 9 to 5
move 3 from 5 to 7
move 1 from 8 to 2
move 2 from 4 to 1
move 14 from 6 to 4
move 2 from 1 to 8
move 5 from 6 to 3
move 3 from 1 to 6
move 5 from 3 to 2
move 1 from 9 to 6
move 8 from 6 to 2
move 2 from 7 to 4
move 1 from 1 to 3
move 2 from 5 to 8
move 5 from 4 to 3
move 2 from 5 to 3
move 1 from 7 to 5
move 4 from 4 to 3
move 2 from 4 to 2
move 1 from 3 to 7
move 5 from 3 to 7
move 1 from 7 to 3
move 3 from 3 to 2
move 1 from 5 to 9
move 2 from 7 to 9
move 1 from 9 to 5
move 1 from 5 to 3
move 10 from 4 to 9
move 3 from 3 to 9
move 27 from 2 to 5
move 3 from 8 to 3
move 2 from 2 to 6
move 4 from 9 to 7
move 5 from 3 to 8
move 5 from 7 to 3
move 25 from 5 to 1
move 3 from 9 to 8
move 1 from 3 to 2
move 1 from 5 to 3
move 2 from 7 to 9
move 10 from 8 to 7
move 1 from 2 to 3
move 13 from 1 to 7
move 3 from 9 to 7
move 3 from 3 to 1
move 1 from 5 to 8
move 2 from 8 to 6
move 4 from 6 to 5
move 4 from 5 to 6
move 1 from 4 to 6
move 23 from 7 to 9
move 2 from 6 to 8
move 28 from 9 to 1
move 1 from 8 to 1
move 3 from 7 to 3
move 1 from 9 to 4
move 3 from 3 to 6
move 3 from 3 to 4
move 6 from 6 to 8
move 12 from 1 to 7
move 9 from 1 to 6
move 3 from 6 to 3
move 2 from 4 to 7
move 4 from 8 to 7
move 1 from 8 to 5
move 1 from 8 to 4
move 8 from 1 to 7
move 1 from 3 to 4
move 1 from 8 to 3
move 3 from 7 to 5
move 9 from 1 to 3
move 3 from 6 to 5
move 3 from 1 to 7
move 4 from 4 to 2
move 3 from 1 to 4
move 4 from 2 to 8
move 1 from 6 to 2
move 3 from 5 to 6
move 4 from 8 to 5
move 9 from 7 to 6
move 12 from 7 to 1
move 5 from 7 to 3
move 1 from 9 to 7
move 1 from 2 to 9
move 1 from 9 to 4
move 7 from 6 to 3
move 5 from 6 to 2
move 1 from 7 to 6
move 3 from 6 to 1
move 2 from 4 to 9
move 7 from 5 to 8
move 2 from 9 to 4
move 1 from 5 to 8
move 4 from 4 to 1
move 11 from 1 to 7
move 8 from 3 to 1
move 8 from 8 to 6
move 3 from 3 to 5
move 5 from 6 to 1
move 2 from 1 to 2
move 6 from 2 to 3
move 2 from 6 to 7
move 3 from 5 to 4
move 7 from 3 to 9
move 5 from 9 to 5
move 3 from 4 to 3
move 4 from 5 to 2
move 2 from 9 to 4
move 6 from 1 to 9
move 1 from 6 to 9
move 7 from 7 to 1
move 1 from 7 to 3
move 2 from 4 to 5
move 1 from 9 to 1
move 4 from 2 to 3
move 2 from 5 to 2
move 9 from 3 to 1
move 3 from 2 to 4
move 28 from 1 to 6
move 2 from 1 to 3
move 17 from 6 to 3
move 2 from 9 to 5
move 2 from 6 to 7
move 1 from 5 to 7
move 1 from 9 to 4
move 5 from 6 to 9
move 14 from 3 to 5
move 15 from 5 to 9
move 1 from 4 to 9
move 1 from 5 to 6
move 1 from 4 to 1
move 11 from 3 to 6
move 1 from 1 to 6
move 12 from 6 to 8
move 4 from 9 to 7
move 20 from 9 to 4
move 18 from 4 to 5
move 6 from 5 to 8
move 12 from 8 to 2
move 2 from 2 to 6
move 1 from 5 to 2
move 4 from 4 to 8
move 5 from 5 to 9
move 4 from 3 to 6
move 1 from 3 to 8
move 7 from 7 to 8
move 10 from 2 to 8
move 1 from 6 to 3
move 10 from 6 to 5
move 10 from 5 to 2
move 2 from 7 to 5
move 9 from 2 to 1
move 27 from 8 to 9
move 2 from 2 to 7
move 9 from 1 to 2
move 1 from 5 to 3
move 9 from 2 to 1
move 1 from 8 to 7
move 2 from 1 to 3
move 19 from 9 to 1
move 5 from 5 to 1
move 3 from 9 to 2
move 2 from 3 to 9
move 1 from 3 to 4
move 5 from 7 to 4
move 1 from 7 to 3
move 17 from 1 to 2
move 1 from 5 to 3
move 9 from 9 to 5
move 2 from 1 to 2
move 1 from 4 to 9
move 2 from 4 to 6
move 1 from 4 to 7
move 6 from 1 to 8)";

void run()
{
  for (const auto &block : runCrane9000(taskInput)) {
    std::cout << block;
  }
  std::cout << '\n';
  for (const auto &block : runCrane9001(taskInput)) {
    std::cout << block;
  }
  std::cout << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL5_HPP
