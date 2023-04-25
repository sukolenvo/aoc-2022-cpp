//
// Created by object on 21/04/23.
//

#ifndef AOC_2022_CPP_LEVEL24_HPP
#define AOC_2022_CPP_LEVEL24_HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level24 {

enum class Direction {
  UP, DOWN, LEFT, RIGHT
};

struct Blizzard {
  size_t row;
  size_t column;
  Direction direction;
  Blizzard(const auto row_, const auto column_, const auto direction_) : row(row_), column(column_), direction(direction_) {

  }
};

auto walk(auto &blizzards, const auto rows, const auto columns,
  const std::pair<size_t, size_t> &from, const std::pair<size_t, size_t> &to) {
  const auto isSafe = [&](auto row, auto column) {
    return row > 0 && row < rows - 1 && std::none_of(blizzards.begin(), blizzards.end(), [&] (const auto &blizzard) {
      return blizzard.row == row && blizzard.column == column;
    });
  };
  auto positionHash = [](const auto &position) {
    return position.first * 1000 + position.second;
  };
  const auto runBlizzards = [&] {
    for (auto &blizzard : blizzards) {
      switch (blizzard.direction) {
        case Direction::UP:
          if (blizzard.row == 1) {
            blizzard.row = rows - 2;
          } else {
            --blizzard.row;
          }
          break;
        case Direction::DOWN:
          if (blizzard.row == rows - 2) {
            blizzard.row = 1;
          } else {
            ++blizzard.row;
          }
          break;
        case Direction::LEFT:
          if (blizzard.column == 1) {
            blizzard.column = columns - 2;
          } else {
            --blizzard.column;
          }
          break;
        case Direction::RIGHT:
          if (blizzard.column == columns - 2) {
            blizzard.column = 1;
          } else {
            ++blizzard.column;
          }
          break;
        default:
          throw std::runtime_error("unexpected direction");
      }
    }
  };
  std::unordered_set<std::pair<size_t, size_t>, decltype(positionHash)> positions{ from };
  auto minutes = 0;
  while (true) {
    ++minutes;
    runBlizzards();
    std::unordered_set<std::pair<size_t, size_t>, decltype(positionHash)> newPositions{from};
    for (const auto &position : positions) {
      if (isSafe(position.first, position.second)) {
        newPositions.emplace(position.first, position.second);
      }
      if (position.first > 1 && isSafe(position.first - 1, position.second)) {
        newPositions.emplace(position.first - 1, position.second);
      }
      if (position.second > 1 && isSafe(position.first, position.second - 1)) {
        newPositions.emplace(position.first, position.second - 1);
      }
      if (position.first < rows - 2 && isSafe(position.first + 1, position.second)) {
        newPositions.emplace(position.first +  1, position.second);
      }
      if (position.second < columns - 2 && isSafe(position.first, position.second + 1)) {
        newPositions.emplace(position.first, position.second + 1);
      }
    }
    if (newPositions.contains(to)) {
      runBlizzards();
      return minutes + 1;
    }
    positions = newPositions;
  }
}

auto part1(const auto &input) {
  const auto lines = splitLines(input);
  std::vector<Blizzard> blizzards;
  for (size_t row = 0; row < lines.size(); ++row) {
    for (size_t column = 0; column < lines[row].size(); ++column) {
      switch (lines[row][column]) {
        case '^':
          blizzards.emplace_back(row, column, Direction::UP);
          break;
        case 'v':
          blizzards.emplace_back(row, column, Direction::DOWN);
          break;
        case '>':
          blizzards.emplace_back(row, column, Direction::RIGHT);
          break;
        case '<':
          blizzards.emplace_back(row, column, Direction::LEFT);
          break;
      }
    }
  }
  return walk(blizzards, lines.size(), lines[0].size(), {0, 1}, {lines.size() - 2, lines[0].size() - 2});
}

auto part2(const auto &input) {
  const auto lines = splitLines(input);
  std::vector<Blizzard> blizzards;
  for (size_t row = 0; row < lines.size(); ++row) {
    for (size_t column = 0; column < lines[row].size(); ++column) {
      switch (lines[row][column]) {
        case '^':
          blizzards.emplace_back(row, column, Direction::UP);
          break;
        case 'v':
          blizzards.emplace_back(row, column, Direction::DOWN);
          break;
        case '>':
          blizzards.emplace_back(row, column, Direction::RIGHT);
          break;
        case '<':
          blizzards.emplace_back(row, column, Direction::LEFT);
          break;
      }
    }
  }
  int i = walk(blizzards, lines.size(), lines[0].size(), { 0, 1 }, { lines.size() - 2, lines[0].size() - 2 });
  int i1 = walk(blizzards, lines.size(), lines[0].size(), { lines.size() - 1, lines[0].size() - 2 }, { 1, 2 });
  int i2 = walk(blizzards, lines.size(), lines[0].size(), { 0, 1 }, { lines.size() - 2, lines[0].size() - 2 });
  return i + i1 + i2;
}

void run() {
  const auto taskInput = readTaskInput(24);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
};

#endif// AOC_2022_CPP_LEVEL24_HPP
