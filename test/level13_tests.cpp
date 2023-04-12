//
// Created by object on 15/03/23.
//
#include <catch2/catch.hpp>

#include <vector>

#include "level13.hpp"

static const auto sampleInput = R"([1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9])";

TEST_CASE("level13:parse", "[level13]") {
  const char *input = "[[],[10,10,9],[[0,[1]]],[0,10]]";
  auto packet = level13::parsePacket(input);
  REQUIRE(packet.fragments.size() == 4);
}

TEST_CASE("level13:getValidPackets", "[level13]")
{
  REQUIRE(level13::getValidPackets(sampleInput) == std::vector<size_t>{0, 1, 3, 5});
}

TEST_CASE("level13:solve", "[level13]")
{
  REQUIRE(level13::part1(sampleInput) == 13);
  REQUIRE(level13::part2(sampleInput) == 140);
}