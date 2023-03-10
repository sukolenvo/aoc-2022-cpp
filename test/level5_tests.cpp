//
// Created by object on 21/01/23.
//
#include <array>
#include <vector>
#include <catch2/catch.hpp>

#include "level5.hpp"

static const auto sample_input = R"(    [D]
[N] [C]
[Z] [M] [P]
 1   2   3

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2)";

TEST_CASE("parseInput", "[level5]")
{
  const auto result = level5::parseInput(sample_input);
  REQUIRE(result.stacks.size() == 3);
  REQUIRE(result.stacks[0] == std::vector<char>{ 'Z', 'N' });
  REQUIRE(result.stacks[1] == std::vector<char>{ 'M', 'C', 'D' });
  REQUIRE(result.stacks[2] == std::vector<char>{ 'P' });

  REQUIRE(result.operations.size() == 4);
  REQUIRE(result.operations[0] == std::array<unsigned int, 3>{  1, 2, 1 });
  REQUIRE(result.operations[1] == std::array<unsigned int, 3>{  3, 1, 3 });
  REQUIRE(result.operations[2] == std::array<unsigned int, 3>{  2, 2, 1 });
  REQUIRE(result.operations[3] == std::array<unsigned int, 3>{  1, 1, 2 });
}

TEST_CASE("level5:runCrane9000", "[level5]")
{
  const auto result = level5::runCrane9000(sample_input);
  REQUIRE(result == std::vector<char>{ 'C', 'M', 'Z' });
}

TEST_CASE("level5:runCrane9001", "[level5]")
{
  const auto result = level5::runCrane9001(sample_input);
  REQUIRE(result == std::vector<char>{ 'M', 'C', 'D' });
}