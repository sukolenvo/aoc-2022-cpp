//
// Created by object on 14/04/23.
//

#ifndef AOC_2022_CPP_LEVEL22_HPP
#define AOC_2022_CPP_LEVEL22_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <string_view>
#include <utility>
#include <vector>

#include "common.hpp"

namespace level22 {

enum class Direction { right = 0, down = 1, left = 2, up = 3 };

auto readDigits(auto &start, const auto &end)
{
  int result{};
  while (start != end && *start >= '0' && *start <= '9') {
    result *= 10;
    result += *start++ - '0';
  }
  return result;
}

auto getNextPosition(auto x, auto y, auto direction)
{
  switch (direction) {
    case Direction::right:
      return std::pair{ x + 1, y };
    case Direction::left:
      return std::pair{ x - 1, y };
    case Direction::up:
      return std::pair{ x, y - 1 };
    case Direction::down:
      return std::pair{ x, y + 1 };
    default:
      throw std::runtime_error("unexpected direction");
  }
}

auto turn(auto direction, auto side)
{
  if (side == 'L') {
    return static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
  }
  if (side != 'R') {
    throw std::runtime_error("unexpected side");
  }
  return static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
}

auto walk(const auto &input, auto handleOutOfBounds)
{
  auto lines = splitLines(input);
  auto instructions = lines.back();
  lines.erase(lines.end() - 2, lines.end());
  auto x = static_cast<int>(std::distance(lines[0].begin(), std::find(lines[0].begin(), lines[0].end(), '.')));
  int y = 0;
  auto direction = Direction::right;
  auto instruction = instructions.begin();
  while (instruction != instructions.end()) {
    auto distance = readDigits(instruction, instructions.end());
    for (auto i = 0; i < distance; ++i) {
      auto nextPosition = getNextPosition(x, y, direction);
      auto nextDirection = direction;
      auto outOfBounds =
        nextPosition.first < 0 || nextPosition.second < 0 || lines.size() == static_cast<size_t>(nextPosition.second)
        || lines[static_cast<size_t>(nextPosition.second)].size() <= static_cast<size_t>(nextPosition.first)
        || lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] == ' ';
      if (outOfBounds) {
        handleOutOfBounds(nextDirection, nextPosition, lines);
      }
      if (lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] == '#') {
        break;
      }
      if (lines[static_cast<size_t>(nextPosition.second)][static_cast<size_t>(nextPosition.first)] != '.') {
        throw std::runtime_error("unexpected tile");
      }
      x = nextPosition.first;
      y = nextPosition.second;
      direction = nextDirection;
    }
    if (instruction != instructions.end()) {
      direction = turn(direction, *instruction++);
    }
  }
  return 1000 * ++y + 4 * ++x + static_cast<int>(direction);
}

auto part1(const auto &input)
{
  return walk(input, [](auto &direction, auto &nextPosition, const auto &lines) {
    switch (direction) {
      case Direction::right:
        nextPosition.first = static_cast<int>(std::distance(lines[static_cast<size_t>(nextPosition.second)].begin(),
          std::find_if(lines[static_cast<size_t>(nextPosition.second)].begin(),
            lines[static_cast<size_t>(nextPosition.second)].end(),
            [](const auto &tile) { return tile != ' '; })));
        break;
      case Direction::down:
        nextPosition.second =
          static_cast<int>(std::distance(lines.begin(), std::find_if(lines.begin(), lines.end(), [&](const auto &line) {
            return line.size() > static_cast<size_t>(nextPosition.first)
                   && line[static_cast<size_t>(nextPosition.first)] != ' ';
          })));
        break;
      case Direction::left:
        nextPosition.first = static_cast<int>(lines[static_cast<size_t>(nextPosition.second)].size() - 1);
        break;
      case Direction::up:
        nextPosition.second =
          static_cast<int>(
            std::distance(std::find_if(lines.rbegin(),
                            lines.rend(),
                            [&](const auto &line) { return line.size() > static_cast<size_t>(nextPosition.first); }),
              lines.rend()))
          - 1;
        break;
    }
  });
}

void part2handleOOBsample(Direction &direction,
  std::pair<int, int> &nextPosition,
  const std::vector<std::string_view> &lines)
{
  auto cubeSize = static_cast<int>(lines.size() / 3);
  if (direction == Direction::right) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize * 4 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = cubeSize * 4 - 1 - nextPosition.second + cubeSize;
      nextPosition.second = cubeSize * 2;
      direction = Direction::down;
    } else {
      nextPosition.first = cubeSize * 3 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    }
  } else if (direction == Direction::down) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = cubeSize * 2 + cubeSize * 2 - nextPosition.first - 1;
      nextPosition.first = cubeSize * 2;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 3) {
      nextPosition.second = cubeSize * 2 - 1;
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      direction = Direction::up;
    } else {
      nextPosition.second = cubeSize + cubeSize * 4 - 1 - nextPosition.first;
      nextPosition.first = 0;
      direction = Direction::right;
    }
  } else if (direction == Direction::left) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize + nextPosition.second;
      nextPosition.second = cubeSize;
      direction = Direction::down;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = cubeSize * 4 - nextPosition.second + cubeSize - 1;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    } else {
      nextPosition.first = cubeSize * 2 - 1 - nextPosition.second + cubeSize * 2;
      nextPosition.second = cubeSize * 2 - 1;
      direction = Direction::up;
    }
  } else if (direction == Direction::up) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      nextPosition.second = 0;
      direction = Direction::down;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = nextPosition.first - cubeSize;
      nextPosition.first = cubeSize * 2;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 3) {
      nextPosition.second = cubeSize;
      nextPosition.first = cubeSize * 3 - 1 - nextPosition.first;
      direction = Direction::down;
    } else {
      nextPosition.second = cubeSize + cubeSize * 4 - 1 - nextPosition.first;
      nextPosition.first = cubeSize * 3 - 1;
      direction = Direction::left;
    }
  } else {
    throw std::runtime_error("unexpected direction");
  }
}

void part2handleOOBtask(Direction &direction,
  std::pair<int, int> &nextPosition,
  const std::vector<std::string_view> &lines)
{
  const auto cubeSize = static_cast<int>(lines.size() / 4);
  if (direction == Direction::right) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = cubeSize * 2 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = nextPosition.second + cubeSize;
      nextPosition.second = cubeSize - 1;
      direction = Direction::up;
    } else if (nextPosition.second < cubeSize * 3) {
      nextPosition.first = cubeSize * 3 - 1;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::left;
    } else {
      nextPosition.first = nextPosition.second - cubeSize * 2;
      nextPosition.second = cubeSize * 3 - 1;
      direction = Direction::up;
    }
  } else if (direction == Direction::down) {
    if (nextPosition.first < cubeSize) {
      nextPosition.first = cubeSize * 2 + nextPosition.first;
      nextPosition.second = 0;
      direction = Direction::down;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = cubeSize * 2 + nextPosition.first;
      nextPosition.first = cubeSize - 1;
      direction = Direction::left;
    } else {
      nextPosition.second = nextPosition.first - cubeSize;
      nextPosition.first = cubeSize * 2 - 1;
      direction = Direction::left;
    }
  } else if (direction == Direction::left) {
    if (nextPosition.second < cubeSize) {
      nextPosition.first = 0;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::right;
    } else if (nextPosition.second < cubeSize * 2) {
      nextPosition.first = nextPosition.second - cubeSize;
      nextPosition.second = cubeSize * 2;
      direction = Direction::down;
    } else if (nextPosition.second < cubeSize * 3) {
      nextPosition.first = cubeSize;
      nextPosition.second = cubeSize * 3 - 1 - nextPosition.second;
      direction = Direction::right;
    } else {
      nextPosition.first = nextPosition.second - cubeSize * 2;
      nextPosition.second = 0;
      direction = Direction::down;
    }
  } else if (direction == Direction::up) {
    if (nextPosition.first < cubeSize) {
      nextPosition.second = nextPosition.first + cubeSize;
      nextPosition.first = cubeSize;
      direction = Direction::right;
    } else if (nextPosition.first < cubeSize * 2) {
      nextPosition.second = nextPosition.first + cubeSize * 2;
      nextPosition.first = 0;
      direction = Direction::right;
    } else {
      nextPosition.second = cubeSize * 4 - 1;
      nextPosition.first = nextPosition.first - cubeSize * 2;
      direction = Direction::up;
    }
  } else {
    throw std::runtime_error("unexpected direction");
  }
}

auto readFoldingPattern(const auto &input)
{
  auto lines = splitLines(input);
  lines.erase(lines.end() - 2, lines.end());
  const auto surfaceArea = std::accumulate(lines.begin(), lines.end(), 0, [](int sum, const auto &line) {
    return sum + std::accumulate(line.begin(), line.end(), 0, [](int lineSum, const auto &c) {
      return lineSum + (c == ' ' ? 0 : 1);
    });
  });
  const auto cubeSize = static_cast<size_t>(std::sqrt(surfaceArea / 6));
  std::string pattern;
  for (size_t y = 0; y < lines.size(); y += cubeSize) {
    for (size_t x = 0; x < lines[y].size(); x += cubeSize) {
      pattern.append(lines[y][x] == ' ' ? " " : ".");
    }
    pattern.append("\n");
  }
  return pattern;
}

auto part2(const auto &input)
{
  const auto pattern = readFoldingPattern(input);
  if (pattern == "  .\n...\n  ..\n") {
    return walk(input, &part2handleOOBsample);
  }
  if (pattern == " ..\n .\n..\n.\n") {
    return walk(input, &part2handleOOBtask);
  }
  throw std::runtime_error(pattern + ": pattern is not supported");
}

static const auto taskInput =
  R"(                                                  .#..#......#...................................................#..#............#...................#
                                                  ....#.........#.........##...............##..............#............#.##......#............#......
                                                  .............#................#.##....##.........#..#.............#...#............#................
                                                  ...........#...........#...#...........................#.#...#.#.#...#...........#................#.
                                                  ..#..................................#...............#....#...........#.#...#......#................
                                                  ......#.................#...................#.......#...............#........#......................
                                                  ...........##..............................##.......##.......#..##..........#..........#......#..#..
                                                  ..........#....#.#..........#..........#......#.#............................................#......
                                                  ........#......................#...................#....#....#........#.#...........................
                                                  .........................#....#....#.#...................#.........#.........#.........#............
                                                  .........................##.......#...#.........#.......#.....##.....#..#......#..................#.
                                                  ..........##....#.........#.##.........................#.....#...#.#......................#.........
                                                  ......#.....#............#.......#..#.....#....#.....#..........#.....#.............#..#....#....#..
                                                  #.................................#..................#.....#..#........#.................#..........
                                                  #................#....##..........................#.....#.....................#......#........#....#
                                                  .................#....##.#.#.....#...........#...#....#...#......#........#.................##......
                                                  ...................#...........................#..........#........#..#...........#.................
                                                  .#.................#....................#........#.................#....#.#..............#.........#
                                                  ..#.................#.....#...#.............#...............................#................#.#....
                                                  ......#.#........#.#......................................#..............#........#.........#.......
                                                  ...#.#.##..................#......##......#.......#.#..........................#....................
                                                  ..............#....#........####..........................#...............#............#............
                                                  ..#.............#.....###....................#....................#....#........#...................
                                                  .................#........#.#..#........................................#..............#.#..........
                                                  ....#...................................................#...#...##...#..#...........................
                                                  .......#...#...................#..............#.........#...#....#......#...............#...........
                                                  .............#...........#.#........................#...#...#.....................#.......#....#....
                                                  #............#...#..#........#...................#...........#......#..#.#.........................#
                                                  ...#...............#..#..#.............#.................#....#....#.........#.................#....
                                                  ......#...........##.#...#..#.................................##.#.#..................#..#..........
                                                  ....#........#.........#................................#.............#.....#....#..................
                                                  ......................#..........#.....#.....#.#...........#........................#............###
                                                  #...#..................#....#......#...#........................#..............#........#..#.......#
                                                  ...#............#.............#..##.#.#..#..#.#.........#.............#....#...........#.....#......
                                                  .................##......#.....................#...#....##................#..#.......#....#.........
                                                  ....#..#.........................................#....................................#...#.........
                                                  #....#...................##................#..........#..#........#..#..............#...............
                                                  .................................#..#.............#.................#...#...................#.....#.
                                                  ..#................................#.........#................#..#......................#...........
                                                  ..#..#.............##..........#......................#..............#....#...#.....#....#.....#...#
                                                  ...........##....................##..#......#...........#..............................#..#........#
                                                  ............................#.....................#.................................................
                                                  ....#..........#.#........#...................#........#.....###....................................
                                                  ...#......#..#......#..........#......#....#..............#..........................#..............
                                                  #...............#..............................#..........#......................#..................
                                                  .#...........#..............#..#.........#.......#..........#..............#........................
                                                  .................#...................#........#..#................#.......#........#................
                                                  ..............#.....##.................#...........#...#.....................#.....#....##..........
                                                  .........................#.#................#......#....#............##.......................#.....
                                                  ..#....#....................#..#...#.....#..#...........................#....##..##.......#.....#.#.
                                                  .#.....................#.....#......#.#...#....#.#
                                                  ........#...........#.........#.....#....#........
                                                  ....#...................#....#...#.......#........
                                                  ................##.#........#..#..........#..#....
                                                  ..#...............##...#......#.............#.....
                                                  .#...................#.....#.................#....
                                                  ....#.....................#.....#.....#...........
                                                  #................#...............#.##..#.#........
                                                  ..#...#....#...................................#..
                                                  ................#...............#.................
                                                  .#......................#.....#.#....#............
                                                  .......#..........#..................#..#..#..#..#
                                                  ###..###......#.#..........................#..#...
                                                  .........#.........................##.............
                                                  ..##.........#.............#......................
                                                  ...#..............#....#.#....#........#....##....
                                                  ...........................#....##.........#.#....
                                                  .............#.................#...#.#............
                                                  .............#..#....#..........................#.
                                                  .#.......#................#..##.............#....#
                                                  #..#.......#.................#........#...........
                                                  ....#...........#..............##.#....#..........
                                                  .#.................#.......#...##..........#....#.
                                                  ..................................................
                                                  ..................#.#......#......#.....#.........
                                                  .........#.....#................................#.
                                                  ..#............##.................................
                                                  ......#...#................##.......#.............
                                                  .......#...##..#..#.................#....#..#.....
                                                  ..............##..#..............#................
                                                  .....................#......#...#.................
                                                  ......................#..#.......#.....#.##.......
                                                  ..#................#...................#..........
                                                  .....##..............................#..##..#.....
                                                  .............#..#.......................#.........
                                                  ........#..................................#..#...
                                                  .....#.#........#.#................##.............
                                                  .....#.........................#..#...............
                                                  .#..#...............................#.............
                                                  .........................#.#......................
                                                  .#...............#...#.##...#................#....
                                                  .#.#..........#........#..#...............#.......
                                                  .........###..............#.......................
                                                  .....#...................#..............#.........
                                                  ..#....#.................#................#.....#.
                                                  ......#......#.............#..........#......#.###
                                                  ...................#..............#...#...........
                                                  ..................................................
                                                  #........#.....##.........................#.......
                                                  ..#....#.......#................................#.
..##.................#....................##................#.....#.#.#........#.............#...#..
................#.........#................#........##..............................................
.......................#.##.................#.........#...........#.##...........................#..
.......#.....#.............#..#....#..................................#......#..#............#.....#
............................#.........................##......#............###..###.......#.........
....#......#...........................................#.#................................#...#.....
....#..........#........#..........#.........#........................##...#..............#.....#...
#.............##.....##..........#....###...#..........#..........#........#..............#...#.....
......#...............#......#.........#.....#.#....#......#................#........#......#.......
........................#.#...........#....#..#.........#............#.#....#.....................#.
.......................................#.....#..#...........#.........#...............#...#.........
.................#........#.........#...##...#.....#..........#.....#...............................
.......#........#..#..#..#......#........#.#........#.#...#........#.........#........##.#..........
...#..#..#.......##.............................#....#.#......#.....................................
.......#.#................................#.........#....#.....#....#.....#................#........
.....#..............##..#........#.........#..................#..........#....#.....................
............#.....#.................#........##...#.................#.#...........#.................
..............#.............##...........#...#.....#.#..........#..........................#........
#....##.....................#........##....#..................#.....................................
#.............................#..#..............#...................#............#..................
.....#....#.#........#.........................#...#................#.#....................#..#.....
....................#.............#..........#....#........#...#...#................................
.........................#...##...#.#...............#........#............#.........#.#..##.........
...#........#.##.........#.....#...#..............#.......#..#...#..........#...#.......#........#..
...#........#...##............#........#.............................##........##.........#.#....#..
.....##.#......#..#.............................................#..#........###.#....#..............
.................#...#.....#......#.............#.....#....................#.............#..........
.....#.......................#.#....##..........#..#......#....................#....#........#.....#
....#....................................#..............#...........#............#..#........#.#....
................#.....#....##.....#...#..#....#.......##............#.##.#.......................#..
.#...................................#.#..................#...#........#......#.....................
...........#.................#........#..#......#..................###...........##..#..............
.....................#.........#.#.......#..................................#....##.........#.......
..................#......#.....#.......#..........#............#..#...#.........#...#.......#.......
................#.................#..#.............................#..#........#....#..#............
.........................#.........................##............#..........##........#.............
...............##.......#........#.........................#.#......#.#.........#...................
...#........#......#...................#.##........#.#...............##.#...........................
.....#.#.........#.........#.#....#...........#.#.............#................#.#......#......#....
#.............#.....#.....###...........................#......................................#....
.#..#........#......#......#..#....#......#.#................#..#.#.........#..............#......#.
....#......#.......................#...#....#.............#......##.........................#...#...
.........#............#...#...#..........#.#.........#............................##....#.........#.
...........................#...........#.......#..#....................#..................#.....##..
......#....#.#.##.....#..#..........##.....#....#.........#......#......#..........#..#.....#.#.....
.........#.........................#....#.........................#..........#.#..............#.#...
................#................#..#...............................................................
..........#.........#..................#......................##..#..#........#...............#.....
.....#........#..#.........#...............................#...#.........................#..........
....#..#......#...#.......#..#.#.#.............#.........##..#.........#...................#.....##.
.....#.................#..........................
#..#.........#....#...................#...........
..#......#....#......#............................
..#...............................#...............
....#.#...#.........................#.#.........#.
#..............##...#..#.#.........#........#.....
..............#.......................###.........
#.#...#..............#...........#...........#.#..
..............................#...................
........................#.#...#....#..............
.......#...........................#..#...........
...................#......#.....#....#.#.........#
............##........................##.#.......#
.....#................#..............##...........
..............#........#...###................#...
..#....................#.......................#..
#..#.#......#......#.........#............#.......
......................#......#....................
....#....#..#....#............#....#..............
....##...........#.......##......#......#..#..#...
.#..........##...................................#
..................#..#...........................#
...............#............................#.....
...#.......#................#.#...#........#.....#
.......##......#...#...........#...........#......
..........................#......#..#.............
.........#.......#..........#.....................
.#.............#...........#....#.................
.....#....#................#..........#...........
...#.....................#.....#..................
.......#.............#.....#.#.........#..........
...#...........#..#.................#..........##.
....##.......................#.........#...#......
............#........#.........#.......##.#..#....
..............#...........................#....###
....#........#.#....#......#.#..#.........#.......
....#....................#.........#.........#....
.........................................##.......
........#....#..#.#..#.........#...#.........#....
.............................#..#...##........#...
...#.......#.......#...............#..............
......................................#......#....
.....#..................#.........................
#.....#.....#..#..................................
..#.....#..#...#................#.....#..........#
.....................#............#...............
.......#...................................#.....#
............#.#..#..#...#.........#...............
..##..........#...#..#........#..............#....
....#.....#..............................#........

23L18L48R15R47R38R29L34L19L43R3L35R3L49L30L34R17L42L18L20R2L44R50L42R26L8L20L30R42L2L8R15R46L12R15R38L10R43L2R42R11R43R47L27R49L28R2R14L26L13L21R36R20R28R40L49L4R34R35R34L43L50L2L48R31R39R6L26R37R1R28L43L23R46L5R15L41R11R23L36R40L47L33R37L25R36L22L12L42R26L44L41R45R13R38L4R29L15L2L6L23R21L1L19L22R28R43R19L43R7R10R19L1L40R45L49L45R37R7R9R40R41L13R29R13R24R47R30L17R50L6L13L40L5L24L46R30L42R36L33L50R46R15L50L6R4R33R6L41L32R25L43L20L45R47R40R41L31L19R44R33R2R46L2L26R25R3L11L36L34R4R48L13L17L49L49R45L35R21L27R8R41L25L13R45L32R35R44R33R39R36L10R19R49R1R23L49L22L12R33R20R13R20R26R27L17L25R48L1L40L15L43R45R6R47R48L46R48R7L33L50R14R14R31R32R21R34L15L34L19R14L14L41R37R14L50R32L11R4L41L22L20L34R49R32R6L2L8L20L37R2L18R15L3R16L42L15L43L38R4R39R35L50R36L31R12L50R22L25R12R15L37L42R26L23R6L22R37R8L40R29L44L46R36L32L17L16L31R27R46L49R19R12L25L33R28L16L2R20R37L40L13R27R31R23L23L12R18L44R49R3R50R27R31R3R31R21R20R37L7L47R33R13L20L45L25L14R47R32L50L33R6L30L38L40R34R8R13R48L10R22L2L5R43R31L9R25R34L9R38R46R44L24R16L24L35L49R9L3R16L15R18L39L6R23R7L38L17R34R25R35R28R1L30L48L13R21R27L22L17R40R27L9R27L4R7R20R14R39L13L2R31L1R31L44L31L35L7L49R21R14L20L33R43L41R16R19L9L40L27L48R49R44R2R12R16L10R26L40L45R32R19R10R24R36R41R5L8R41R30L27L22L20L47R30L26R17R37R16L3R19L13R37L26R16R33R46R25L41L25L2L27R7L49R29L42L1R47R27L33L20R26L36L15R41L27R17R27R42L15L14R30R1L25R15R2R16L1L2L5R29L13R30L11L26L26L39R41L50R32R25L8R1R10R6L8L45L36R28L26R40R31R46L28R36R39L19L41L23R12R25R48R11L10R6R5R23R47R34L34L8R12R48L48R27R40L45R28L8L11R45L34R39R4R41L4R37L15L23R28R1L34R41R46R11L46L29L33R49R14L22R14R32L45L23L1R35R2R12R43L2R8L13L22R27R31L36R4L1R13R38L40L40R36L33L21L39R39R33R17L31R31R32R27L36L4R46L23L37R12L16R37L1R35R16L49L15L39L37R48L32R30R15L38R42R46R42L10R47R18R27L14L48R16R42L15L29L15L1L25L28R28L5L11R42R35L22L7R21L2R49R26L43L32L14L11R33R19R21R17L50L28R37L20R15L18L11R20R14L4L32L29R44R22L42R39L2L39R33L34L5L18R25R11L3R17R35R38R30L17L6L24R13L30R20R30R28L5L31L50L46L3R32R28L43L20L23R32L43R33R50L31R29R48L12L28L11R25R14L15L25R5R1R20L44R25R31R20R29L9L37L7R12L33L44L28R36R50L19L24L9L29R5R45R20L21R22L41L25R24R27L38R29L2R23R34L3R34R11L35L26R34L27R27L14R28R19R28L47L5R30L17R44L5R49R17R48R49L15R48R4R44L9L3R28R4L6L40R24L46L34R40R26L29R4R38R20R43R47R36R19R17R45L34R19R11L4R41L23L50R34L45L15L38R38R18R4L15L26L15R49R22R14L33R37R22L20L19L23L9L40R30R42L44R22L41L20L26R32R29R40L48L18L43L48R46R24R25L15R37L32R22L22R17R27L28L28R15L38L36L20R7R35R38L42L22R24L27L2R39R9R1L6R15R12R7R50R26R20L41R44R27R42R40L50R12L50L37R46R45L7R27R45R5R5L37R34L24R25L1L5L35R20R29L11L45L1L1L18L43L38L50L1L18R48L18L24L9L1R28R8R6R46L9R18R4R33R49R28R35L18R45R27L46R10L41R20L25R49R49R50L25R30R20R45L47R41L39L50L39L5R36R46L28R42L45R11R2R3L18R15L43R48L35R21L20R49R47L33L31R10R7L20L30L35R25L16L49L41R44R49L31R10R40R17R30L16L17R17R13R46L32R46R34R46L23R40L38L13L20R46R27L38L48L2L26L32R30L29L15L41R19R13R46L28R6R23L26L40R24R40R4L7L21R40L5R34R31L6L12R19R42R29R31L19R8L15L30R10L42R33R6R20L48L26R19L24L34R4R47R26R31L44R47L14L9R41R41R38L7L38L30L37L13R23L44L15R48R33R18L49L20L26L39L8L24R50L19L33R27L27R23R24L36R12L22R23L34L49R2L14R45R38R23R46R18L40L39L20L24R46L34L15L1L44R28L20R23R18L19R27L15L1R34R35R1L26R47R12R26R7L18R36L16R48R37L18R18R29L16L21R24L45R27L15L32L48R38L6R48L44L29L2R47R50L13L32R40R48L9L46L1L50R8L3L13L13R30L10L36L8L13L33R13R14R36L16L24L38L7R1L22L48L36L22L39R44R24L44R21R20R17R1L17L19R50L45R6R6L13L12R25R2R37L43L46L34R1L26R9R17L25R2L5R31R20R33L44L3L43R12R40L32L7L4L39R38R18L7L14L21R21R15L30L50R6L6L37R5R24L39R19L27L28L47R20R31L15R31L23R1L2L43R39R36R18R30R21R3R34L35R35R25R38L40R27R20R47R23R45L20R3R41L17L40L3R14R37R22R48L47L40L43R1R33L6L43L28R17L48L28R32R24R39R22L31R30L18L36L13L12R24R16L20L26R47L20L6R25R28R43L32R47R12R49R32R10L24R15L50L39L41L16R25R40R28R31L9L50L37R24L15R7L26L22R49R33R50R38L46L17R29R29L21L35R20L50R7R26R36R29L47R44R25R29L1L41L50L25R30L1L22L12R6L7L32L33L19L34L31R5R25R28L12R5R35L34R41R50L8R20R28L3R48L7R10R26R16R17R38R35R31L7L48L50R37L46L31L36L46R45L50R18L46L39L26L48R38L11L37R50L48R43R17L31L41R38L31R19R11R41L46L12L14L45R24R33R23L25L16R7R24R39L21R18L31R33L49R4L33L32L3R29L37L40R4L33L16L9R7R30R34L37R13R8L50L4R49R50L17L50L2R11L32L34L21R6L49L16R46R27R25R14R35L46R21R5R43R16R28R43L4L24L26L2R26R15R41L2R37L33L6R37L39R29L43R34R14L28L14R50L48R21R32L4L7L47R26R34R16R47R19L11R19R40L24R47L8L13R47R10R43L23L10L34L1R27R15L32L40L18L40L45R41L50L28R9L9L41R19R6L26R45R5R49R21R45L21L48R5R43L25L44L15L4R46R45R35L8R21R33R10L7L5R22R12R10L28R43L49L33R28R12L3L24L21L28R7L1L42L12L50R26L28L19L3R39R10R27L17R16L12R36L22L40L38L30R16R10L2R39R18L20L24R11R14R8R37R45R16R33L29R2R27L8R22R15R21L26L14R40R27L50L30R24L31L9L9R31R49L21R42L19L19L5R24L34R47R6R25R21R15R24R43R25L4R14R9L5L42L15L40R18R33L47R37R22L38R1R35R41R39L34L46L27L2R18L42R13L49R46L49L15R39R13L24R36L29L5R18R44R36R37L6R48R39L27R11R16R35L48R47R44L42R29R12L30R20L50R17L11L32R27L11L49L43L9R18R2R33L15R5R24R28R45R41R10L43R46L44L36L21R4R27L8R7R13L46R25L43L4R44R28R38L27L11R45L25R17L13R26L4L33L20R39R25L16R28R36L10L14R13L46L25L22R44R20R5L11L32L10L32R1L20R5R15L34L23R24R2R25L36R39R15L10L26L13L42L21L8L22L9L25R9R3R40R50R15R29L10L27L13L20R32R5R4L2R11L25L29L22L5L6R22R49R18L24L45L40R4L3L41L12L8R5L29L26R22L40R20L7L6L18R39R24R22L9L41L32L30L21L15R10R32R31L31R8L5R3R5R41R48R16L35R37L33R9R27R25L4L15L34L18L45R22L22L32R31L32R41L10R41R45R34R36R20L46L40L37L22L21R8R23R50R32L40R24R43L47L36L46L19R1L23R48L25L8R5R21L13R27R28L14R12R49L31L22L19R8R41L14R3R45L49L15L3L6L48R9L14L11R44L31R35L25L47R6R29R35R49L16L38L7L2R8R50R50L22L30L38R38L3R28L14L3R12L50L18L5L6L17L35L16L36R28R5R17R29R27R20R20L23L38R44L28R6R29R5R28R1R34R2R33L49L42L11R11R17L27L25R49R48L9L19R10R1R33R27L35L35R45R13L23R14R14R50L39L10R34L33R39L42R7R16L43L38L19R8R12R37L20L2R16L32L30R24R38L45L24L19R43L19R25L41R46R48R49L36R40L12L13L29R38L27L24R45R6L20R12R7R29)";

void run()
{
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}// namespace level22

#endif// AOC_2022_CPP_LEVEL22_HPP
