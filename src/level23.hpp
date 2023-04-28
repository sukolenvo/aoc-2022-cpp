//
// Created by object on 19/04/23.
//

#ifndef AOC_2022_CPP_LEVEL23_HPP
#define AOC_2022_CPP_LEVEL23_HPP

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level23 {

struct Elf {
  size_t id;
  int x;
  int y;
  Elf(size_t id_, int x_, int y_): id(id_), x(x_), y(y_) {

  }
};

auto moveElves(const auto &input, size_t rounds) {
  const auto lines = splitLines(input);
  std::vector<Elf> elves;
  for (size_t i = 0; i < lines.size(); ++i) {
    for (size_t j = 0; j < lines[0].size(); ++j) {
      if (lines[i][j] == '#') {
        elves.emplace_back(elves.size(), static_cast<int>(j), static_cast<int>(i));
      }
    }
  }
  const auto isOccupied = [&](int x, int y) {
    return std::any_of(elves.begin(), elves.end(), [&](const auto &elf) {
      return elf.x == x && elf.y == y;
    });
  };
  const auto hasNorthNeighbour = [&](int x, int y) {
    return isOccupied(x, y - 1) || isOccupied(x - 1, y - 1) || isOccupied(x + 1, y - 1);
  };
  const auto hasSouthNeighbour = [&](int x, int y) {
    return isOccupied(x, y + 1) || isOccupied(x - 1, y + 1) || isOccupied(x + 1, y + 1);
  };
  const auto hasWestNeighbour = [&](int x, int y) {
    return isOccupied(x - 1, y) || isOccupied(x - 1, y - 1) || isOccupied(x - 1, y + 1);
  };
  const auto hasEastNeighbour = [&](int x, int y) {
    return isOccupied(x + 1, y) || isOccupied(x + 1, y - 1) || isOccupied(x + 1, y + 1);
  };
  const auto hasNeighbour = [&](int x, int y) {
    return hasNorthNeighbour(x, y) || hasSouthNeighbour(x, y) || hasWestNeighbour(x, y) || hasEastNeighbour(x, y);
  };
  const std::array<std::function<bool(int, int)>, 4> neighbourChecks = { hasNorthNeighbour, hasSouthNeighbour, hasWestNeighbour, hasEastNeighbour };
  const std::array<std::array<int, 2>, 4> proposedMove { { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } } };
  for (size_t r = 0; r < rounds; ++r) {
    static const auto hashFunction = [](const std::pair<int, int> &key) {
      return std::hash<int>()(key.first);
    };
    std::unordered_map<std::pair<int, int>, std::vector<size_t>, decltype(hashFunction)> proposedPositions;
    for (const auto &elf : elves) {
      if (!hasNeighbour(elf.x, elf.y)) {
        continue;
      }
      for (size_t i = 0; i < neighbourChecks.size(); ++i) {
        const auto normalisedStep = (i + r) % neighbourChecks.size();
        if (!neighbourChecks[normalisedStep](elf.x, elf.y)) {
          auto newX = elf.x + proposedMove[normalisedStep][0];
          auto newY = elf.y + proposedMove[normalisedStep][1];
          proposedPositions[{newX, newY}].push_back(elf.id);
          break;
        }
      }
    }
    for (const auto&[position, ids] : proposedPositions) {
      if (ids.size() == 1) {
        const auto id = ids[0];
        auto element = std::find_if(elves.begin(), elves.end(), [&](const auto &elf) {
          return elf.id == id;
        });
        element->x = position.first;
        element->y = position.second;
      }
    }
  }
  return elves;
}

auto part1(const auto &input) {
  const auto elves = moveElves(input, 10);
  const auto minX = std::min_element(elves.begin(), elves.end(), [](const auto &left, const auto &right) {
    return left.x < right.x;
  })->x;
  const auto maxX = std::max_element(elves.begin(), elves.end(), [](const auto &left, const auto &right) {
    return left.x < right.x;
  })->x;
  const auto minY = std::min_element(elves.begin(), elves.end(), [](const auto &left, const auto &right) {
    return left.y < right.y;
  })->y;
  const auto maxY = std::max_element(elves.begin(), elves.end(), [](const auto &left, const auto &right) {
    return left.y < right.y;
  })->y;
  return (maxX - minX + 1) * (maxY - minY + 1) - static_cast<int>(elves.size());
}

auto part2(const auto &input) {
  const auto lines = splitLines(input);
  static const auto padding = 1000;
  std::vector<std::pair<int, int>> elves;
  std::vector<std::vector<size_t>> map{lines.size() + padding * 2, std::vector<size_t>(lines[0].size() + padding * 2)};
  for (size_t i = 0; i < lines.size(); ++i) {
    for (size_t j = 0; j < lines[0].size(); ++j) {
      if (lines[i][j] == '#') {
        elves.emplace_back(static_cast<int>(j + padding), static_cast<int>(i + padding));
        map[i + padding][j + padding] = elves.size();
      }
    }
  }
  const auto isOccupied = [&](int x, int y) {
    return map[static_cast<size_t>(y)][static_cast<size_t>(x)] > 0;
  };
  const auto hasNorthNeighbour = [&](int x, int y) {
    return isOccupied(x, y - 1) || isOccupied(x - 1, y - 1) || isOccupied(x + 1, y - 1);
  };
  const auto hasSouthNeighbour = [&](int x, int y) {
    return isOccupied(x, y + 1) || isOccupied(x - 1, y + 1) || isOccupied(x + 1, y + 1);
  };
  const auto hasWestNeighbour = [&](int x, int y) {
    return isOccupied(x - 1, y) || isOccupied(x - 1, y - 1) || isOccupied(x - 1, y + 1);
  };
  const auto hasEastNeighbour = [&](int x, int y) {
    return isOccupied(x + 1, y) || isOccupied(x + 1, y - 1) || isOccupied(x + 1, y + 1);
  };
  const auto hasNeighbour = [&](int x, int y) {
    return hasNorthNeighbour(x, y) || hasSouthNeighbour(x, y) || hasWestNeighbour(x, y) || hasEastNeighbour(x, y);
  };
  const std::array<std::function<bool(int, int)>, 4> neighbourChecks = { hasNorthNeighbour, hasSouthNeighbour, hasWestNeighbour, hasEastNeighbour };
  const std::array<std::array<int, 2>, 4> proposedMove { { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } } };
  for (size_t r = 0; ; ++r) {
    static const auto hashFunction = [](const std::pair<int, int> &key) {
      return std::hash<int>()(key.first);
    };
    std::unordered_map<std::pair<int, int>, std::vector<size_t>, decltype(hashFunction)> proposedMoves;
    for (size_t e = 0; e < elves.size(); ++e) {
      const auto[x, y] = elves[e];
      if (!hasNeighbour(x, y)) {
        continue;
      }
      for (size_t i = 0; i < neighbourChecks.size(); ++i) {
        const auto normalisedStep = (i + r) % neighbourChecks.size();
        if (!neighbourChecks[normalisedStep](x, y)) {
          auto newX = x + proposedMove[normalisedStep][0];
          auto newY = y + proposedMove[normalisedStep][1];
          proposedMoves[{ newX, newY }].emplace_back(e + 1);
          break;
        }
      }
    }
    if (proposedMoves.size() == 0) {
      return r + 1;
    }
    for (const auto&[to, ids] : proposedMoves) {
      if (ids.size() == 1) {
        auto &elf = elves[ids[0] - 1];
        map[static_cast<size_t>(elf.second)][static_cast<size_t>(elf.first)] = 0;
        map[static_cast<size_t>(to.second)][static_cast<size_t>(to.first)] = ids[0];
        elf.first = to.first;
        elf.second = to.second;
      }
    }
  }
}

void run() {
  const auto taskInput = readTaskInput(23);
//  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL23_HPP
