//
// Created by object on 20/03/23.
//
#include <catch2/catch.hpp>

#include <vector>

#include "level15.hpp"

static const auto sampleInput = R"(Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3)";

TEST_CASE("level15:parseInput", "[level15]") {
  auto beacons = level15::parseInput(sampleInput);
  REQUIRE(beacons.size() == 14);
  REQUIRE(beacons[0].getSensorX() == 2);
  REQUIRE(beacons[0].getSensorY() == 18);
  REQUIRE(beacons[0].getBeaconX() == -2);
  REQUIRE(beacons[0].getBeaconY() == 15);
}

TEST_CASE("level15:solve", "[level15]") {
  REQUIRE(level15::part1(sampleInput, 10) == 26);
  const auto beaconPosition = level15::findOnlyBeaconPlace(sampleInput, 20);
  REQUIRE(beaconPosition.first == 14);
  REQUIRE(beaconPosition.second == 11);
  REQUIRE(level15::part2(sampleInput, 20) == 56'000'011);
}