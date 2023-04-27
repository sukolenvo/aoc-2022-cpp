#include <string>

#include <catch2/catch.hpp>

#include "common.hpp"

TEST_CASE("common:readFile", "[common]")
{
  REQUIRE(readFile("resources/sample.txt") == std::string("Hello World"));
}

static const auto sample_input = R"(1000
2000
3000

4000

5000
6000

7000
8000
9000

10000)";

static const auto singleChar = R"(1
2)";

TEST_CASE("common:splitLines", "[common]")
{
  const auto sampleResult = splitLines(sample_input);
  REQUIRE(sampleResult.size() == 14);
  REQUIRE(sampleResult[0] == "1000");
  REQUIRE(sampleResult[1] == "2000");
  REQUIRE(sampleResult[2] == "3000");
  REQUIRE(sampleResult[3].empty());
  REQUIRE(sampleResult[13] == "10000");

  const auto singleCharResult = splitLines(singleChar);
  REQUIRE(singleCharResult.size() == 2);
  REQUIRE(singleCharResult[0] == "1");
  REQUIRE(singleCharResult[1] == "2");
}