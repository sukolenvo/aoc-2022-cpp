#include <string>

#include <catch2/catch.hpp>

#include "common.hpp"

TEST_CASE("common:readFile", "[common]")
{
  REQUIRE(std::string(readFile("resources/sample.txt")) == std::string("Hello World"));
}