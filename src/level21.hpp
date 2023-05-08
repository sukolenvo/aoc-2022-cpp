//
// Created by object on 13/04/23.
//

#ifndef AOC_2022_CPP_LEVEL21_HPP
#define AOC_2022_CPP_LEVEL21_HPP

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "common.hpp"

namespace level21 {

auto parseInput(const auto &input)
{
  const auto lines = splitLines(input);
  std::unordered_map<std::string, std::string> context;
  for (const auto &line : lines) {
    context[std::string{ line.begin(), line.begin() + 4 }] = std::string{ line.begin() + 6, line.end() };
  }
  return context;
}

std::function<long(long, long)> buildOperation(const auto operation)
{
  switch (operation) {
    case '+':
      return [](auto left, auto right) { return left + right; };
    case '-':
      return [](auto left, auto right) { return left - right; };
    case '*':
      return [](auto left, auto right) { return left * right; };
    case '/':
      return [](auto left, auto right) { return left / right; };
    default:
      throw std::runtime_error("unexpected operation");
  }
}

auto eval(const auto &evalContext, const auto &var)
{
  const auto value = evalContext.at(var);
  if (value[0] >= '0' && value[0] <= '9') {
    return static_cast<long>(readNumber(value.begin(), value.end()));
  }
  return buildOperation(*(value.begin() + 5))(eval(evalContext, std::string{ value.begin(), value.begin() + 4 }),
    eval(evalContext, std::string(value.begin() + 7, value.end())));
}

auto part1(const auto &input)
{
  auto evalContext = parseInput(input);
  return eval(evalContext, std::string{ "root" });
}

auto checkIsMyInput(const auto &context, const auto &var)
{
  if (var == "humn") {
    return true;
  }
  const auto value = context.at(var);
  if (value[0] >= '0' && value[0] <= '9') {
    return false;
  }
  return checkIsMyInput(context, std::string{ value.begin(), value.begin() + 4 })
         || checkIsMyInput(context, std::string(value.begin() + 7, value.end()));
}

auto oppositeOperation(auto operation, auto isLeft)
{
  switch (operation) {
    case '+':
      return buildOperation('-');
    case '-':
      return isLeft ? buildOperation('+') : [](auto left, auto right) { return right - left; };
    case '*':
      return buildOperation('/');
    case '/':
      return buildOperation(isLeft ? '*' : '/');
    default:
      throw std::runtime_error("unexpected operation");
  }
}

auto evalHumn(const auto &context, const auto &key, auto expressionResult)
{
  if (key == "humn") {
    return expressionResult;
  }
  const auto valueLine = context.at(key);
  if (valueLine.length() != 11) {
    throw std::runtime_error("unexpected value line format. expecting operation.");
  }
  const auto left = std::string{ valueLine.begin(), valueLine.begin() + 4 };
  const auto right = std::string{ valueLine.begin() + 7, valueLine.end() };
  auto leftIsMyInput = checkIsMyInput(context, left);
  auto rightIsMyInput = checkIsMyInput(context, right);
  if (leftIsMyInput == rightIsMyInput) {
    throw std::runtime_error("unsuppported scenario");
  }
  auto constantPart = eval(context, leftIsMyInput ? right : left);
  auto operation = oppositeOperation(*(valueLine.begin() + 5), leftIsMyInput);
  return evalHumn(context, leftIsMyInput ? left : right, operation(expressionResult, constantPart));
}

auto part2(const auto &input)
{
  auto evalContext = parseInput(input);
  const auto rootLine = evalContext.at(std::string{ "root" });
  if (rootLine.length() != 11) {
    throw std::runtime_error("unexpected root line format. expecting operation.");
  }
  const auto left = std::string{ rootLine.begin(), rootLine.begin() + 4 };
  const auto right = std::string{ rootLine.begin() + 7, rootLine.end() };
  auto leftIsMyInput = checkIsMyInput(evalContext, left);
  auto rightIsMyInput = checkIsMyInput(evalContext, right);
  if (leftIsMyInput == rightIsMyInput) {
    throw std::runtime_error("unsuppported scenario");
  }
  auto value = eval(evalContext, leftIsMyInput ? right : left);
  return evalHumn(evalContext, leftIsMyInput ? left : right, value);
}

void run()
{
  const auto taskInput = readTaskInput(21);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
} // namespace level21

#endif // AOC_2022_CPP_LEVEL21_HPP
