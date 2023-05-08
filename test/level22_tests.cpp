//
// Created by object on 14/04/23.
//
#include <tuple>

#include <catch2/catch.hpp>

#include "level22.hpp"

static const auto sampleInput = R"(        ...#
        .#..
        #...
        ....
...#.......#
........#...
..#....#....
..........#.
        ...#....
        .....#..
        .#......
        ......#.

10R5L5R10L4R5L5)";

auto oppositeDirection(const auto &direction)
{
  return static_cast<level22::Direction>((static_cast<int>(direction) + 2) % 4);
}

TEST_CASE("level22:part2handleOOBsample", "[level22]")
{
  const std::vector<std::string_view> lines(12);
  level22::Direction startDirection{};
  int startX{};
  int startY{};
  SECTION("UP")
  {
    startDirection = level22::Direction::up;
    std::tie(startX, startY) =
      GENERATE(std::make_tuple(0, 4), std::make_tuple(4, 4), std::make_tuple(8, 0), std::make_tuple(12, 8));
  }
  SECTION("DOWN")
  {
    startDirection = level22::Direction::down;
    std::tie(startX, startY) =
      GENERATE(std::make_tuple(0, 7), std::make_tuple(4, 7), std::make_tuple(8, 11), std::make_tuple(12, 11));
  }
  SECTION("RIGHT")
  {
    startDirection = level22::Direction::right;
    std::tie(startX, startY) = GENERATE(std::make_tuple(11, 0), std::make_tuple(11, 4), std::make_tuple(15, 8));
  }
  SECTION("LEFT")
  {
    startDirection = level22::Direction::left;
    std::tie(startX, startY) = GENERATE(std::make_tuple(8, 0), std::make_tuple(0, 4), std::make_tuple(8, 8));
  }
  CAPTURE(startDirection, startX, startY);
  auto tmpDirection = startDirection;
  auto tmpPosition = level22::getNextPosition(startX, startY, tmpDirection);
  level22::part2handleOOBsample(tmpDirection, tmpPosition, lines);
  tmpDirection = oppositeDirection(tmpDirection);
  tmpPosition = level22::getNextPosition(tmpPosition.first, tmpPosition.second, tmpDirection);
  level22::part2handleOOBsample(tmpDirection, tmpPosition, lines);
  tmpDirection = oppositeDirection(tmpDirection);
  // expecting to return to the same position
  REQUIRE(tmpDirection == startDirection);
  REQUIRE(tmpPosition.first == startX);
  REQUIRE(tmpPosition.second == startY);
}

TEST_CASE("level22:part2handleOOBtask", "[level22]")
{
  const std::vector<std::string_view> lines(16);
  level22::Direction startDirection{};
  int startX{};
  int startY{};
  SECTION("UP")
  {
    startDirection = level22::Direction::up;
    std::tie(startX, startY) = GENERATE(std::make_tuple(0, 8), std::make_tuple(4, 0), std::make_tuple(8, 0));
  }
  SECTION("DOWN")
  {
    startDirection = level22::Direction::down;
    std::tie(startX, startY) = GENERATE(std::make_tuple(0, 15), std::make_tuple(4, 11), std::make_tuple(8, 3));
  }
  SECTION("RIGHT")
  {
    startDirection = level22::Direction::right;
    std::tie(startX, startY) =
      GENERATE(std::make_tuple(11, 0), std::make_tuple(7, 4), std::make_tuple(7, 8), std::make_tuple(3, 12));
  }
  SECTION("LEFT")
  {
    startDirection = level22::Direction::left;
    std::tie(startX, startY) =
      GENERATE(std::make_tuple(4, 0), std::make_tuple(4, 4), std::make_tuple(0, 8), std::make_tuple(0, 12));
  }
  CAPTURE(startDirection, startX, startY);
  auto tmpDirection = startDirection;
  auto tmpPosition = level22::getNextPosition(startX, startY, tmpDirection);
  level22::part2handleOOBtask(tmpDirection, tmpPosition, lines);
  tmpDirection = oppositeDirection(tmpDirection);
  tmpPosition = level22::getNextPosition(tmpPosition.first, tmpPosition.second, tmpDirection);
  level22::part2handleOOBtask(tmpDirection, tmpPosition, lines);
  tmpDirection = oppositeDirection(tmpDirection);
  // expecting to return to the same position
  REQUIRE(tmpDirection == startDirection);
  REQUIRE(tmpPosition.first == startX);
  REQUIRE(tmpPosition.second == startY);
}

TEST_CASE("level22:turn", "[level22]")
{
  REQUIRE(level22::turn(level22::Direction::up, 'L') == level22::Direction::left);
  REQUIRE(level22::turn(level22::Direction::up, 'R') == level22::Direction::right);
  REQUIRE(level22::turn(level22::Direction::right, 'L') == level22::Direction::up);
  REQUIRE(level22::turn(level22::Direction::right, 'R') == level22::Direction::down);
}

TEST_CASE("level22:solve", "[level22]")
{
  REQUIRE(level22::part1(sampleInput) == 6032);
  REQUIRE(level22::part2(sampleInput) == 5031);
}