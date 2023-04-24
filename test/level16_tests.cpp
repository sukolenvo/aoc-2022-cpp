//
// Created by object on 21/03/23.
//
#include <catch2/catch.hpp>
#include <vector>
#include <string_view>

#include "level16.hpp"

static const auto sampleInput = R"(Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II)";

TEST_CASE("level16:parseInput", "[level16]") {
  auto valves = level16::parseInput(sampleInput);
  REQUIRE(valves.size() == 10);
  REQUIRE(valves[0].getId() == std::string_view("AA"));
  REQUIRE(valves[0].getRate() == 0);
  REQUIRE(valves[0].getTunnels() == std::vector<std::string_view>{"DD", "II", "BB"});
  REQUIRE(valves[1].getRate() == 13);
}

TEST_CASE("level16:solve", "[level16]") {
  REQUIRE(level16::part1(sampleInput) == 1651);
  REQUIRE(level16::part2(sampleInput) == 1707);
}