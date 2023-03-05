//
// Created by object on 26/02/23.
//
#include <catch2/catch.hpp>

#include "level6.hpp"

TEST_CASE("level6:searchMarker", "[level6]")
{
  REQUIRE(level6::searchMarker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 4) == 7);
  REQUIRE(level6::searchMarker("bvwbjplbgvbhsrlpgdmjqwftvncz", 4) == 5);
  REQUIRE(level6::searchMarker("nppdvjthqldpwncqszvftbrmjlhg", 4) == 6);
  REQUIRE(level6::searchMarker("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 4) == 10);
  REQUIRE(level6::searchMarker("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 4) == 11);

  REQUIRE(level6::searchMarker("mjqjpqmgbljsphdztnvjfqwrcgsmlb", 14) == 19);
  REQUIRE(level6::searchMarker("bvwbjplbgvbhsrlpgdmjqwftvncz", 14) == 23);
  REQUIRE(level6::searchMarker("nppdvjthqldpwncqszvftbrmjlhg", 14) == 23);
  REQUIRE(level6::searchMarker("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 14) == 29);
  REQUIRE(level6::searchMarker("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 14) == 26);
}