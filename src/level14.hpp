//
// Created by object on 18/03/23.
//

#ifndef AOC_2022_CPP_LEVEL14_HPP
#define AOC_2022_CPP_LEVEL14_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <map>

#include "common.hpp"

namespace level14
{

struct RockSegment {
  int startX; // start is top left
  int startY;
  int endX;
  int endY;

  RockSegment(int x1, int y1, int x2, int y2) : startX(std::min(x1, x2)), startY(std::min(y1, y2)),
                                                endX(std::max(x1, x2)), endY(std::max(y1, y2)) {
  }

  bool isHorizontal() const {
    return startY == endY;
  }

  auto blocks(int x, int y) const {
    if (isHorizontal()) {
      return startY == y && startX <= x && endX >= x;
    }
    return startX == x && startY <= y && endY >= y;
  }
};

auto parseCoordinate(auto &input, const auto &inputEnd) {
  int result = 0;
  while (input != inputEnd && *input >= '0' && *input <= '9') {
    result *= 10;
    result += *input++ - '0';
  }
  return result;
}

auto parseInput(const auto &input) {
  auto lines = splitLines(input);
  std::vector<RockSegment> result;
  for (const auto &line : lines) {
    auto start = line.begin();
    auto end = line.end();
    auto startX = parseCoordinate(start, end);
    ++start;
    auto startY = parseCoordinate(start, end);
    while (start != end) {
      start += std::strlen(" -> ");
      auto endX = parseCoordinate(start, end);
      ++start;
      auto endY = parseCoordinate(start, end);
      result.emplace_back(startX, startY, endX, endY);
      startX = endX;
      startY = endY;
    }
  }
  return result;
}

auto part1(const auto &input) {
  auto rocks = parseInput(input);
  std::set<std::pair<int, int>> restingSand;
  const auto caveBottom = std::max_element(rocks.begin(), rocks.end(), [](const auto &left, const auto &right) {
    return left.endY < right.endY;
  })->endY;
  auto blocked = [&](int x, int y) {
    if (restingSand.contains({x, y})) {
      return true;
    }
    return std::any_of(rocks.begin(), rocks.end(), [&](const auto &rock) {
      return rock.blocks(x, y);
    });
  };
  auto sandCount{0};
  while (true) {
    auto sandX = 500;
    auto sandY = 0;
    while (true) {
      if (sandY == caveBottom) {
        return sandCount;
      }
      if (!blocked(sandX, sandY + 1)) {
        ++sandY;
      } else if (!blocked(sandX - 1, sandY + 1)) {
        --sandX;
        ++sandY;
      } else if (!blocked(sandX + 1, sandY + 1)) {
        ++sandX;
        ++sandY;
      } else {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
    }
  }
}

auto part2(const auto &input) {
  const auto rocks = parseInput(input);
  std::map<int, std::vector<RockSegment>> horizontalRocks;
  std::map<int, std::vector<RockSegment>> verticalRocks;
  for (const auto &rock : rocks) {
    if (rock.isHorizontal()) {
      horizontalRocks[rock.startY].push_back(rock);
    } else {
      verticalRocks[rock.startX].push_back(rock);
    }
  }
  std::set<std::pair<int, int>> restingSand;
  const auto caveBottom = std::max_element(rocks.begin(), rocks.end(), [](const auto &left, const auto &right) {
    return left.endY < right.endY;
  })->endY;
  auto checkBlocked = [&](int x, int y) {
    if (restingSand.contains({x, y})) {
      return true;
    }
    if (horizontalRocks.contains(y)) {
      auto areaBlocked = std::any_of(horizontalRocks[y].begin(), horizontalRocks[y].end(), [&](const auto &rock) {
        return rock.blocks(x, y);
      });
      if (areaBlocked) {
        return true;
      }
    }
    if (verticalRocks.contains(x)) {
      return std::any_of(verticalRocks[x].begin(), verticalRocks[x].end(), [&](const auto &rock) {
        return rock.blocks(x, y);
      });
    }
    return false;
  };
  auto sandCount{0};
  while (true) {
    auto sandX = 500;
    auto sandY = 0;
    if (restingSand.contains({sandX, sandY})) {
      break;
    }
    while (true) {
      if (sandY == caveBottom + 1) {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
      if (!checkBlocked(sandX, sandY + 1)) {
        ++sandY;
      } else if (!checkBlocked(sandX - 1, sandY + 1)) {
        --sandX;
        ++sandY;
      } else if (!checkBlocked(sandX + 1, sandY + 1)) {
        ++sandX;
        ++sandY;
      } else {
        restingSand.emplace(sandX, sandY);
        ++sandCount;
        break;
      }
    }
  }
  return sandCount;
}

static const auto taskInput = R"(508,146 -> 513,146
514,142 -> 519,142
483,88 -> 487,88
522,146 -> 527,146
492,90 -> 496,90
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
486,86 -> 490,86
489,84 -> 493,84
507,79 -> 511,79
486,90 -> 490,90
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
494,26 -> 494,27 -> 505,27
485,113 -> 489,113
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
515,53 -> 515,54 -> 524,54
498,76 -> 502,76
515,53 -> 515,54 -> 524,54
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
495,88 -> 499,88
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
497,103 -> 497,104 -> 513,104 -> 513,103
505,31 -> 513,31 -> 513,30
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
497,109 -> 501,109
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
501,79 -> 505,79
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
510,76 -> 514,76
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
513,79 -> 517,79
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
491,109 -> 495,109
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
492,82 -> 496,82
515,146 -> 520,146
498,90 -> 502,90
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
498,86 -> 502,86
491,113 -> 495,113
512,148 -> 517,148
511,144 -> 516,144
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
503,113 -> 507,113
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
505,148 -> 510,148
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
501,88 -> 505,88
504,70 -> 508,70
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
489,88 -> 493,88
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
494,107 -> 498,107
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
495,79 -> 499,79
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
501,73 -> 505,73
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
494,111 -> 498,111
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
497,113 -> 501,113
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
507,73 -> 511,73
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
526,148 -> 531,148
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
488,111 -> 492,111
508,93 -> 508,97 -> 507,97 -> 507,101 -> 517,101 -> 517,97 -> 510,97 -> 510,93
497,103 -> 497,104 -> 513,104 -> 513,103
497,103 -> 497,104 -> 513,104 -> 513,103
497,23 -> 497,15 -> 497,23 -> 499,23 -> 499,21 -> 499,23 -> 501,23 -> 501,15 -> 501,23
519,148 -> 524,148
505,31 -> 513,31 -> 513,30
495,84 -> 499,84
480,90 -> 484,90
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
518,144 -> 523,144
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
494,26 -> 494,27 -> 505,27
503,34 -> 503,37 -> 495,37 -> 495,40 -> 512,40 -> 512,37 -> 505,37 -> 505,34
500,111 -> 504,111
504,76 -> 508,76
517,116 -> 517,119 -> 516,119 -> 516,126 -> 527,126 -> 527,119 -> 520,119 -> 520,116
512,139 -> 512,132 -> 512,139 -> 514,139 -> 514,136 -> 514,139 -> 516,139 -> 516,135 -> 516,139
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
504,90 -> 508,90
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
511,43 -> 511,46 -> 510,46 -> 510,50 -> 516,50 -> 516,46 -> 515,46 -> 515,43
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
507,67 -> 507,63 -> 507,67 -> 509,67 -> 509,61 -> 509,67 -> 511,67 -> 511,66 -> 511,67 -> 513,67 -> 513,60 -> 513,67 -> 515,67 -> 515,62 -> 515,67 -> 517,67 -> 517,62 -> 517,67 -> 519,67 -> 519,59 -> 519,67
525,161 -> 525,156 -> 525,161 -> 527,161 -> 527,156 -> 527,161 -> 529,161 -> 529,154 -> 529,161 -> 531,161 -> 531,154 -> 531,161 -> 533,161 -> 533,157 -> 533,161 -> 535,161 -> 535,152 -> 535,161 -> 537,161 -> 537,160 -> 537,161
492,86 -> 496,86)";

void run() {
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL14_HPP
