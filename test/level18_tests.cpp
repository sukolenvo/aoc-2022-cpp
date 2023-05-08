//
// Created by object on 4/04/23.
//
#include <catch2/catch.hpp>

#include "level18.hpp"

static const auto sampleInput = R"(2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5)";

TEST_CASE("level18:solve", "[level18]")
{
  REQUIRE(level18::part1(sampleInput) == 64);
  REQUIRE(level18::part2(sampleInput) == 58);
}