//
// Created by object on 1/04/23.
//

#ifndef AOC_2022_CPP_LEVEL17_HPP
#define AOC_2022_CPP_LEVEL17_HPP

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level17 {

class Rock
{
  std::vector<std::vector<bool>> blocks;

public:
  explicit Rock(decltype(blocks) &&rock) : blocks(rock)
  {}
  auto getWidth() const
  {
    return blocks[0].size();
  }

  auto getHeight() const
  {
    return blocks.size();
  }

  auto isBlock(size_t x, size_t y) const
  {
    return blocks[y][x];
  }
};

auto canMoveRock(const auto &rock, int left, auto height, const auto &chamber)
{
  for (size_t x = 0; x < rock.getWidth(); ++x) {
    int chamberX = left + static_cast<int>(x);
    if (chamberX < 0 || chamberX >= static_cast<int>(chamber[0].size())) {
      return false;
    }
    for (size_t y = 0; y < rock.getHeight(); ++y) {
      size_t chamberY = static_cast<size_t>(height + rock.getHeight() - 1 - y);
      if (chamberY >= chamber.size()) {
        continue;
      }
      if (rock.isBlock(x, y) && chamber[chamberY][static_cast<size_t>(chamberX)]) {
        return false;
      }
    }
  }
  return true;
}

// NOLINT(constParameter)
void moveRock(const auto &rock, auto &chamber, auto &nextWindLeft)
{ // cppcheck-suppress constParameter
  int left = 2;
  size_t height = chamber.size() + 3;
  while (true) {
    const int newLeft = nextWindLeft() ? left - 1 : left + 1;
    if (canMoveRock(rock, newLeft, height, chamber)) {
      //      std::cout << (newLeft > left ? '>' : '<');
      left = newLeft;
    } else {
      //      std::cout << (newLeft > left ? ')' : '(');
    }
    if (canMoveRock(rock, left, height - 1, chamber)) {
      --height;
    } else {
      //      std::cout << '\n';
      for (size_t y = 0; y < rock.getHeight(); ++y) {
        auto &line = height + y >= chamber.size() ? chamber.emplace_back() : chamber[height + y];
        for (size_t x = 0; x < rock.getWidth(); ++x) {
          line[static_cast<size_t>(left) + x] |= rock.isBlock(x, rock.getHeight() - 1 - y);
        }
      }
      return;
    }
  }
}

std::vector<Rock> buildRocks()
{
  std::vector<Rock> rocks;
  rocks.emplace_back(std::vector<std::vector<bool>>{ { true, true, true, true } });
  rocks.emplace_back(
    std::vector<std::vector<bool>>{ { false, true, false }, { true, true, true }, { false, true, false } });
  rocks.emplace_back(std::vector<std::vector<bool>>{
    { false, false, true },
    { false, false, true },
    { true, true, true },
  });
  rocks.emplace_back(std::vector<std::vector<bool>>{
    { true },
    { true },
    { true },
    { true },
  });
  rocks.emplace_back(std::vector<std::vector<bool>>{
    { true, true },
    { true, true },
  });
  return rocks;
}

auto part1(const auto &input)
{
  const std::string_view wind = { input };
  auto nextWindLeft = [&wind, i = std::size_t{ 0 }]() mutable { return wind.at(i++ % wind.size()) == '<'; };
  const auto rocks = buildRocks();
  static const size_t cycles = 2022;
  std::vector<std::array<bool, 7>> chamber;
  chamber.push_back({ true, true, true, true, true, true, true });
  for (size_t i = 0; i < cycles; ++i) {
    moveRock(rocks[i % rocks.size()], chamber, nextWindLeft);
  }
  return chamber.size() - 1;
}

struct CycleInfo
{
  size_t rocks;
  size_t height;
  size_t rocksPreCycle;
  size_t heightPreCycle;
  size_t windIndex;
};

void process(auto line, auto column, auto &processed, const auto &chamber)
{
  if (!processed.contains({ line, column }) && !chamber[line][column]) {
    processed.insert({ line, column });
    if (column > 0) {
      process(line, column - 1, processed, chamber);
    }
    if (column < chamber[0].size() - 1) {
      process(line, column + 1, processed, chamber);
    }
    if (line > 0) {
      process(line - 1, column, processed, chamber);
    }
  }
}

auto findFloor(const auto &chamber)
{
  std::set<std::pair<size_t, size_t>> accessible;
  for (size_t i = 0; i < chamber[0].size(); ++i) {
    process(chamber.size() - 1, i, accessible, chamber);
  }
  if (accessible.empty()) {
    return chamber.size();
  }
  return std::min_element(accessible.begin(), accessible.end(), [](const auto &left, const auto &right) {
    return left.first < right.first;
  })->first;
}

CycleInfo findCycle(const auto &wind, auto &chamber, const auto &rocks)
{
  size_t windIndex{};
  size_t rockIndex{};
  auto nextWindLeft = [&] { return wind.at(windIndex++ % wind.size()) == '<'; };
  std::map<std::pair<size_t, std::vector<std::array<bool, 7>>>, std::pair<size_t, size_t>> surfaces;
  size_t erased{};
  while (true) {
    if (rockIndex % rocks.size() == 0) {
      auto splitIndex = findFloor(chamber) - 1;
      if (splitIndex > 0) {
        erased += splitIndex;
        //        for (size_t i = 0; i < chamber.size(); ++i) {
        //          for (size_t j = 0; j < chamber[0].size(); ++j) {
        //            std::cout << (chamber[chamber.size() - i - 1][j] ? '#' : '.');
        //          }
        //          std::cout << '\n';
        //          if (i == chamber.size() - splitIndex - 1) {
        //            std::cout << "-------\n";
        //          }
        //        }
        //        std::cout << '\n';
        chamber.erase(chamber.begin(), chamber.begin() + static_cast<long>(splitIndex));
      }
      const std::pair<size_t, std::vector<std::array<bool, 7>>> key = { windIndex % wind.size(), chamber };
      if (surfaces.contains(key)) {
        const auto &cycleStart = surfaces[key];
        return {
          rockIndex - cycleStart.first, erased - cycleStart.second, cycleStart.first, cycleStart.second, windIndex
        };
      } else {
        surfaces[key] = { rockIndex, erased };
      }
    }
    moveRock(rocks[rockIndex++ % rocks.size()], chamber, nextWindLeft);
  }
}

auto part2(const auto &input)
{
  const std::string_view wind = { input };
  const auto rocks = buildRocks();
  std::vector<std::array<bool, 7>> chamber;
  chamber.push_back({ true, true, true, true, true, true, true });
  const auto &cycleInfo = findCycle(wind, chamber, rocks);
  const auto cycles = (1'000'000'000'000 - cycleInfo.rocksPreCycle) / cycleInfo.rocks;
  const auto rocksPostCycle = (1'000'000'000'000 - cycleInfo.rocksPreCycle) % cycleInfo.rocks;
  auto nextWindLeft = [&wind, i = cycleInfo.windIndex]() mutable { return wind.at(i++ % wind.size()) == '<'; };
  for (size_t i = 0; i < rocksPostCycle; ++i) {
    moveRock(rocks[i % rocks.size()], chamber, nextWindLeft);
  }
  return cycleInfo.heightPreCycle + cycles * cycleInfo.height + chamber.size() - 1;
}

void run()
{
  const auto taskInput = readTaskInput(17);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
} // namespace level17

#endif // AOC_2022_CPP_LEVEL17_HPP
