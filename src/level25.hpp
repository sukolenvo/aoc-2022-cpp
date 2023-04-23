//
// Created by object on 23/04/23.
//

#ifndef AOC_2022_CPP_LEVEL25_HPP
#define AOC_2022_CPP_LEVEL25_HPP

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <string>

#include "common.hpp"

namespace level25 {

static const auto snafuBase = 5;

auto fromSnafu(const auto &value) {
  long result = 0;
  for (size_t i = 0; i < value.size(); i++) {
    switch(value[i]) {
      case '2':
        result += static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i)) * 2;
        break;
      case '1':
        result += static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i));
        break;
      case '0':
        break;
      case '-':
        result -= static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i));
        break;
      case '=':
        result -= static_cast<decltype(result)>(std::pow(snafuBase, value.size() - 1 - i)) * 2;
        break;
      default:
        throw std::runtime_error("unexpected SNAFU symbol");
    }
  }
  return result;
}

auto toSnafu(auto value) {
  std::string result;
  while (value > 0) {
    auto digit = value % snafuBase;
    switch(digit) {
      case 0:
        result.append("0");
        break;
      case 1:
        result.append("1");
        break;
      case 2:
        result.append("2");
        break;
      case 3:
        result.append("=");
        break;
      case 4:
        result.append("-");
        break;
      default:
        throw std::runtime_error("unexpected modulus");
    }
    value /= snafuBase;
    if (digit > 2) {
      ++value;
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

auto part1(const auto &input) {
  const auto lines = splitLines(input);
  auto result = std::accumulate(lines.begin(), lines.end(), 0L, [] (const auto sum, const auto &line) {
    return sum + fromSnafu(line);
  });
  return toSnafu(result);
}

static const auto taskInput = R"(1--
1-
1--02
1122-=2-==0-0=1-2
2=1-==-1022=10
1==-02211022=000
101==210-21=2
11010==0==221=0=02
11=1101020=02-11
1===
10-2
10=02=-=02
1-1--112-202--0
11-=-0-002=0112-1
2--0-2=02=-22
1001-2
1=101=2=2102-201--1
1-2==1100--
2-01=--=1-11
101-=0010-210==
1=-21-022121110
1==21
22==1
2-0-=2=1
2--0=10-00--2-
1--1=011-2
1-100-2
11-1
112
222200110-11110-2=
1==2-210-1122=-2022
2==1-20-2-=-21
21=1-=0=-1-=-=-12
210---=-1===-===00
1--21=01-022=2-
11-=
21-2
2-22-0
1==-
1-----01
1-0-21-
211212-22-12--=2
1=220=10=20200-0
21-=11-==20=1
102-2==2-==-0
1=0=01-
22=1--0111
1-----=1==00===0-
20==-1-121-
1=--2--21-202=21
1-0122=--=
2==-2--===-=-1=-012
2220=-00-10-
1==0=---10212-1-
1221=-022120
100112
2-=0=220-=01-1=12-
1=00
2122121=2
2-==
12-21===10220
1=0
22-1=--01-==0
1-2-2202=1=02
11-===21-
1-22=0102-=-222-1
1021=0=21=0=11=2-2
1===2222=0-0=
20=2-=0=
1=2==02-1-100200
1=1211211----
12=1=1---=20
2--2=2=1100020200
1=0-002-11-10----
1=1=-1=-0
1-21=1=02=21=20
1-=0-002-0=--=0=
20-===-1=
220021012
12===1001=--=2=1-
1=-02
21
2=200222122
1-2=110111=012=1-
1=02=
101==1-=1=0001
1=22--1-021
120
201-0211
1-1012211--2=-1=
12=10202=20
11-22=2=2=-0==1=00
11=-
12-1--2222
22
1120--0==
10=-0=001-
1=00200200=
1-20
1-0-
10112-202=-011
11-2=
1-=10=1-0=-21==11200
11
12-0-=01--12
1-=1121-=0
120-21=-1=10121=
1102-22
21=
1==--1==---1-==
12=0--=2210-1==
22210=1-=0
1102-20-1
1=1=2102
11--=2==-0
2=0=1=
21-0-10=
12-212-2=-==102--
1==0100210-=
12210-0-1
1=0-2-0=
10-1=211=1-2=
201-0--
1--2=00=-1-211
121=-22100=210--
122-0-1-
2=211
2=1-=2-2-0001=-1
1102--11=0=1=10==2
1==01121-2--0=12
2=21-20=-0-0
2-212=21-02=1-2-02
2=1-0
10-0-=20
2=2=201)";

void run() {
  std::cout << part1(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL25_HPP
