//
// Created by object on 23/04/23.
//

#ifndef AOC_2022_CPP_LEVEL25_HPP
#define AOC_2022_CPP_LEVEL25_HPP

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <numeric>
#include <string>

#include "common.hpp"

namespace level25 {

static const auto snafuBase = 5;

auto fromSnafu(const auto &value)
{
  long result = 0;
  for (size_t i = 0; i < value.size(); i++) {
    switch (value[i]) {
      case '2':
        result += static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i)) * 2;
        break;
      case '1':
        result += static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i));
        break;
      case '0':
        break;
      case '-':
        result -= static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i));
        break;
      case '=':
        result -= static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i)) * 2;
        break;
      default:
        throw std::runtime_error("unexpected SNAFU symbol");
    }
  }
  return result;
}

auto toSnafu(auto value)
{
  std::string result;
  while (value > 0) {
    auto digit = value % snafuBase;
    switch (digit) {
      case 0:
        result.append("0");
        break;
      case 1:
        result.append("1");
        break;
      case 2:
        result.append("2");
        break;
      case 3:
        result.append("=");
        break;
      case 4:
        result.append("-");
        break;
      default:
        throw std::runtime_error("unexpected modulus");
    }
    value /= snafuBase;
    if (digit > 2) {
      ++value;
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

auto part1(const auto &input)
{
  const auto lines = splitLines(input);
  auto result = std::accumulate(
    lines.begin(), lines.end(), 0L, [](const auto sum, const auto &line) { return sum + fromSnafu(line); });
  return toSnafu(result);
}

void run()
{
  const auto taskInput = readTaskInput(25);
  std::cout << part1(taskInput) << '\n';
}
} // namespace level25

#endif // AOC_2022_CPP_LEVEL25_HPP
