//
// Created by object on 8/03/23.
//
#include <catch2/catch.hpp>

#include "level9.hpp"

static const auto sampleInput = R"(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2)";

static const auto sampleInput2 = R"(R 5
U 8
L 8
D 3
R 17
D 10
L 25
U 20)";

TEST_CASE("level9:solve", "[level9]")
{
  REQUIRE(level9::solve<2>(sampleInput) == 13);
  REQUIRE(level9::solve<10>(sampleInput) == 1);
  REQUIRE(level9::solve<10>(sampleInput2) == 36);
}