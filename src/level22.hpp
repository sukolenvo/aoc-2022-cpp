//
// Created by object on 14/04/23.
//

#ifndef AOC_2022_CPP_LEVEL22_HPP
#define AOC_2022_CPP_LEVEL22_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string_view>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level22 {

enum class Direction { right = 0, down = 1, left = 2, up = 3 };

auto readDigits(auto &start, const auto &end)
{
  int result{};
  while (start != end && *start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  return result;
}

auto getNextPosition(auto x, auto y, auto direction)
{
  switch (direction) {
    case Direction::right:
      return std::pair{ x + 1, y };
    case Direction::left:
      return std::pair{ x - 1, y };
    case Direction::up:
      return std::pair{ x, y - 1 };
    case Direction::down:
      return std::pair{ x, y + 1 };
    default:
      throw std::runtime_error("unexpected direction");
  }
}

auto turn(auto direction, auto side)
{
  if (side == 'L') {
    return static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
  }
  if (side != 'R') {
    throw std::runtime_error("unexpected side");
  }
  return static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
}

auto isOOB(const auto &nextPosition, const auto &lines)
{
  if (nextPosition.first < 0) {
    return true;
  }
  if (nextPosition.second < 0) {
    return true;
  }
  if (lines.size() == static_cast<size_t>(nextPosition.second)) {
    return true;
  }
  if (lines[static_cast<size_t>(nextPosition.second)].size() <= static_cast<size_t>(nextPosition.first)) {
    return true;
  }
  if (lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] == ' ') {
    return true;
  }
  return false;
}

auto walk(const auto &input, const auto &handleOutOfBounds)
{
  auto lines = splitLines(input);
  if (lines.size() < 3) {// gcc-12 shows warnings for read operations without checking size of input on Win
    throw std::runtime_error("invalid input");
  }
  auto instructions = lines.back();
  lines.erase(lines.end() - 2, lines.end());
  auto x =
    static_cast<int>(std::distance(lines.front().begin(), std::find(lines.front().begin(), lines.front().end(), '.')));
  int y = 0;
  auto direction = Direction::right;
  auto instruction = instructions.begin();
  while (instruction != instructions.end()) {
    auto distance = readDigits(instruction, instructions.end());
    for (auto i = 0; i < distance; ++i) {
      auto nextPosition = getNextPosition(x, y, direction);
      auto nextDirection = direction;
      auto outOfBounds = isOOB(nextPosition, lines);
      if (outOfBounds) {
        handleOutOfBounds(nextDirection, nextPosition, lines);
      }
      if (lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] == '#') {
        break;
      }
      if (lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] != '.') {
        throw std::runtime_error("unexpected tile");
      }
      x = nextPosition.first;
      y = nextPosition.second;
      direction = nextDirection;
    }
    if (instruction != instructions.end()) {
      direction = turn(direction, *instruction++);
    }
  }
  return 1000 * ++y + 4 * ++x + static_cast<int>(direction);
}

auto part1(const auto &input)
{
  return walk(input, [](auto &direction, auto &nextPosition, const auto &lines) {
    switch (direction) {
      case Direction::right:
        nextPosition.first = static_cast<int>(std::distance(lines[static_cast<size_t>(nextPosition.second)].begin(),
          std::find_if(lines[static_cast<size_t>(nextPosition.second)].begin(),
            lines[static_cast<size_t>(nextPosition.second)].end(),
            [](const auto &tile) { return tile != ' '; })));
        break;
      case Direction::down:
        nextPosition.second =
          static_cast<int>(std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [&](const auto &line) {
            return line.size() > static_cast<size_t>(nextPosition.first)
                   && line[static_cast<size_t>(nextPosition.first)] != ' ';
          })));
        break;
      case Direction::left:
        nextPosition.first = static_cast<int>(lines[static_cast<size_t>(nextPosition.second)].size() - 1);
        break;
      case Direction::up:
        nextPosition.second =
          static_cast<int>(
            std::distance(std::find_if(lines.rbegin(),
                            lines.rend(),
                            [&](const auto &line) { return line.size() > static_cast<size_t>(nextPosition.first); }),
              lines.rend()))
          - 1;
        break;
    }
  });
}

void part2handleOOBsample(Direction &direction,
  std::pair<int, int> &nextPosition,
  const std::vector<std::string_view> &lines)
{
  auto cubeSize = static_cast<int>(lines.size() / 3);
  if (direction == Direction::right) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize * 4 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = cubeSize * 4 - 1 - nextPosition.second + cubeSize;
      nextPosition.second = cubeSize * 2;
      direction = Direction::down;
    } else {
      nextPosition.first = cubeSize * 3 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    }
  } else if (direction == Direction::down) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = cubeSize * 2 + cubeSize * 2 - nextPosition.first - 1;
      nextPosition.first = cubeSize * 2;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 3) {
      nextPosition.second = cubeSize * 2 - 1;
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      direction = Direction::up;
    } else {
      nextPosition.second = cubeSize + cubeSize * 4 - 1 - nextPosition.first;
      nextPosition.first = 0;
      direction = Direction::right;
    }
  } else if (direction == Direction::left) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize + nextPosition.second;
      nextPosition.second = cubeSize;
      direction = Direction::down;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = cubeSize * 4 - nextPosition.second + cubeSize - 1;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    } else {
      nextPosition.first = cubeSize * 2 - 1 - nextPosition.second + cubeSize * 2;
      nextPosition.second = cubeSize * 2 - 1;
      direction = Direction::up;
    }
  } else if (direction == Direction::up) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      nextPosition.second = 0;
      direction = Direction::down;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = nextPosition.first - cubeSize;
      nextPosition.first = cubeSize * 2;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 3) {
      nextPosition.second = cubeSize;
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      direction = Direction::down;
    } else {
      nextPosition.second = cubeSize + cubeSize * 4 - 1 - nextPosition.first;
      nextPosition.first = cubeSize * 3 - 1;
      direction = Direction::left;
    }
  } else {
    throw std::runtime_error("unexpected direction");
  }
}

void part2handleOOBtask(Direction &direction,
  std::pair<int, int> &nextPosition,
  const std::vector<std::string_view> &lines)
{
  const auto cubeSize = static_cast<int>(lines.size() / 4);
  if (direction == Direction::right) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize * 2 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = nextPosition.second + cubeSize;
      nextPosition.second = cubeSize - 1;
      direction = Direction::up;
    } else if (nextPosition.second < cubeSize * 3) {
      nextPosition.first = cubeSize * 3 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else {
      nextPosition.first = nextPosition.second - cubeSize * 2;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    }
  } else if (direction == Direction::down) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 2 + nextPosition.first;
      nextPosition.second = 0;
      direction = Direction::down;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = cubeSize * 2 + nextPosition.first;
      nextPosition.first = cubeSize - 1;
      direction = Direction::left;
    } else {
      nextPosition.second = nextPosition.first - cubeSize;
      nextPosition.first = cubeSize * 2 - 1;
      direction = Direction::left;
    }
  } else if (direction == Direction::left) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = 0;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::right;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = nextPosition.second - cubeSize;
      nextPosition.second = cubeSize * 2;
      direction = Direction::down;
    } else if (nextPosition.second < cubeSize * 3) {
      nextPosition.first = cubeSize;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::right;
    } else {
      nextPosition.first = nextPosition.second - cubeSize * 2;
      nextPosition.second = 0;
      direction = Direction::down;
    }
  } else if (direction == Direction::up) {
    if (nextPosition.first < cubeSize) {
      nextPosition.second = nextPosition.first + cubeSize;
      nextPosition.first = cubeSize;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = nextPosition.first + cubeSize * 2;
      nextPosition.first = 0;
      direction = Direction::right;
    } else {
      nextPosition.second = cubeSize * 4 - 1;
      nextPosition.first = nextPosition.first - cubeSize * 2;
      direction = Direction::up;
    }
  } else {
    throw std::runtime_error("unexpected direction");
  }
}

auto readFoldingPattern(const auto &input)
{
  auto lines = splitLines(input);
  lines.erase(lines.end() - 2, lines.end());
  const auto surfaceArea = std::accumulate(lines.begin(), lines.end(), 0, [](int sum, const auto &line) {
    return sum + std::accumulate(line.begin(), line.end(), 0, [](int lineSum, const auto &c) {
      return lineSum + (c == ' ' ? 0 : 1);
    });
  });
  const auto cubeSize = static_cast<size_t>(std::sqrt(surfaceArea / 6));
  std::string pattern;
  for (size_t y = 0; y < lines.size(); y += cubeSize) {
    for (size_t x = 0; x < lines[y].size(); x += cubeSize) {
      pattern.append(lines[y][x] == ' ' ? " " : ".");
    }
    pattern.append("\n");
  }
  return pattern;
}

auto part2(const auto &input)
{
  const auto pattern = readFoldingPattern(input);
  if (pattern == "  .\n...\n  ..\n") {
    return walk(input, &part2handleOOBsample);
  }
  if (pattern == " ..\n .\n..\n.\n") {
    return walk(input, &part2handleOOBtask);
  }
  throw std::runtime_error(pattern + ": pattern is not supported");
}

void run()
{
  const auto taskInput = readTaskInput(22);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level22

#endif// AOC_2022_CPP_LEVEL22_HPP
