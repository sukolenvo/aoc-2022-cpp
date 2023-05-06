//
// Created by object on 11/03/23.
//

#ifndef AOC_2022_CPP_LEVEL11_HPP
#define AOC_2022_CPP_LEVEL11_HPP

#include <algorithm>
#include <cstring>
#include <exception>
#include <functional>
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

#include "common.hpp"

namespace level11 {

class Monkey
{
  std::vector<size_t> items;
  std::function<size_t(size_t)> operation;
  int testDivider;
  size_t successMonkey;
  size_t failedMonkey;
  size_t inspections{ 0 };
  size_t reliefFactor{ 3 };
  size_t normaliseFactor{ 0 };

public:
  Monkey(const std::vector<size_t> &items_,
    const std::function<size_t(size_t)> &operation_,
    int testDivider_,
    size_t successMonkey_,
    size_t failedMonkey_)
    : items(items_), operation(operation_), testDivider(testDivider_), successMonkey(successMonkey_),
      failedMonkey(failedMonkey_)
  {}
  auto getTestDivide() const
  {
    return testDivider;
  }
  auto getSuccessMonkey() const
  {
    return successMonkey;
  }
  auto getFailedMonkey() const
  {
    return failedMonkey;
  }
  auto &getItems()
  {
    return items;
  }
  auto getInspections() const
  {
    return inspections;
  }
  void setReliefFactor(const size_t &value)
  {
    this->reliefFactor = value;
  }
  void setNormaliseFactor(const size_t &value)
  {
    this->normaliseFactor = value;
  }
  void inspectItems(std::vector<Monkey> &monkeys)
  {
    for (const auto item : items) {
      auto newLevel = operation(item) / reliefFactor;
      auto &destination =
        newLevel % static_cast<size_t>(testDivider) == 0 ? monkeys.at(successMonkey) : monkeys.at(failedMonkey);
      destination.getItems().push_back(newLevel % normaliseFactor);
      ++inspections;
    }
    items.clear();
  }
};

auto readDigits(auto &start, const auto &end)
{
  size_t result = 0;
  while (start != end && *start >= '0' && *start <= '9') {
    result *= 10;
    result += static_cast<size_t>(*start++ - '0');
  }
  return result;
}

auto buildOperation(auto &start, const auto &end)
{
  auto leftIsItem = static_cast<size_t>(*start == 'o');
  size_t leftValue = 0;
  if (leftIsItem) {
    start += 4;
  } else {
    leftValue = readDigits(start, end);
    ++start;
  }
  std::function<size_t(size_t, size_t)> operation;
  if (*start == '+') {
    operation = [](size_t left, size_t right) { return left + right; };
  } else if (*start == '*') {
    operation = [](size_t left, size_t right) { return left * right; };
  } else {
    throw std::invalid_argument("unexpected operation");
  }
  start += 2;
  auto rightIsItem = static_cast<size_t>(*start == 'o');
  size_t rightValue = 0;
  if (rightIsItem) {
    start += 4;
  } else {
    rightValue = readDigits(start, end);
    ++start;
  }
  return [=](size_t item) {
    const size_t left = leftIsItem ? item : leftValue;
    const size_t right = rightIsItem ? item : rightValue;
    return operation(left, right);
  };
}

auto parseInput(const auto &taskInput)
{
  std::vector<Monkey> result{};
  auto input = std::string_view{ taskInput };
  for (auto start = input.begin(); start != input.end();) {
    while (*start == '\n') {
      ++start; // skip separator line
    }
    while (*start++ != '\n')
      ; // skip monkey index line
    start += std::strlen("  Starting items: ");
    std::vector<size_t> items;
    while (*start != '\n') {
      items.push_back(readDigits(start, input.end()));
      if (*start == ',') {
        start += 2;
      }
    }
    ++start; // newline
    start += std::strlen("  Operation: new = ");
    const auto operation = buildOperation(start, input.end());
    start += std::strlen("  Test: divisible by ");
    auto testDivider = static_cast<int>(readDigits(start, input.end()));
    ++start; // newline
    start += std::strlen("    If true: throw to monkey ");
    size_t successMonkey = readDigits(start, input.end());
    ++start; // newline
    start += std::strlen("    If false: throw to monkey ");
    size_t failedMonkey = readDigits(start, input.end());
    result.emplace_back(items, operation, testDivider, successMonkey, failedMonkey);
  }
  auto normaliseFactor = std::accumulate(
    begin(result), end(result), 1, [](const auto &left, const Monkey &right) { return left * right.getTestDivide(); });
  for (auto &monkey : result) {
    monkey.setNormaliseFactor(static_cast<const size_t>(normaliseFactor));
  }
  return result;
}

auto part1(const auto &taskInput)
{
  auto monkeys = parseInput(taskInput);
  const auto rounds = 20;
  for (size_t i = 0; i < rounds; ++i) {
    for (auto &monkey : monkeys) {
      monkey.inspectItems(monkeys);
    }
  }
  std::sort(begin(monkeys), end(monkeys), [](const auto &left, const auto &right) {
    return left.getInspections() > right.getInspections();
  });
  return monkeys[0].getInspections() * monkeys[1].getInspections();
}

auto part2(const auto &taskInput)
{
  auto monkeys = parseInput(taskInput);
  for (auto &monkey : monkeys) {
    monkey.setReliefFactor(1);
  }
  const auto rounds = 10000;
  for (size_t i = 0; i < rounds; ++i) {
    for (auto &monkey : monkeys) {
      monkey.inspectItems(monkeys);
    }
  }
  std::sort(begin(monkeys), end(monkeys), [](const auto &left, const auto &right) {
    return left.getInspections() > right.getInspections();
  });
  return monkeys[0].getInspections() * monkeys[1].getInspections();
}

void run()
{
  const auto taskInput = readTaskInput(11);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
} // namespace level11

#endif // AOC_2022_CPP_LEVEL11_HPP
