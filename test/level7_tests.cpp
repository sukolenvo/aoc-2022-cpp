//
// Created by object on 4/03/23.
//
#include <catch2/catch.hpp>

#include "level7.hpp"

static const auto sampleInput = R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)";

TEST_CASE("level7:solve", "[level7]")
{
  REQUIRE(level7::part1(sampleInput) == 95437);
  REQUIRE(level7::part2(sampleInput) == 24933642);
}