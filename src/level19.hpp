//
// Created by object on 9/04/23.
//

#ifndef AOC_2022_CPP_LEVEL19_HPP
#define AOC_2022_CPP_LEVEL19_HPP

#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level19 {

struct Resources
{
  int ore{};
  int clay{};
  int obsidian{};
  int geode{};
};

struct Blueprint
{
  int id;
  int oreRobotCost;
  int clayRobotCost;
  std::pair<int, int> obsidianRobotCost;
  std::pair<int, int> geodeRobotCost;

  Blueprint(int id_,
    int oreRobotCost_,
    int clayRobotCost_,
    std::pair<int, int> obsidianRobotCost_,
    std::pair<int, int> geodeRobotCost_)
    : id(id_), oreRobotCost(oreRobotCost_), clayRobotCost(clayRobotCost_), obsidianRobotCost(std::move(obsidianRobotCost_)),
      geodeRobotCost(std::move(geodeRobotCost_))
  {}

  auto canMakeOreRobot(const auto &vault) const
  {
    return vault.ore >= oreRobotCost;
  }

  auto canMakeClayRobot(const auto &vault) const
  {
    return vault.ore >= clayRobotCost;
  }

  auto canMakeObsidianRobot(const auto &vault) const
  {
    return vault.ore >= obsidianRobotCost.first && vault.clay >= obsidianRobotCost.second;
  }

  auto canMakeGeodeRobot(const auto &vault) const
  {
    return vault.ore >= geodeRobotCost.first && vault.obsidian >= geodeRobotCost.second;
  }
};

auto readDigits(auto &start, const auto &end)
{
  int result{};
  while (start != end && *start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  if (result == 0) {
    throw std::runtime_error("failed to read digits");
  }
  return result;
}

auto parseInput(const auto &input)
{
  const auto lines = splitLines(input);
  std::vector<Blueprint> result;
  for (const auto &line : lines) {
    auto start = line.begin();
    auto end = line.end();
    start += std::strlen("Blueprint ");
    auto id = readDigits(start, end);
    start += std::strlen(": Each ore robot costs ");
    auto oreRobotCost = readDigits(start, end);
    start += std::strlen(" ore. Each clay robot costs ");
    auto clayRobotCost = readDigits(start, end);
    start += std::strlen(" ore. Each obsidian robot costs ");
    auto obsidianRobotOreCost = readDigits(start, end);
    start += std::strlen(" ore and ");
    auto obsidianRobotClayCost = readDigits(start, end);
    start += std::strlen(" clay. Each geode robot costs ");
    auto geodeRobotOreCost = readDigits(start, end);
    start += std::strlen(" ore and ");
    auto geodeRobotObsidianCost = readDigits(start, end);
    result.emplace_back(id,
      oreRobotCost,
      clayRobotCost,
      std::pair{ obsidianRobotOreCost, obsidianRobotClayCost },
      std::pair{ geodeRobotOreCost, geodeRobotObsidianCost });
  }
  return result;
}

void waitResources(auto &vault, const auto &production, auto oreNeeded, auto clayNeed, auto obsidianNeed, const auto &callback) {
  auto oreMissing = oreNeeded - vault.ore;
  auto clayMissing = clayNeed - vault.clay;
  auto obsidianMissing = obsidianNeed - vault.obsidian;
  int minutesWait{};
  if (oreMissing > 0) {
    minutesWait = std::max(minutesWait, oreMissing / production.ore + (oreMissing % production.ore == 0 ? 0 : 1));
  }
  if (clayMissing > 0) {
    if (production.clay == 0) {
      return;
    }
    minutesWait =
      std::max(minutesWait, clayMissing / production.clay + (clayMissing % production.clay == 0 ? 0 : 1));
  }
  if (obsidianMissing > 0) {
    if (production.obsidian == 0) {
      return;
    }
    minutesWait = std::max(
      minutesWait, obsidianMissing / production.obsidian + (obsidianMissing % production.obsidian == 0 ? 0 : 1));
  }
  vault.ore += production.ore * minutesWait;
  vault.clay += production.clay * minutesWait;
  vault.obsidian += production.obsidian * minutesWait;
  vault.geode += production.geode * minutesWait;
  vault.ore -= oreNeeded;
  vault.clay -= clayNeed;
  vault.obsidian -= obsidianNeed;
  callback(minutesWait);
  vault.ore += oreNeeded;
  vault.clay += clayNeed;
  vault.obsidian += obsidianNeed;
  vault.ore -= production.ore * minutesWait;
  vault.clay -= production.clay * minutesWait;
  vault.obsidian -= production.obsidian * minutesWait;
  vault.geode -= production.geode * minutesWait;
}

auto walk(const auto &blueprint, auto &vault, auto &production, int minutesLeft)
{
  if (minutesLeft == 0) {
    return vault.geode;
  }

  auto max = vault.geode + production.geode * minutesLeft;
  waitResources(
    vault, production, blueprint.geodeRobotCost.first, 0, blueprint.geodeRobotCost.second, [&](auto minutesPassed) {
      if (minutesPassed > minutesLeft) {
        return;
      }
      vault.ore += production.ore;
      vault.clay += production.clay;
      vault.obsidian += production.obsidian;
      vault.geode += production.geode;
      ++production.geode;
      max = std::max(max, walk(blueprint, vault, production, minutesLeft - minutesPassed - 1));
      --production.geode;
      vault.ore -= production.ore;
      vault.clay -= production.clay;
      vault.obsidian -= production.obsidian;
      vault.geode -= production.geode;
    });
  if (production.obsidian < blueprint.geodeRobotCost.second) {
    waitResources(vault,
      production,
      blueprint.obsidianRobotCost.first,
      blueprint.obsidianRobotCost.second,
      0,
      [&](auto minutesPassed) {
        if (minutesPassed > minutesLeft) {
          return;
        }
        vault.ore += production.ore;
        vault.clay += production.clay;
        vault.obsidian += production.obsidian;
        vault.geode += production.geode;
        ++production.obsidian;
        max = std::max(max, walk(blueprint, vault, production, minutesLeft - minutesPassed - 1));
        --production.obsidian;
        vault.ore -= production.ore;
        vault.clay -= production.clay;
        vault.obsidian -= production.obsidian;
        vault.geode -= production.geode;
      });
  }
  if ((blueprint.geodeRobotCost.second - production.obsidian) * blueprint.obsidianRobotCost.second > vault.clay
      && production.clay < blueprint.obsidianRobotCost.second) {
    waitResources(vault, production, blueprint.clayRobotCost, 0, 0, [&](auto minutesPassed) {
      if (minutesPassed > minutesLeft) {
        return;
      }
      vault.ore += production.ore;
      vault.clay += production.clay;
      vault.obsidian += production.obsidian;
      vault.geode += production.geode;
      ++production.clay;
      max = std::max(max, walk(blueprint, vault, production, minutesLeft - minutesPassed - 1));
      --production.clay;
      vault.ore -= production.ore;
      vault.clay -= production.clay;
      vault.obsidian -= production.obsidian;
      vault.geode -= production.geode;
    });
  }
  if (production.ore < blueprint.oreRobotCost || production.ore < blueprint.clayRobotCost
      || production.ore < blueprint.obsidianRobotCost.first || production.ore < blueprint.geodeRobotCost.first) {
    waitResources(vault, production, blueprint.oreRobotCost, 0, 0, [&](auto minutesPassed) {
      if (minutesPassed > minutesLeft) {
        return;
      }
      vault.ore += production.ore;
      vault.clay += production.clay;
      vault.obsidian += production.obsidian;
      vault.geode += production.geode;
      ++production.ore;
      max = std::max(max, walk(blueprint, vault, production, minutesLeft - minutesPassed - 1));
      --production.ore;
      vault.ore -= production.ore;
      vault.clay -= production.clay;
      vault.obsidian -= production.obsidian;
      vault.geode -= production.geode;
    });
  }
  return max;
}

auto calculateGeodes(const auto &blueprint, auto minutes)
{
  Resources vault{};
  Resources production{ 1, 0, 0, 0 };
  return walk(blueprint, vault, production, minutes);
}

auto part1(const auto &input)
{
  const auto blueprints = parseInput(input);
  return std::accumulate(blueprints.begin(), blueprints.end(), 0, [](auto sum, const auto &blueprint) {
    std::cout << "Calculating geodes for blueprint#" << blueprint.id << '\n';
    return sum + blueprint.id * calculateGeodes(blueprint, 24);
  });
}

auto part2(const auto &input)
{
  auto blueprints = parseInput(input);
  if (blueprints.size() > 3) {
    blueprints.erase(blueprints.begin() + 3, blueprints.end());
  }
  return std::accumulate(blueprints.begin(), blueprints.end(), 1, [](auto result, const auto &blueprint) {
    std::cout << "Calculating geodes for blueprint#" << blueprint.id << '\n';
    return result * calculateGeodes(blueprint, 32);
  });
}

static const auto taskInput =
  R"(Blueprint 1: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 15 clay. Each geode robot costs 3 ore and 9 obsidian.
Blueprint 2: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 12 clay. Each geode robot costs 4 ore and 19 obsidian.
Blueprint 3: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 14 clay. Each geode robot costs 3 ore and 16 obsidian.
Blueprint 4: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 17 clay. Each geode robot costs 3 ore and 19 obsidian.
Blueprint 5: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 10 clay. Each geode robot costs 2 ore and 10 obsidian.
Blueprint 6: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 16 clay. Each geode robot costs 3 ore and 9 obsidian.
Blueprint 7: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 7 clay. Each geode robot costs 3 ore and 9 obsidian.
Blueprint 8: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 19 clay. Each geode robot costs 2 ore and 12 obsidian.
Blueprint 9: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 16 clay. Each geode robot costs 3 ore and 20 obsidian.
Blueprint 10: Each ore robot costs 2 ore. Each clay robot costs 2 ore. Each obsidian robot costs 2 ore and 7 clay. Each geode robot costs 2 ore and 14 obsidian.
Blueprint 11: Each ore robot costs 2 ore. Each clay robot costs 4 ore. Each obsidian robot costs 2 ore and 16 clay. Each geode robot costs 4 ore and 12 obsidian.
Blueprint 12: Each ore robot costs 2 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 15 clay. Each geode robot costs 2 ore and 15 obsidian.
Blueprint 13: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 5 clay. Each geode robot costs 2 ore and 10 obsidian.
Blueprint 14: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 2 ore and 15 clay. Each geode robot costs 3 ore and 16 obsidian.
Blueprint 15: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 19 clay. Each geode robot costs 2 ore and 9 obsidian.
Blueprint 16: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 4 ore and 15 clay. Each geode robot costs 4 ore and 9 obsidian.
Blueprint 17: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 17 clay. Each geode robot costs 3 ore and 16 obsidian.
Blueprint 18: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 13 clay. Each geode robot costs 3 ore and 12 obsidian.
Blueprint 19: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 10 clay. Each geode robot costs 4 ore and 10 obsidian.
Blueprint 20: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 17 clay. Each geode robot costs 3 ore and 10 obsidian.
Blueprint 21: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 2 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 22: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 3 ore and 20 clay. Each geode robot costs 3 ore and 14 obsidian.
Blueprint 23: Each ore robot costs 2 ore. Each clay robot costs 2 ore. Each obsidian robot costs 2 ore and 17 clay. Each geode robot costs 2 ore and 10 obsidian.
Blueprint 24: Each ore robot costs 3 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 19 clay. Each geode robot costs 3 ore and 19 obsidian.
Blueprint 25: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 2 ore and 14 clay. Each geode robot costs 3 ore and 14 obsidian.
Blueprint 26: Each ore robot costs 3 ore. Each clay robot costs 4 ore. Each obsidian robot costs 3 ore and 13 clay. Each geode robot costs 3 ore and 19 obsidian.
Blueprint 27: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 7 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 28: Each ore robot costs 4 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 20 clay. Each geode robot costs 2 ore and 19 obsidian.
Blueprint 29: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 16 clay. Each geode robot costs 2 ore and 15 obsidian.
Blueprint 30: Each ore robot costs 2 ore. Each clay robot costs 4 ore. Each obsidian robot costs 4 ore and 19 clay. Each geode robot costs 2 ore and 18 obsidian.)";

void run()
{
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level19

#endif// AOC_2022_CPP_LEVEL19_HPP
