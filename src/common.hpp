//
// Created by object on 3/12/22.
//

#ifndef AOC_2022_CPP_COMMON_HPP
#define AOC_2022_CPP_COMMON_HPP

#include <cstdio>
#include <exception>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>

inline std::vector<std::string_view> splitLines(const std::string_view &input)
{
  std::vector<std::string_view> lines;
  auto start = 0;
  auto index = 1;
  for (const auto &item : input) {
    if (item == '\n') {
      lines.emplace_back(input.begin() + start, input.begin() + index - 1);
      start = index;
    }
    ++index;
  }
  if (start != index - 1) {
    lines.emplace_back(input.begin() + start, input.begin() + index - 1);
  }
  return lines;
}

inline auto readNumber(auto start, auto end)
{
  auto value = 0;
  auto negative = false;
  if (*start == '-') {
    negative = true;
    ++start;
  }
  while (start != end) {
    value *= 10;
    value += *start++ - '0';
  }
  return negative ? value * -1 : value;
}

inline auto readFile(auto filename)
{
  std::ifstream stream(filename);
  if (stream.is_open()) {
    auto* pbuf = stream.rdbuf();
    const long size = pbuf->pubseekoff(0, stream.end, stream.in);
    pbuf->pubseekpos(0, stream.in);
    char* buffer = new char[static_cast<unsigned long>(size + 1)]();
    pbuf->sgetn (buffer,size);
    stream.close();
    return buffer;
  } else {
    throw std::runtime_error("Failed to open task input file");
  }
}

inline auto readTaskInput(auto level)
{
  std::array<char, 100> filename;
  auto status = std::snprintf(filename.data(), filename.max_size(), "input/task%d.txt", level);
  if (status < 0 || static_cast<size_t>(status) >= filename.max_size()) {
    throw std::runtime_error("Failed to construct task input filename");
  }
  return readFile(filename.data());
}

#endif// AOC_2022_CPP_COMMON_HPP