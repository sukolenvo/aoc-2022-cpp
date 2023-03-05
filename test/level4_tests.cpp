//
// Created by object on 21/01/23.
//
#include <catch2/catch.hpp>

#include "level4.hpp"

const auto sample_input = R"(2-4,6-8
2-3,4-5
5-7,7-9
2-8,3-7
6-6,4-6
2-6,4-8)";

TEST_CASE("lvel4:solve4", "[level4]")
{
  REQUIRE(level4::part1(sample_input) == 2);
  REQUIRE(level4::part2(sample_input) == 4);
}