//
// Created by object on 4/01/23.
//

#ifndef AOC_2022_CPP_LEVEL3_HPP
#define AOC_2022_CPP_LEVEL3_HPP

#include <string_view>
#include <iostream>
#include <exception>
#include <vector>
#include <algorithm>
#include <set>

#include "common.hpp"

namespace level3 {

auto findSharedItems(const auto& left, const auto& right)
{
  std::set<char> sharedItems;
  auto leftIterator = left.begin();
  auto rightIterator = right.begin();
  while (leftIterator != left.end() && rightIterator != right.end()) {
    if (*leftIterator == *rightIterator) {
      sharedItems.insert(*leftIterator);
      ++leftIterator;
      ++rightIterator;
    } else if (*leftIterator > *rightIterator) {
      ++rightIterator;
    } else {
      ++leftIterator;
    }
  }
  return sharedItems;
}

int part1(const std::string_view& input)
{
  auto lines = splitLines(input);
  int totalPriority = 0;
  for (const auto &line : lines) {
    if (line.size() % 2 == 1) {
      throw std::invalid_argument("odd size rucksack");
    }
    auto left = std::vector<char>{line.begin(), line.begin() + line.size() / 2};
    auto right = std::vector<char>{line.begin() + line.size() / 2, line.end()};
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    auto sharedItems = findSharedItems(left, right);
    for (const auto& sharedItem : sharedItems) {
      if (sharedItem >= 'a' && sharedItem <= 'z') {
        totalPriority += sharedItem - 'a' + 1;
      } else if (sharedItem >= 'A' && sharedItem <= 'Z') {
        totalPriority += sharedItem - 'A' + 27; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      } else {
        throw std::runtime_error("unexpected shared item");
      }
    }
  }
  return totalPriority;
}

int part2(const std::string_view& input)
{
  auto lines = splitLines(input);
  int totalPriority = 0;
  if (lines.size() % 3 != 0) {
    throw std::invalid_argument("rucksacks can't be divided in groups of 3");
  }
  for (auto begin = lines.begin(); begin != lines.end();) {
    auto firstLine = *begin++;
    auto secondLine = *begin++;
    auto thirdLine = *begin++;
    auto firstCopy = std::vector<char>{firstLine.begin(), firstLine.end()};
    auto secondCopy = std::vector<char>{secondLine.begin(), secondLine.end()};
    auto thirdCopy = std::vector<char>{thirdLine.begin(), thirdLine.end()};
    std::sort(firstCopy.begin(), firstCopy.end());
    std::sort(secondCopy.begin(), secondCopy.end());
    std::sort(thirdCopy.begin(), thirdCopy.end());
    auto sharedItems12 = findSharedItems(firstCopy, secondCopy);
    auto sharedItems23 = findSharedItems(secondCopy, thirdCopy);
    auto sharedItems = findSharedItems(sharedItems12, sharedItems23);
    for (const auto& sharedItem : sharedItems) {
      if (sharedItem >= 'a' && sharedItem <= 'z') {
        totalPriority += sharedItem - 'a' + 1;
      } else if (sharedItem >= 'A' && sharedItem <= 'Z') {
        totalPriority += sharedItem - 'A' + 27; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
      } else {
        throw std::runtime_error("unexpected shared item");
      }
    }
  }
  return totalPriority;
}

void run()
{
  const auto taskInput = readTaskInput(3);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL3_HPP
