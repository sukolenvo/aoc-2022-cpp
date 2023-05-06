//
// Created by object on 4/04/23.
//

#ifndef AOC_2022_CPP_LEVEL18_HPP
#define AOC_2022_CPP_LEVEL18_HPP

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

#include "common.hpp"

namespace level18 {

struct Cube
{
  int x;
  int y;
  int z;

  Cube(int a, int b, int c) : x(a), y(b), z(c)
  {}
};

auto readDigits(auto &input, const auto &lineEnd)
{
  int result = 0;
  while (input != lineEnd && *input >= '0' && *input <= '9') {
    result *= 10;
    result += *input++ - '0';
  }
  return result;
}

auto parseInput(const auto &input)
{
  const auto lines = splitLines(input);
  std::vector<Cube> cubes;
  for (const auto &line : lines) {
    auto i = line.begin();
    const auto end = line.end();
    auto x = readDigits(i, end);
    ++i;
    auto y = readDigits(i, end);
    ++i;
    auto z = readDigits(i, end);
    cubes.emplace_back(x, y, z);
  }
  return cubes;
}

auto part1(const auto &input)
{
  auto cubes = parseInput(input);
  const auto comparator = [](const auto &left, const auto &right) {
    return left.x == right.x ? (left.y == right.y ? left.z < right.z : left.y < right.y) : left.x < right.x;
  };
  std::set<Cube, decltype(comparator)> cubeSet{ comparator };
  cubeSet.insert(cubes.begin(), cubes.end());
  int result{};
  for (const auto &cube : cubes) {
    if (!cubeSet.contains({ cube.x - 1, cube.y, cube.z })) {
      ++result;
    }
    if (!cubeSet.contains({ cube.x + 1, cube.y, cube.z })) {
      ++result;
    }
    if (!cubeSet.contains({ cube.x, cube.y - 1, cube.z })) {
      ++result;
    }
    if (!cubeSet.contains({ cube.x, cube.y + 1, cube.z })) {
      ++result;
    }
    if (!cubeSet.contains({ cube.x, cube.y, cube.z - 1 })) {
      ++result;
    }
    if (!cubeSet.contains({ cube.x, cube.y, cube.z + 1 })) {
      ++result;
    }
  }
  return result;
}

auto part2(const auto &input)
{
  auto cubes = parseInput(input);
  const auto comparator = [](const auto &left, const auto &right) {
    return left.x == right.x ? (left.y == right.y ? left.z < right.z : left.y < right.y) : left.x < right.x;
  };
  std::set<Cube, decltype(comparator)> cubeSet{ comparator };
  cubeSet.insert(cubes.begin(), cubes.end());
  const auto minX = std::min_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.x < right.x;
  })->x - 1;
  const auto maxX = std::max_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.x < right.x;
  })->x + 1;
  const auto minY = std::min_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.y < right.y;
  })->y - 1;
  const auto maxY = std::max_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.y < right.y;
  })->y + 1;
  const auto minZ = std::min_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.z < right.z;
  })->z - 1;
  const auto maxZ = std::max_element(cubes.begin(), cubes.end(), [](const auto &left, const auto &right) {
    return left.z < right.z;
  })->z + 1;
  std::set<Cube, decltype(comparator)> water{ comparator };
  std::function<void(int, int, int)> processWater = [&](auto x, auto y, auto z) {
    if (cubeSet.contains({ x, y, z }) || water.contains({ x, y, z })) {
      return;
    }
    if (x < minX || x > maxX || y < minY || y > maxY || z < minZ || z > maxZ) {
      return;
    }
    water.insert({ x, y, z });
    processWater(x - 1, y, z);
    processWater(x + 1, y, z);
    processWater(x, y - 1, z);
    processWater(x, y + 1, z);
    processWater(x, y, z - 1);
    processWater(x, y, z + 1);
  };
  for (auto x = minX; x <= maxX; ++x) {
    for (auto y = minY; y <= maxY; ++y) {
      processWater(x, y, minZ);
      processWater(x, y, maxZ);
    }
  }
  for (auto x = minX; x <= maxX; ++x) {
    for (auto z = minZ; z <= maxZ; ++z) {
      processWater(x, minY, z);
      processWater(x, maxZ, z);
    }
  }
  for (auto z = minX; z <= maxX; ++z) {
    for (auto y = minY; y <= maxY; ++y) {
      processWater(minX, y, z);
      processWater(maxX, y, z);
    }
  }
  int result{};
  for (const auto &cube : cubes) {
    if (water.contains({ cube.x - 1, cube.y, cube.z })) {
      ++result;
    }
    if (water.contains({ cube.x + 1, cube.y, cube.z })) {
      ++result;
    }
    if (water.contains({ cube.x, cube.y - 1, cube.z })) {
      ++result;
    }
    if (water.contains({ cube.x, cube.y + 1, cube.z })) {
      ++result;
    }
    if (water.contains({ cube.x, cube.y, cube.z - 1 })) {
      ++result;
    }
    if (water.contains({ cube.x, cube.y, cube.z + 1 })) {
      ++result;
    }
  }
  return result;
}


void run()
{
  const auto taskInput = readTaskInput(18);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level18

#endif// AOC_2022_CPP_LEVEL18_HPP
