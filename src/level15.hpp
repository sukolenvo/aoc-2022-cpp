//
// Created by object on 20/03/23.
//

#ifndef AOC_2022_CPP_LEVEL15_HPP
#define AOC_2022_CPP_LEVEL15_HPP

#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level15 {

auto distance(auto x1, auto y1, auto x2, auto y2)
{
  return std::abs(x1 - x2) + std::abs(y1 - y2);
}

class Sensor
{
  int sensorX;
  int sensorY;
  int beaconX;
  int beaconY;

public:
  Sensor(int sensorX_, int sensorY_, int beaconX_, int beaconY_)
    : sensorX(sensorX_), sensorY(sensorY_), beaconX(beaconX_), beaconY(beaconY_)
  {}
  auto getSensorX() const
  {
    return sensorX;
  }

  auto getSensorY() const
  {
    return sensorY;
  }

  auto getBeaconX() const
  {
    return beaconX;
  }

  auto getBeaconY() const
  {
    return beaconY;
  }
};

auto parseNumber(auto &start, const auto &end)
{
  int result = 0;
  bool negative = *start == '-';
  if (negative) {
    ++start;
  }
  while (start != end && *start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  return negative ? -result : result;
}

auto parseInput(const auto &input)
{
  auto lines = splitLines(input);
  std::vector<Sensor> result;
  for (const auto &line : lines) {
    auto start = line.begin();
    start += std::strlen("Sensor at x=");
    auto sensorX = parseNumber(start, line.end());
    start += std::strlen(", y=");
    auto sensorY = parseNumber(start, line.end());
    start += std::strlen(": closest beacon is at x=");
    auto beaconX = parseNumber(start, line.end());
    start += std::strlen(", y=");
    auto beaconY = parseNumber(start, line.end());
    result.emplace_back(sensorX, sensorY, beaconX, beaconY);
  }
  return result;
}

auto part1(const auto &input, int line)
{
  auto sensors = parseInput(input);
  std::vector<std::pair<int, int>> deadZones;
  for (const auto &sensor : sensors) {
    auto distanceToClosestBeacon =
      distance(sensor.getSensorX(), sensor.getSensorY(), sensor.getBeaconX(), sensor.getBeaconY());
    auto distanceToTargetLine = std::abs(sensor.getSensorY() - line);
    if (distanceToTargetLine >= distanceToClosestBeacon) {
      continue;
    }
    deadZones.emplace_back(sensor.getSensorX() - distanceToClosestBeacon + distanceToTargetLine,
      sensor.getSensorX() + distanceToClosestBeacon - distanceToTargetLine);
  }
  std::sort(
    deadZones.begin(), deadZones.end(), [](const auto &left, const auto &right) { return left.first < right.first; });
  auto result = 0;
  for (size_t i = 0; i < deadZones.size(); ++i) {
    if (i + 1 < deadZones.size() && deadZones[i + 1].first <= deadZones[i].second) {
      result += deadZones[i + 1].first - deadZones[i].first;
      deadZones[i + 1].second = std::max(deadZones[i + 1].second, deadZones[i].second);
    } else {
      result += deadZones[i].second - deadZones[i].first + 1;
    }
  }
  std::set<int> beaconsInTargetLine;
  for (const auto &sensor : sensors) {
    if (sensor.getBeaconY() == line) {
      beaconsInTargetLine.insert(sensor.getBeaconX());
    }
  }
  return result - static_cast<int>(beaconsInTargetLine.size());
}

auto findOnlyBeaconPlace(const auto &input, int areaSize)
{
  auto sensors = parseInput(input);
  for (int i = 0; i <= areaSize; ++i) {
    std::vector<std::pair<int, int>> deadZones;
    for (const auto &sensor : sensors) {
      auto distanceToClosestBeacon =
        distance(sensor.getSensorX(), sensor.getSensorY(), sensor.getBeaconX(), sensor.getBeaconY());
      auto distanceToTargetLine = std::abs(sensor.getSensorY() - i);
      if (distanceToTargetLine >= distanceToClosestBeacon) {
        continue;
      }
      auto deadZoneStart = sensor.getSensorX() - distanceToClosestBeacon + distanceToTargetLine;
      auto deadZoneEnd = sensor.getSensorX() + distanceToClosestBeacon - distanceToTargetLine;
      if (deadZoneEnd < 0 || deadZoneStart > areaSize) {
        continue;
      }

      deadZones.emplace_back(std::max(deadZoneStart, 0), std::min(deadZoneEnd, areaSize));
    }
    std::sort(
      deadZones.begin(), deadZones.end(), [](const auto &left, const auto &right) { return left.first < right.first; });
    auto eliminatedTill = 0;
    for (const auto &deadZone : deadZones) {
      if (deadZone.first > eliminatedTill) {
        return std::pair{ eliminatedTill + 1, i };
      }
      eliminatedTill = std::max(eliminatedTill, deadZone.second);
    }
    if (eliminatedTill < areaSize) {
      return std::pair{ areaSize, i };
    }
  }
  throw std::runtime_error("cannot find beacon position");
}

auto part2(const auto &input, int line)
{
  auto result = findOnlyBeaconPlace(input, line);
  return static_cast<long>(result.first) * 4'000'000 + result.second;
}

void run()
{
  const auto taskInput = readTaskInput(15);
  std::cout << part1(taskInput, 2'000'000) << '\n';
  std::cout << part2(taskInput, 4'000'000) << '\n';
}
}// namespace level15

#endif// AOC_2022_CPP_LEVEL15_HPP
