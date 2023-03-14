//
// Created by object on 14/03/23.
//
#include <catch2/catch.hpp>

#include "level12.hpp"

static const auto sampleInput = R"(Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi)";

TEST_CASE("level12:solve", "[level12]")
{
  REQUIRE(level12::part1(sampleInput) == 31);
  REQUIRE(level12::part2(sampleInput) == 29);
}