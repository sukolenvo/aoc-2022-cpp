//
// Created by object on 1/04/23.
//
#include <catch2/catch.hpp>

#include "level17.hpp"

static const auto sampleInput = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";

TEST_CASE("level17:solve", "[level17]") {
  REQUIRE(level17::part1(sampleInput) == 3068);
  REQUIRE(level17::part2(sampleInput) == 1'514'285'714'288);
}