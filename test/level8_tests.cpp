//
// Created by object on 5/03/23.
//

#include <catch2/catch.hpp>

#include "level8.hpp"

static const auto sampleInput = R"(30373
25512
65332
33549
35390)";

TEST_CASE("level8:solve", "[level8]")
{
  REQUIRE(level8::part1(sampleInput) == 21);
  REQUIRE(level8::part2(sampleInput) == 8);
}