//
// Created by object on 3/12/22.
//
#include <catch2/catch.hpp>

#include "level1.hpp"

const auto sample_input = R"(
1000
2000
3000

4000

5000
6000

7000
8000
9000

10000
)";

TEST_CASE("solve1", "[level1]")
{
  REQUIRE(level1::solve<1>(sample_input) == 24000);
  REQUIRE(level1::solve<3>(sample_input) == 45000);
}

TEST_CASE("save1DwarfSecond", "[level1]")
{
  std::array<int, 3> dwarf = {3, 1, 0};
  level1::saveDwarf(dwarf, 2);
  REQUIRE(dwarf.at(0) == 3);
  REQUIRE(dwarf.at(1) == 2);
  REQUIRE(dwarf.at(2) == 1);
}

TEST_CASE("save1DwarfTop", "[level1]")
{
  std::array<int, 3> dwarf = {3, 0, 0};
  level1::saveDwarf(dwarf, 5); //NOLINT
  REQUIRE(dwarf.at(0) == 5);
  REQUIRE(dwarf.at(1) == 3);
  REQUIRE(dwarf.at(2) == 0);
}

TEST_CASE("save1DwarfLast", "[level1]")
{
  std::array<int, 3> dwarf = {5, 3, 1};  //NOLINT
  level1::saveDwarf(dwarf, 2);
  REQUIRE(dwarf.at(0) == 5);
  REQUIRE(dwarf.at(1) == 3);
  REQUIRE(dwarf.at(2) == 2);
}