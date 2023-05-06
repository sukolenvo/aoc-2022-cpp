//
// Created by object on 18/03/23.
//

#ifndef AOC_2022_CPP_LEVEL14_HPP
#define AOC_2022_CPP_LEVEL14_HPP

#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level14 {

struct RockSegment
{
  int startX;// start is top left
  int startY;
  int endX;
  int endY;

  RockSegment(int x1, int y1, int x2, int y2)
    : startX(std::min(x1, x2)), startY(std::min(y1, y2)), endX(std::max(x1, x2)), endY(std::max(y1, y2))
  {}

  bool isHorizontal() const
  {
    return startY == endY;
  }

  auto blocks(int x, int y) const
  {
    if (isHorizontal()) {
      return startY == y && startX <= x && endX >= x;
    }
    return startX == x && startY <= y && endY >= y;
  }
};

auto parseCoordinate(auto &input, const auto &inputEnd)
{
  int result = 0;
  while (input != inputEnd && *input >= '0' && *input <= '9') {
    result *= 10;
    result += *input++ - '0';
  }
  return result;
}

auto parseInput(const auto &input)
{
  auto lines = splitLines(input);
  std::vector<RockSegment> result;
  for (const auto &line : lines) {
    auto start = line.begin();
    auto end = line.end();
    auto startX = parseCoordinate(start, end);
    ++start;
    auto startY = parseCoordinate(start, end);
    while (start != end) {
      start += std::strlen(" -> ");
      auto endX = parseCoordinate(start, end);
      ++start;
      auto endY = parseCoordinate(start, end);
      result.emplace_back(startX, startY, endX, endY);
      startX = endX;
      startY = endY;
    }
  }
  return result;
}

auto part1(const auto &input)
{
  auto rocks = parseInput(input);
  std::set<std::pair<int, int>> restingSand;
  const auto caveBottom = std::max_element(rocks.begin(), rocks.end(), [](const auto &left, const auto &right) {
    return left.endY < right.endY;
  })->endY;
  auto blocked = [&](int x, int y) {
    if (restingSand.contains({ x, y })) {
      return true;
    }
    return std::any_of(rocks.begin(), rocks.end(), [&](const auto &rock) { return rock.blocks(x, y); });
  };
  auto sandCount{ 0 };
  while (true) {
    auto sandX = 500;
    auto sandY = 0;
    while (true) {
      if (sandY == caveBottom) {
        return sandCount;
      }
      if (!blocked(sandX, sandY + 1)) {
        ++sandY;
      } else if (!blocked(sandX - 1, sandY + 1)) {
        --sandX;
        ++sandY;
      } else if (!blocked(sandX + 1, sandY + 1)) {
        ++sandX;
        ++sandY;
      } else {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
    }
  }
}

auto part2(const auto &input)
{
  const auto rocks = parseInput(input);
  std::map<int, std::vector<RockSegment>> horizontalRocks;
  std::map<int, std::vector<RockSegment>> verticalRocks;
  for (const auto &rock : rocks) {
    if (rock.isHorizontal()) {
      horizontalRocks[rock.startY].push_back(rock);
    } else {
      verticalRocks[rock.startX].push_back(rock);
    }
  }
  std::set<std::pair<int, int>> restingSand;
  const auto caveBottom = std::max_element(rocks.begin(), rocks.end(), [](const auto &left, const auto &right) {
    return left.endY < right.endY;
  })->endY;
  auto checkBlocked = [&](int x, int y) {
    if (restingSand.contains({ x, y })) {
      return true;
    }
    if (horizontalRocks.contains(y)) {
      auto areaBlocked = std::any_of(
        horizontalRocks[y].begin(), horizontalRocks[y].end(), [&](const auto &rock) { return rock.blocks(x, y); });
      if (areaBlocked) {
        return true;
      }
    }
    if (verticalRocks.contains(x)) {
      return std::any_of(
        verticalRocks[x].begin(), verticalRocks[x].end(), [&](const auto &rock) { return rock.blocks(x, y); });
    }
    return false;
  };
  auto sandCount{ 0 };
  while (true) {
    auto sandX = 500;
    auto sandY = 0;
    if (restingSand.contains({ sandX, sandY })) {
      break;
    }
    while (true) {
      if (sandY == caveBottom + 1) {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
      if (!checkBlocked(sandX, sandY + 1)) {
        ++sandY;
      } else if (!checkBlocked(sandX - 1, sandY + 1)) {
        --sandX;
        ++sandY;
      } else if (!checkBlocked(sandX + 1, sandY + 1)) {
        ++sandX;
        ++sandY;
      } else {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
    }
  }
  return sandCount;
}

void run()
{
  const auto taskInput = readTaskInput(14);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level14

#endif// AOC_2022_CPP_LEVEL14_HPP
