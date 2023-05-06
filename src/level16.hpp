//
// Created by object on 21/03/23.
//

#ifndef AOC_2022_CPP_LEVEL16_HPP
#define AOC_2022_CPP_LEVEL16_HPP

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string_view>
#include <vector>

#include "common.hpp"

namespace level16 {

class Valve
{
  std::string_view id;
  int rate;
  std::vector<std::string_view> tunnels;

public:
  Valve(const std::string_view &id_, int rate_, std::vector<std::string_view> tunnels_)
    : id(id_), rate(rate_), tunnels(std::move(tunnels_))
  {}
  auto getId() const
  {
    return id;
  }

  auto getRate() const
  {
    return rate;
  }

  auto getTunnels() const
  {
    return tunnels;
  }
};

auto parseNumber(auto &start)
{
  int result = 0;
  while (*start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  return result;
}

auto parseInput(const auto &input)
{
  auto lines = splitLines(input);
  std::vector<Valve> valves;
  for (const auto &line : lines) {
    auto start = line.begin();
    start += std::strlen("Valve ");
    auto id = std::string_view(start, start + 2);
    start += std::strlen("KR has flow rate=");
    auto rate = parseNumber(start);
    start += std::strlen("; tunnel lead to valves ");
    if (*start == ' ') {
      ++start;
    }
    std::vector<std::string_view> tunnels;
    tunnels.emplace_back(start, start + 2);
    while (start + 2 != line.end()) {
      start += 4;
      tunnels.emplace_back(start, start + 2);
    }
    valves.emplace_back(id, rate, std::move(tunnels));
  }
  return valves;
}

struct PositionKey
{
  std::string_view position;
  std::set<std::string_view> openValves;
  int minutesUsed;

  bool operator<(const auto &other) const
  {
    if (minutesUsed == other.minutesUsed) {
      if (position == other.position) {
        return openValves < other.openValves;
      } else {
        return position < other.position;
      }
    } else {
      return minutesUsed < other.minutesUsed;
    }
  }
};

auto traverseGraph(const auto &distances,
  auto &openValves,
  const auto &position,
  int minutesLeft,
  const auto &valves,
  const auto &valveIndices) -> int
{
  int maxPressure = 0;
  const PositionKey positionKey{ position, openValves, minutesLeft };
  static std::map<PositionKey, int> cache;
  auto &result = cache[positionKey];
  if (result != 0) {
    return result;
  }
  const auto index = valveIndices.find(position)->second;
  for (const auto &valve : valves) {
    const auto openValveTs = minutesLeft - distances[index][valveIndices.find(valve.getId())->second] - 1;
    if (openValveTs < 0) {
      continue;
    }
    auto inserted = openValves.insert(valve.getId());
    if (inserted.second) {
      auto solvedPressure = traverseGraph(distances, openValves, valve.getId(), openValveTs, valves, valveIndices)
                            + openValveTs * valve.getRate();
      maxPressure = std::max(maxPressure, solvedPressure);
      openValves.erase(inserted.first);
    }
  }
  return result = maxPressure;
}

auto part1(const auto &input)
{
  auto valves = parseInput(input);
  std::map<std::string_view, size_t> valveIndices;
  for (const auto &valve : valves) {
    valveIndices.insert({ valve.getId(), valveIndices.size() });
  }
  const auto distances = calculateDistances(valveIndices, valves);
  std::set<std::string_view> openValves;
  for (const auto &valve : valves) {
    if (valve.getRate() == 0) {
      openValves.insert(valve.getId());
    }
  }
  return traverseGraph(distances, openValves, "AA", 30, valves, valveIndices);
}

auto calculateDistances(const auto &valveIndices, const auto &valves)
{
  std::vector<std::vector<int>> distances{ valveIndices.size(), std::vector<int>(valveIndices.size()) };
  for (const auto &valve : valves) {
    const size_t valveIdx = valveIndices.find(valve.getId())->second;
    for (const auto &linked : valve.getTunnels()) {
      auto linkedIdx = valveIndices.find(linked);
      if (linkedIdx != valveIndices.end()) {
        distances[valveIdx][linkedIdx->second] = 1;
      }
    }
  }
  auto allDistancesSet = [&]() {
    for (size_t i = 0; i < distances.size(); ++i) {
      for (size_t j = 0; j < distances.size(); ++j) {
        if (i != j && distances[i][j] == 0) {
          return false;
        }
      }
    }
    return true;
  };
  while (!allDistancesSet()) {
    for (size_t i = 0; i < valveIndices.size(); ++i) {
      for (size_t j = 0; j < valveIndices.size(); ++j) {
        if (i == j) {
          continue;
        }
        if (distances[i][j] != 0) {
          for (size_t k = 0; k < valveIndices.size(); ++k) {
            if (k == j) {
              continue;
            }
            if (distances[k][i] != 0) {
              if (distances[k][j] == 0) {
                distances[k][j] = distances[i][j] + distances[k][i];
              } else {
                distances[k][j] = std::min(distances[k][j], distances[i][j] + distances[k][i]);
              }
            }
          }
        }
      }
    }
  }
  return distances;
}

auto solve(int pressure,
  const auto &distances,
  const auto &openValves,
  const auto &position,
  const auto &otherPosition,
  int minutesUsed,
  int otherMinutesUsed,
  const auto &valves,
  const auto &valveIndices)
{
  if (minutesUsed >= 26 && otherMinutesUsed >= 26) {
    return pressure;
  }
  if (valves.size() == openValves.size()) {
    return pressure;
  }
  int maxPressure = pressure;
  if (minutesUsed < 26) {
    const auto index = valveIndices.find(position)->second;
    for (const auto &valve : valves) {
      if (!openValves.contains(valve.getId())) {
        const auto openValveTs = minutesUsed + distances[index][valveIndices.find(valve.getId())->second] + 1;
        if (openValveTs > 26) {
          continue;
        }
        auto newOpenValves = openValves;
        newOpenValves.insert(valve.getId());
        auto solvedPressure = solve(pressure + (26 - openValveTs) * valve.getRate(),
          distances,
          newOpenValves,
          valve.getId(),
          otherPosition,
          openValveTs,
          otherMinutesUsed,
          valves,
          valveIndices);
        maxPressure = std::max(maxPressure, solvedPressure);
      }
    }
  }
  if (otherMinutesUsed < 26) {
    const auto index = valveIndices.find(otherPosition)->second;
    for (const auto &valve : valves) {
      if (!openValves.contains(valve.getId())) {
        const auto openValveTs = otherMinutesUsed + distances[index][valveIndices.find(valve.getId())->second] + 1;
        if (openValveTs > 26) {
          continue;
        }
        auto newOpenValves = openValves;
        newOpenValves.insert(valve.getId());
        auto solvedPressure = solve(pressure + (26 - openValveTs) * valve.getRate(),
          distances,
          newOpenValves,
          position,
          valve.getId(),
          minutesUsed,
          openValveTs,
          valves,
          valveIndices);
        maxPressure = std::max(maxPressure, solvedPressure);
      }
    }
  }
  return maxPressure;
}

auto enumerateMyMoves(int pressure,
  const auto &distances,
  auto &openValves,
  const auto &position,
  int minutesUsed,
  const auto &valves,
  const auto &valveIndices) -> int
{
  int maxPressure = pressure + traverseGraph(distances, openValves, "AA", 26, valves, valveIndices);
  const auto index = valveIndices.find(position)->second;
  for (const auto &valve : valves) {
    const auto openValveTs = minutesUsed + distances[index][valveIndices.find(valve.getId())->second] + 1;
    if (openValveTs > 26) {
      continue;
    }
    auto inserted = openValves.insert(valve.getId());
    if (inserted.second) {
      auto solvedPressure = enumerateMyMoves(pressure + (26 - openValveTs) * valve.getRate(),
        distances,
        openValves,
        valve.getId(),
        openValveTs,
        valves,
        valveIndices);
      maxPressure = std::max(maxPressure, solvedPressure);
      openValves.erase(inserted.first);
    }
  }
  return maxPressure;
}

auto part2(const auto &input)
{
  auto valves = parseInput(input);
  std::map<std::string_view, size_t> valveIndices;
  for (const auto &valve : valves) {
    valveIndices.insert({ valve.getId(), valveIndices.size() });
  }
  const auto distances = calculateDistances(valveIndices, valves);
  std::set<std::string_view> openValves;
  for (const auto &valve : valves) {
    if (valve.getRate() == 0) {
      openValves.insert(valve.getId());
    }
  }
  return enumerateMyMoves(0, distances, openValves, "AA", 0, valves, valveIndices);
}

void run()
{
  const auto taskInput = readTaskInput(16);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level16

#endif// AOC_2022_CPP_LEVEL16_HPP
