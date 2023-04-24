//
// Created by object on 21/03/23.
//

#ifndef AOC_2022_CPP_LEVEL16_HPP
#define AOC_2022_CPP_LEVEL16_HPP

#include <iostream>
#include <string_view>
#include <vector>
#include <cstring>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

#include "common.hpp"

namespace level16
{

class Valve {
  std::string_view id;
  int rate;
  std::vector<std::string_view> tunnels;
public:
  Valve(const std::string_view &id_, int rate_, std::vector<std::string_view> tunnels_) : id(id_), rate(rate_), tunnels(std::move(tunnels_)) {

  }
  auto getId() const {
    return id;
  }

  auto getRate() const {
    return rate;
  }

  auto getTunnels() const {
    return tunnels;
  }
};

auto parseNumber(auto &start) {
  int result = 0;
  while(*start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  return result;
}

auto parseInput(const auto &input) {
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

struct PositionKey {
  std::string_view position;
  std::set<std::string_view> openValves;
  int minutesUsed;

  bool operator<(const auto &other) const {
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

auto traverseGraph(const auto &distances, auto &openValves,
  const auto &position, int minutesLeft,
  const auto &valves, const auto &valveIndices) -> int {
  int maxPressure = 0;
  const PositionKey positionKey{position, openValves, minutesLeft};
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
      auto solvedPressure = traverseGraph(distances, openValves, valve.getId(), openValveTs, valves, valveIndices) + openValveTs * valve.getRate();
      maxPressure = std::max(maxPressure, solvedPressure);
      openValves.erase(inserted.first);
    }
  }
  return result = maxPressure;
}

auto part1(const auto &input) {
  auto valves = parseInput(input);
  std::map<std::string_view, size_t> valveIndices;
  for (const auto &valve : valves) {
    valveIndices.insert({valve.getId(), valveIndices.size()});
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

auto calculateDistances(const auto &valveIndices, const auto &valves) {
  std::vector<std::vector<int>> distances{ valveIndices.size(), std::vector<int>(valveIndices.size())};
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

auto solve(int pressure, const auto &distances, const auto &openValves,
  const auto &position, const auto &otherPosition,
  int minutesUsed, int otherMinutesUsed,
  const auto &valves, const auto &valveIndices) {
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
        auto solvedPressure = solve(pressure + (26 - openValveTs) * valve.getRate(), distances, newOpenValves, valve.getId(), otherPosition, openValveTs, otherMinutesUsed, valves, valveIndices);
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
        auto solvedPressure = solve(pressure + (26 - openValveTs) * valve.getRate(), distances, newOpenValves, position, valve.getId(), minutesUsed, openValveTs, valves, valveIndices);
        maxPressure = std::max(maxPressure, solvedPressure);
      }
    }
  }
  return maxPressure;
}

auto enumerateMyMoves(int pressure, const auto &distances, auto &openValves,
  const auto &position, int minutesUsed,
  const auto &valves, const auto &valveIndices) -> int {
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

auto part2(const auto &input) {
  auto valves = parseInput(input);
  std::map<std::string_view, size_t> valveIndices;
  for (const auto &valve : valves) {
    valveIndices.insert({valve.getId(), valveIndices.size()});
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

static const auto taskInput = R"(Valve KR has flow rate=17; tunnels lead to valves WA, JQ, JY, KI
Valve JN has flow rate=0; tunnels lead to valves XS, JQ
Valve JY has flow rate=0; tunnels lead to valves KR, AP
Valve WE has flow rate=0; tunnels lead to valves GY, XS
Valve HW has flow rate=0; tunnels lead to valves GP, AA
Valve QS has flow rate=0; tunnels lead to valves WW, MF
Valve MF has flow rate=15; tunnels lead to valves RJ, QS
Valve IM has flow rate=0; tunnels lead to valves WH, HS
Valve RJ has flow rate=0; tunnels lead to valves MF, PM
Valve IG has flow rate=0; tunnels lead to valves WX, ZY
Valve YL has flow rate=0; tunnels lead to valves GY, EB
Valve LI has flow rate=25; tunnels lead to valves YS, SI
Valve WW has flow rate=6; tunnels lead to valves PJ, QS
Valve QK has flow rate=0; tunnels lead to valves MV, AU
Valve AU has flow rate=20; tunnels lead to valves QK, BT, VK
Valve WH has flow rate=0; tunnels lead to valves MV, IM
Valve YS has flow rate=0; tunnels lead to valves ZL, LI
Valve FR has flow rate=0; tunnels lead to valves XS, AA
Valve NX has flow rate=0; tunnels lead to valves KI, NG
Valve OI has flow rate=5; tunnels lead to valves SU, OX, LW, JH, DK
Valve YJ has flow rate=0; tunnels lead to valves XK, XU
Valve DK has flow rate=0; tunnels lead to valves OI, IZ
Valve SU has flow rate=0; tunnels lead to valves OI, XU
Valve OH has flow rate=0; tunnels lead to valves CX, WX
Valve HS has flow rate=8; tunnels lead to valves UY, IM, WJ, XK, UC
Valve UY has flow rate=0; tunnels lead to valves HS, OX
Valve AP has flow rate=0; tunnels lead to valves JY, GY
Valve JQ has flow rate=0; tunnels lead to valves KR, JN
Valve XK has flow rate=0; tunnels lead to valves YJ, HS
Valve PM has flow rate=0; tunnels lead to valves RJ, ZY
Valve WJ has flow rate=0; tunnels lead to valves AA, HS
Valve VK has flow rate=0; tunnels lead to valves AU, SI
Valve OX has flow rate=0; tunnels lead to valves UY, OI
Valve ZL has flow rate=0; tunnels lead to valves YS, MV
Valve LW has flow rate=0; tunnels lead to valves TT, OI
Valve TT has flow rate=0; tunnels lead to valves LW, MV
Valve PJ has flow rate=0; tunnels lead to valves UC, WW
Valve UC has flow rate=0; tunnels lead to valves PJ, HS
Valve XU has flow rate=3; tunnels lead to valves EB, CW, SU, TL, YJ
Valve XS has flow rate=4; tunnels lead to valves IZ, CW, WE, JN, FR
Valve CW has flow rate=0; tunnels lead to valves XU, XS
Valve TF has flow rate=0; tunnels lead to valves AA, TL
Valve EB has flow rate=0; tunnels lead to valves XU, YL
Valve WA has flow rate=0; tunnels lead to valves KR, BT
Valve GY has flow rate=9; tunnels lead to valves GP, AP, YL, ZO, WE
Valve IZ has flow rate=0; tunnels lead to valves DK, XS
Valve KI has flow rate=0; tunnels lead to valves NX, KR
Valve AA has flow rate=0; tunnels lead to valves HW, TF, FR, JH, WJ
Valve ZO has flow rate=0; tunnels lead to valves GY, QM
Valve QM has flow rate=0; tunnels lead to valves ZO, CX
Valve SI has flow rate=0; tunnels lead to valves LI, VK
Valve BT has flow rate=0; tunnels lead to valves AU, WA
Valve BC has flow rate=0; tunnels lead to valves CX, ZY
Valve NG has flow rate=21; tunnel leads to valve NX
Valve WX has flow rate=16; tunnels lead to valves IG, OH
Valve GP has flow rate=0; tunnels lead to valves HW, GY
Valve JH has flow rate=0; tunnels lead to valves AA, OI
Valve CX has flow rate=13; tunnels lead to valves OH, BC, QM
Valve TL has flow rate=0; tunnels lead to valves TF, XU
Valve MV has flow rate=14; tunnels lead to valves QK, ZL, WH, TT
Valve ZY has flow rate=10; tunnels lead to valves PM, BC, IG)";

void run() {
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL16_HPP
