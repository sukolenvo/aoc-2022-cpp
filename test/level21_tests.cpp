//
// Created by object on 13/04/23.
//
#include <catch2/catch.hpp>

#include "level21.hpp"

static const auto sampleInput = R"(root: pppw + sjmn
dbpl: 5
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
dvpt: 3
lfqf: 4
humn: 5
ljgn: 2
sjmn: drzm * dbpl
sllz: 4
pppw: cczh / lfqf
lgvd: ljgn * ptdq
drzm: hmdt - zczc
hmdt: 32)";

TEST_CASE("level21:solve", "[level21]") {
  REQUIRE(level21::part1(sampleInput) == 152);
  REQUIRE(level21::part2(sampleInput) == 301);
}