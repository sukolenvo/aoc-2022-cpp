#include <string>

#include <catch2/catch.hpp>

#include "common.hpp"

TEST_CASE("common:readFile", "[common]")
{
  REQUIRE(readFile("resources/sample.txt") == std::string("Hello World"));
}