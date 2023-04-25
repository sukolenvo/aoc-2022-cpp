//
// Created by object on 5/03/23.
//

#ifndef AOC_2022_CPP_LEVEL8_HPP
#define AOC_2022_CPP_LEVEL8_HPP

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

#include "common.hpp"

namespace level8
{

auto parseInput(const auto &input) {
  auto lines = splitLines(input);
  std::vector<std::vector<int>> forest;
  for (const auto &line : lines) {
    auto &trees = forest.emplace_back();
    std::transform(line.cbegin(), line.cend(), std::back_inserter(trees), [](const auto &c) {
      return c - '0';
    });
  }
  return forest;
}

auto part1(const auto &input) {
  static const auto maxHeight = 9;
  auto forest = parseInput(input);
  std::set<size_t> visibleTrees;
  const auto lines = forest.size();
  const auto columns = forest[0].size();

  for (size_t i = 0; i < columns; ++i) { // top
    int lastTree = -1;
    for (size_t j = 0; j < lines; ++j) {
      if (forest[j][i] > lastTree) {
        visibleTrees.insert(j * columns + i);
        lastTree = forest[j][i];
      }
      if (lastTree == maxHeight) {
        break;
      }
    }
  }

  for (size_t i = 0; i < lines; ++i) { // right
    int lastTree = -1;
    for (size_t j = 0; j < columns; ++j) {
      auto column = columns - 1 - j;
      if (forest[i][column] > lastTree) {
        visibleTrees.insert(i * columns + column);
        lastTree = forest[i][column];
      }
      if (lastTree == maxHeight) {
        break;
      }
    }
  }

  for (size_t i = 0; i < columns; ++i) { // bottom
    int lastTree = -1;
    for (size_t j = 0; j < lines; ++j) {
      auto line = lines - 1 - j;
      if (forest[line][i] > lastTree) {
        visibleTrees.insert(line * columns + i);
        lastTree = forest[line][i];
      }
      if (lastTree == maxHeight) {
        break;
      }
    }
  }

  for (size_t i = 0; i < lines; ++i) { // left
    int lastTree = -1;
    for (size_t j = 0; j < columns; ++j) {
      if (forest[i][j] > lastTree) {
        visibleTrees.insert(i * columns + j);
        lastTree = forest[i][j];
      }
      if (lastTree == maxHeight) {
        break;
      }
    }
  }
  return visibleTrees.size();
}

auto part2(const auto &input) {
  auto forest = parseInput(input);
  size_t maxScore = 0;
  for (size_t i = 0; i < forest.size(); ++i) {
    for (size_t j = 0; j < forest[0].size(); ++j) {
      const auto treeHeight = forest[i][j];
      size_t distanceTop = 0;
      while (i > distanceTop) {
        if (forest[i - ++distanceTop][j] >= treeHeight) {
          break;
        }
      }
      size_t distanceBottom = 0;
      while (i + distanceBottom < forest.size() - 1) {
        if (forest[i + ++distanceBottom][j] >= treeHeight) {
          break;
        }
      }
      size_t distanceRight = 0;
      while (j + distanceRight < forest[0].size() - 1) {
        if (forest[i][j + ++distanceRight] >= treeHeight) {
          break;
        }
      }
      size_t distanceLeft = 0;
      while (j > distanceLeft) {
        if (forest[i][j - ++distanceLeft] >= treeHeight) {
          break;
        }
      }
      maxScore = std::max(maxScore, distanceTop * distanceRight * distanceBottom * distanceLeft);
    }
  }
  return maxScore;
}

void run() {
  const auto *taskInput = readTaskInput(8);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL8_HPP
