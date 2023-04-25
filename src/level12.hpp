//
// Created by object on 14/03/23.
//

#ifndef AOC_2022_CPP_LEVEL12_HPP
#define AOC_2022_CPP_LEVEL12_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <exception>
#include <algorithm>

#include "common.hpp"

namespace level12
{

auto parseInput(const auto &input) {
  auto lines = splitLines(input);
  std::vector<std::vector<char>> area;
  for (const auto &line : lines) {
    auto &v = area.emplace_back();
    std::copy(line.begin(), line.end(), std::back_inserter(v));
  }
  return area;
}

auto part1(const auto &input) {
  auto area = parseInput(input);
  std::vector<std::vector<int>> visited(area.size(), std::vector(area[0].size(), -1));
  std::vector<std::pair<size_t, size_t>> queue;
  for (size_t i = 0; i < area.size() && queue.empty(); ++i) {
    for (size_t j = 0; j < area[0].size() && queue.empty(); ++j) {
      if (area[i][j] == 'S') {
        queue.emplace_back(i, j);
        visited[i][j] = 0;
        area[i][j] = 'a';
      }
    }
  }
  if (queue.empty()) {
    throw std::invalid_argument("start not found");
  }
  size_t destinationLine;
  size_t destinationColumn;
  for (size_t i = 0; i < area.size(); ++i) {
    for (size_t j = 0; j < area[0].size(); ++j) {
      if (area[i][j] == 'E') {
        destinationLine = i;
        destinationColumn = j;
        area[i][j] = 'z';
        goto end_found;
      }
    }
  }
  throw std::invalid_argument("end not found");
end_found:
  size_t processed = 0;
  while (processed < queue.size()) {
    auto line = queue[processed].first;
    auto column = queue[processed].second;
    ++processed;
    if (line > 0 && visited[line - 1][column] == -1 && area[line - 1][column] - area[line][column] <= 1) {
      if (line - 1 == destinationLine && column == destinationColumn) {
        return visited[line][column] + 1;
      }
      visited[line - 1][column] = visited[line][column] + 1;
      queue.emplace_back(line - 1, column);
    }
    if (column > 0 && visited[line][column - 1] == -1 && area[line][column - 1] - area[line][column] <= 1) {
      if (line == destinationLine && column - 1 == destinationColumn) {
        return visited[line][column] + 1;
      }
      visited[line][column - 1] = visited[line][column] + 1;
      queue.emplace_back(line, column - 1);
    }
    if (line < area.size() - 1 && visited[line + 1][column] == -1 && area[line + 1][column] - area[line][column] <= 1) {
      if (line + 1 == destinationLine && column == destinationColumn) {
        return visited[line][column] + 1;
      }
      visited[line + 1][column] = visited[line][column] + 1;
      queue.emplace_back(line + 1, column);
    }
    if (column < area[0].size() - 1 && visited[line][column + 1] == -1 && area[line][column + 1] - area[line][column] <= 1) {
      if (line == destinationLine && column + 1 == destinationColumn) {
        return visited[line][column] + 1;
      }
      visited[line][column + 1] = visited[line][column] + 1;
      queue.emplace_back(line, column + 1);
    }
  }
  throw std::invalid_argument("cannot reach destination");
}

auto part2(const auto &input) {
  auto area = parseInput(input);
  std::vector<std::vector<int>> visited(area.size(), std::vector(area[0].size(), -1));
  std::vector<std::pair<size_t, size_t>> queue;
  for (size_t i = 0; i < area.size(); ++i) {
    for (size_t j = 0; j < area[0].size(); ++j) {
      if (area[i][j] == 'S' || area[i][j] == 'a') {
        queue.emplace_back(i, j);
        visited[i][j] = 0;
        area[i][j] = 'a';
      }
    }
  }
  if (queue.empty()) {
    throw std::invalid_argument("start not found");
  }
  size_t destinationLine;
  size_t destinationColumn;
  for (size_t i = 0; i < area.size(); ++i) {
    for (size_t j = 0; j < area[0].size(); ++j) {
      if (area[i][j] == 'E') {
        destinationLine = i;
        destinationColumn = j;
        area[i][j] = 'z';
        goto end_found;
      }
    }
  }
  throw std::invalid_argument("end not found");
end_found:
  size_t processed = 0;
  while (processed < queue.size()) {
    auto line = queue[processed].first;
    auto column = queue[processed].second;
    ++processed;
    if (line > 0 && area[line - 1][column] - area[line][column] <= 1) {
      if (visited[line - 1][column] == -1 || visited[line - 1][column] > visited[line][column] + 1) {
        visited[line - 1][column] = visited[line][column] + 1;
        queue.emplace_back(line - 1, column);
      }
    }
    if (column > 0 && area[line][column - 1] - area[line][column] <= 1) {
      if (visited[line][column - 1] == -1 || visited[line][column - 1] > visited[line][column] + 1) {
        visited[line][column - 1] = visited[line][column] + 1;
        queue.emplace_back(line, column - 1);
      }
    }
    if (line < area.size() - 1 && area[line + 1][column] - area[line][column] <= 1) {
      if (visited[line + 1][column] == -1 || visited[line + 1][column] > visited[line][column] + 1) {
        visited[line + 1][column] = visited[line][column] + 1;
        queue.emplace_back(line + 1, column);
      }
    }
    if (column < area[0].size() - 1 && area[line][column + 1] - area[line][column] <= 1) {
      if (visited[line][column + 1] == -1 || visited[line][column + 1] > visited[line][column] + 1) {
        visited[line][column + 1] = visited[line][column] + 1;
        queue.emplace_back(line, column + 1);
      }
    }
  }
  return visited[destinationLine][destinationColumn];
}

void run() {
  const auto taskInput = readTaskInput(12);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL12_HPP
