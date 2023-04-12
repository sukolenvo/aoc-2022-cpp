//
// Created by object on 9/04/23.
//
#include <catch2/catch.hpp>

#include "level19.hpp"

static const auto sampleInput = R"(Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.)";

TEST_CASE("level19:solve", "[level19]") {
  const auto blueprints = level19::parseInput(sampleInput);
  REQUIRE(level19::calculateGeodes(blueprints[0], 24) == 9);
  REQUIRE(level19::part1(sampleInput) == 33);
  REQUIRE(level19::part2(sampleInput) == 62 * 56);
}