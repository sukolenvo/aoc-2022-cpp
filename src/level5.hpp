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

void run()
{
  const auto *taskInput = readTaskInput(5);
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
