//
// Created by object on 23/04/23.
//
#include <catch2/catch.hpp>

#include "level25.hpp"

static const auto sampleInput = R"(1=-0-2
12111
2=0=
21
2=01
111
20012
112
1=-1=
1-12
12
1=
122)";

TEST_CASE("level25:solve", "[level25]")
{
  REQUIRE(level25::fromSnafu(std::string("1=11-2")) == 2022);
  REQUIRE(level25::part1(sampleInput) == "2=-1=0");
}