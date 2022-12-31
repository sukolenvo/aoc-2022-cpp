//
// Created by object on 3/12/22.
//
#include "level1.hpp"

#include <catch2/catch.hpp>

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

TEST_CASE("solve", "[level1]")
{
  REQUIRE(solve<1>(sample_input) == 24000);
  REQUIRE(solve<3>(sample_input) == 45000);
}

TEST_CASE("saveDwarfSecond", "[level1]")
{
  std::array<int, 3> dwarf = {3, 1, 0};
  saveDwarf(dwarf, 2);
  REQUIRE(dwarf.at(0) == 3);
  REQUIRE(dwarf.at(1) == 2);
  REQUIRE(dwarf.at(2) == 1);
}

TEST_CASE("saveDwarfTop", "[level1]")
{
  std::array<int, 3> dwarf = {3, 0, 0};
  saveDwarf(dwarf, 5); //NOLINT
  REQUIRE(dwarf.at(0) == 5);
  REQUIRE(dwarf.at(1) == 3);
  REQUIRE(dwarf.at(2) == 0);
}

TEST_CASE("saveDwarfLast", "[level1]")
{
  std::array<int, 3> dwarf = {5, 3, 1};  //NOLINT
  saveDwarf(dwarf, 2);
  REQUIRE(dwarf.at(0) == 5);
  REQUIRE(dwarf.at(1) == 3);
  REQUIRE(dwarf.at(2) == 2);
}