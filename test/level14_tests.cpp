//
// Created by object on 18/03/23.
//
#include <catch2/catch.hpp>

#include <vector>

#include "level14.hpp"

static const auto sampleInput = R"(498,4 -> 498,6 -> 496,6
503,4 -> 502,4 -> 502,9 -> 494,9)";

TEST_CASE("level14:solve", "[level14]")
{
  REQUIRE(level14::part1(sampleInput) == 24);
  REQUIRE(level14::part2(sampleInput) == 93);
}