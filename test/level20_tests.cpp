//
// Created by object on 11/04/23.
//
#include <catch2/catch.hpp>

#include "level20.hpp"

static const auto sampleInput = R"(1
2
-3
3
-2
0
4)";

TEST_CASE("level20:solve", "[level20]")
{
  REQUIRE(level20::part1(sampleInput) == 3);
  REQUIRE(level20::part2(sampleInput) == 1623178306L);
}