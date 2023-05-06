//
// Created by object on 19/04/23.
//
#include <catch2/catch.hpp>

#include "level23.hpp"

static const auto sampleInput = R"(....#..
..###.#
#...#.#
.#...##
#.###..
##.#.##
.#..#..)";

TEST_CASE("level23:moveElves", "[level23]")
{
  std::string expected;
  size_t rounds{};
  SECTION("1 round")
  {
    rounds = 1;
    expected =
      ".....#...\n"
      "...#...#.\n"
      ".#..#.#..\n"
      ".....#..#\n"
      "..#.#.##.\n"
      "#..#.#...\n"
      "#.#.#.##.\n"
      ".........\n"
      "..#..#...\n";
  }
  SECTION("10 rounds")
  {
    rounds = 10;// NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    expected =
      "......#.....\n"
      "..........#.\n"
      ".#.#..#.....\n"
      ".....#......\n"
      "..#.....#..#\n"
      "#......##...\n"
      "....##......\n"
      ".#........#.\n"
      "...#.#..#...\n"
      "............\n"
      "...#..#..#..\n";
  }
  CAPTURE(rounds);
  const auto elves = level23::moveElves(sampleInput, rounds);
  const auto minX =
    std::min_element(elves.begin(), elves.end(), [](const auto &l, const auto &r) { return l.x < r.x; })->x;
  const auto maxX =
    std::max_element(elves.begin(), elves.end(), [](const auto &l, const auto &r) { return l.x < r.x; })->x;
  const auto minY =
    std::min_element(elves.begin(), elves.end(), [](const auto &l, const auto &r) { return l.y < r.y; })->y;
  const auto maxY =
    std::max_element(elves.begin(), elves.end(), [](const auto &l, const auto &r) { return l.y < r.y; })->y;
  std::string map;
  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      const auto found =
        std::any_of(elves.begin(), elves.end(), [&](const auto &elf) { return elf.x == x && elf.y == y; });
      map.append(found ? "#" : ".");
    }
    map.append("\n");
  }
  REQUIRE(map == expected);
}

TEST_CASE("level23:solve", "[level23]")
{
  REQUIRE(level23::part1(sampleInput) == 110);
  REQUIRE(level23::part2(sampleInput) == 20);
}