//
// Created by object on 4/01/23.
//
#include <catch2/catch.hpp>

#include "level3.hpp"

const auto sample_input = R"(vJrwpWtwJgWrhcsFMMfFFhFp
jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL
PmmdzqPrVvPwwTWBwg
wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn
ttgJtRGJQctTZtZT
CrZsJsPPZsGzwwsLwLmpwMDw)";

TEST_CASE("level3:solve3", "[level3]")
{
  REQUIRE(level3::part1(sample_input) == 157);
  REQUIRE(level3::part2(sample_input) == 70);
}