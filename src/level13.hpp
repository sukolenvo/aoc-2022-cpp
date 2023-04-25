//
// Created by object on 15/03/23.
//

#ifndef AOC_2022_CPP_LEVEL13_HPP
#define AOC_2022_CPP_LEVEL13_HPP

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <variant>
#include <vector>

#include "common.hpp"

namespace level13 {

struct Packet
{
  std::vector<std::variant<int, Packet>> fragments;

  bool operator==(const Packet &other)
  {
    if (fragments.size() != other.fragments.size()) {
      return false;
    }
    for (size_t i = 0; i < fragments.size(); ++i) {
      if (std::holds_alternative<int>(fragments[i]) && std::holds_alternative<int>(other.fragments[i])) {
        if (std::get<int>(fragments[i]) != std::get<int>(other.fragments[i])) {
          return false;
        }
        continue;
      }
      if (std::holds_alternative<Packet>(fragments[i]) && std::holds_alternative<Packet>(other.fragments[i])) {
        if (std::get<Packet>(fragments[i]) != std::get<Packet>(other.fragments[i])) {
          return false;
        }
        continue;
      }
      return false;
    }
    return true;
  }
};

auto parseNumber(auto &input)
{
  int result = 0;
  while (*input >= '0' && *input <= '9') {
    result *= 10;
    result += *input++ - '0';
  }
  return result;
}

Packet parsePacket(auto &line)
{
  Packet result;
  ++line;
  while (true) {
    if (*line == '[') {
      result.fragments.push_back(parsePacket(line));
    }
    if (*line >= '0' && *line <= '9') {
      result.fragments.push_back(parseNumber(line));
    }
    if (*line == ']') {
      ++line;
      return result;
    }
    if (*line != ',') {
      throw std::invalid_argument("failed to parse packet");
    }
    ++line;
  }
}

auto parseInput(const auto &input)
{
  auto lines = splitLines(input);
  std::vector<std::pair<Packet, Packet>> result;
  for (auto start = lines.begin(); start != lines.end();) {
    auto rightLine = start++->begin();
    auto leftLine = start++->begin();
    result.emplace_back(parsePacket(rightLine), parsePacket(leftLine));
    if (start != lines.end()) {
      ++start;
    }
  }
  return result;
}

enum class PacketValidationResult { VALID, INVALID, UNDEFINED };

auto validatePackets(const auto &left, const auto &right)
{
  for (size_t i = 0; i < left.fragments.size(); ++i) {
    if (i >= right.fragments.size()) {
      return PacketValidationResult::INVALID;
    }
    const auto &leftValue = left.fragments[i];
    const auto &rightValue = right.fragments[i];
    if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue)) {
      auto diff = std::get<int>(leftValue) - std::get<int>(rightValue);
      if (diff < 0) {
        return PacketValidationResult::VALID;
      }
      if (diff > 0) {
        return PacketValidationResult::INVALID;
      }
      continue;
    }
    const auto leftPacket = std::holds_alternative<Packet>(leftValue)
                              ? std::get<Packet>(leftValue)
                              : Packet{ std::vector<std::variant<int, Packet>>{ leftValue } };
    const auto rightPacket = std::holds_alternative<Packet>(rightValue)
                               ? std::get<Packet>(rightValue)
                               : Packet{ std::vector<std::variant<int, Packet>>{ rightValue } };
    auto validationResult = validatePackets(leftPacket, rightPacket);
    if (validationResult != PacketValidationResult::UNDEFINED) {
      return validationResult;
    }
  }
  return left.fragments.size() == right.fragments.size() ? PacketValidationResult::UNDEFINED
                                                         : PacketValidationResult::VALID;
}

auto getValidPackets(const auto &input)
{
  auto packets = parseInput(input);
  std::vector<size_t> validPackets;
  for (size_t i = 0; i < packets.size(); ++i) {
    const PacketValidationResult result = validatePackets(packets[i].first, packets[i].second);
    if (result == PacketValidationResult::UNDEFINED) {
      throw std::invalid_argument("cannot determine packet order");
    }
    if (result == PacketValidationResult::VALID) {
      validPackets.push_back(i);
    }
  }
  return validPackets;
}

auto part1(const auto &input)
{
  auto validPackets = getValidPackets(input);
  return std::accumulate(validPackets.begin(), validPackets.end(), static_cast<size_t>(0), std::plus<>())
         + validPackets.size();
}

auto part2(const auto &input)
{
  auto pairs = parseInput(input);
  std::vector<Packet> packets;
  const Packet divider2{ Packet{ { 2 } } };
  const Packet divider6{ Packet{ { 6 } } };
  packets.push_back(divider2);
  packets.push_back(divider6);
  for (const auto &pair : pairs) {
    packets.push_back(pair.first);
    packets.push_back(pair.second);
  }
  std::sort(packets.begin(), packets.end(), [](const Packet &left, const Packet &right) {
    return validatePackets(left, right) == PacketValidationResult::VALID;
  });
  const auto index2 = std::find(packets.begin(), packets.end(), divider2);
  const auto index6 = std::find(packets.begin(), packets.end(), divider6);
  return (std::distance(packets.begin(), index2) + 1) * (std::distance(packets.begin(), index6) + 1);
}

void run()
{
  const auto *taskInput = readTaskInput(13);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level13

#endif// AOC_2022_CPP_LEVEL13_HPP
