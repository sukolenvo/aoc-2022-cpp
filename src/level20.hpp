//
// Created by object on 11/04/23.
//

#ifndef AOC_2022_CPP_LEVEL20_HPP
#define AOC_2022_CPP_LEVEL20_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>

#include "common.hpp"

namespace level20 {

auto shuffle(const auto &input) {
  auto lines = splitLines(input);
  std::vector<std::pair<int, bool>> items;
  items.reserve(lines.size());
  std::transform(lines.begin(), lines.end(), std::back_inserter(items), [](const auto &line) {
    return std::pair<int, bool>{readNumber(line.begin(), line.end()), false};
  });
  for (size_t i = 0; i < items.size(); ++i) {
    auto item = items[i];
    if (item.second) {
      continue;
    }
    if (item.first == 0) {
      continue;
    }
    int pos = (item.first + static_cast<int>(i)) % static_cast<int>(items.size() - 1);
    if (pos < 0) {
      pos += static_cast<int>(items.size()) - 1;
    }
    items.erase(items.begin() + static_cast<long int>(i));
    item.second = true;
    items.insert(items.begin() + pos, item);
    --i;
  }
  return items;
}

auto part1(const auto &input) {
  const auto items = shuffle(input);
  const auto zeroPos = static_cast<size_t>(std::distance(items.begin(), std::find(items.begin(), items.end(), std::pair<int, bool>{ 0, false })));
  const auto fPos = (zeroPos + 1000) % items.size();
  const auto sPos = (zeroPos + 2000) % items.size();
  const auto tPos = (zeroPos + 3000) % items.size();
  return items[fPos].first + items[sPos].first + items[tPos].first;
}

static int itemCounter{};
struct Item {
  int id;
  long value;
  explicit Item(long value_): id(itemCounter++), value(value_) {
  }
};

auto part2(const auto &input) {
  auto lines = splitLines(input);
  std::vector<Item> items;
  items.reserve(lines.size());
  std::transform(lines.begin(), lines.end(), std::back_inserter(items), [](const auto &line) {
    return Item{static_cast<long>(readNumber(line.begin(), line.end())) * 811589153L};
  });
  const auto runShuffle = [&] {
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
      auto itemIterator = std::find_if(items.begin(), items.end(), [&](const auto &item) {
        return item.id == i;
      });
      if (itemIterator->value == 0) {
        continue;
      }
      const auto oldPos = std::distance(items.begin(), itemIterator);
      int pos = static_cast<int>((itemIterator->value + static_cast<long>(oldPos)) % static_cast<long>(items.size() - 1));
      if (pos < 0) {
        pos += static_cast<int>(items.size() - 1);
      }
      auto item = *itemIterator; // copy value
      items.erase(itemIterator);
      items.insert(items.begin() + pos, item);
    }
  };
  for (int i = 0; i < 10; ++i) {
    runShuffle();
  }
  const auto zeroPos = static_cast<size_t>(std::distance(items.begin(), std::find_if(items.begin(), items.end(), [] (const auto &item) {
    return item.value == 0;
  })));
  const auto fPos = (zeroPos + 1000) % items.size();
  const auto sPos = (zeroPos + 2000) % items.size();
  const auto tPos = (zeroPos + 3000) % items.size();
  return items[fPos].value + items[sPos].value + items[tPos].value;
}

void run() {
  const auto taskInput = readTaskInput(20);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
}

#endif// AOC_2022_CPP_LEVEL20_HPP
