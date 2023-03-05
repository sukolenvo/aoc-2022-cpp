//
// Created by object on 3/12/22.
//
#include <catch2/catch.hpp>

#include "level2.hpp"

const auto sample_input = R"(A Y
B X
C Z)";

TEST_CASE("level2:solve2", "[level2]")
{
  REQUIRE(level2::solve(sample_input, &level2::calculatePointsPart1) == 15);
  REQUIRE(level2::solve(sample_input, &level2::calculatePointsPart2) == 12);
}