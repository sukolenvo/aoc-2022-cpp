//
// Created by object on 12/03/23.
//
#include <catch2/catch.hpp>

#include "level11.hpp"

static const auto sampleInput = R"(Monkey 0:
  Starting items: 79, 98
  Operation: new = old * 19
  Test: divisible by 23
    If true: throw to monkey 2
    If false: throw to monkey 3

Monkey 1:
  Starting items: 54, 65, 75, 74
  Operation: new = old + 6
  Test: divisible by 19
    If true: throw to monkey 2
    If false: throw to monkey 0

Monkey 2:
  Starting items: 79, 60, 97
  Operation: new = old * old
  Test: divisible by 13
    If true: throw to monkey 1
    If false: throw to monkey 3

Monkey 3:
  Starting items: 74
  Operation: new = old + 3
  Test: divisible by 17
    If true: throw to monkey 0
    If false: throw to monkey 1)";

TEST_CASE("level11:parseInput", "[level11]")
{
  auto result = level11::parseInput(sampleInput);

  REQUIRE(result.size() == 4);
  REQUIRE(result.at(0).getTestDivide() == 23);
  REQUIRE(result.at(0).getSuccessMonkey() == 2);
  REQUIRE(result.at(0).getFailedMonkey() == 3);
  REQUIRE(result.at(0).getItems() == std::vector<size_t>{79, 98});
}

TEST_CASE("level11:solve", "[level11]")
{
  REQUIRE(level11::part1(sampleInput) == 10605);
  REQUIRE(level11::part2(sampleInput) == 2713310158);
}