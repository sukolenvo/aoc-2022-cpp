//
// Created by object on 21/04/23.
//
#include <catch2/catch.hpp>

#include "level24.hpp"

static const auto sampleInput = R"(#.######
#>>.<^<#
#.<..<<#
#>v.><>#
#<^v^^>#
######.#)";

TEST_CASE("level24:solve", "[level24]")
{
  REQUIRE(level24::part1(sampleInput) == 18);
  REQUIRE(level24::part2(sampleInput) == 54);
}