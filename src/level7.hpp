//
// Created by object on 4/03/23.
//

#ifndef AOC_2022_CPP_LEVEL7_HPP
#define AOC_2022_CPP_LEVEL7_HPP

#include <iostream>
#include <map>
#include <exception>
#include <memory>
#include <string>

#include "common.hpp"

namespace level7 {

class Directory {
  std::string path;
  Directory *parent;
  std::map<std::string_view, Directory*> children;
  unsigned long fileSize = 0;

public:
  Directory(const Directory &copy) = delete;
  Directory(Directory &&move) = default;
  explicit Directory(Directory *parentDir, const auto &dirName) : path(), parent(parentDir) {
    if (parentDir == nullptr) {
      path = std::string("/");
    } else {
      path = parentDir->path + std::string(dirName) + "/";
    }
  }

  ~Directory() {
    for (const auto &item : children) {
      delete item.second;
    }
  }

  Directory* getChild(const std::string_view &location)
  {
    if (location == "..") {
      return parent;
    }
    return children.at(location);
  }

  void addChildDir(const auto &dirName) {
    children.insert({dirName, new Directory(this, dirName)});
  }

  void addFile(auto size) {
    fileSize += size;
  }

  void traverse(auto f) const {
    for (const auto &entry : children) {
      entry.second->traverse(f);
    }
    f(*this);
  }

  auto getDirSize() const {
    unsigned long size = 0;
    traverse([&] (const Directory &dir) {
      size += dir.fileSize;
    });
    return size;
  }

  auto getDirPath() const {
    return path;
  }
};

auto processLs(auto &inputLines, auto inputEnd, Directory *current)
{
  auto readSize = [](const auto &line) {
    unsigned long size = 0;
    for (const auto &c : line) {
      if (c >= '0' && c <= '9') {
        size *= 10;
        size += static_cast<unsigned int>(c - '0');
      } else if (c == ' ') {
        break;
      } else {
        throw std::invalid_argument("can't read file size");
      }
    }
    return size;
  };
  while (inputLines != inputEnd && !(*inputLines).starts_with("$")) {
    auto line = *inputLines++;
    if (line.starts_with("dir ")) {
      current->addChildDir(line.substr(4));
    } else {
      current->addFile(readSize(line));
    }
  }
}

Directory *processCd(const std::string_view &location, Directory &root, Directory *current)
{
  Directory *base = location.starts_with("/") ? &root : current;
  size_t start = 0;
  auto end = location.find('/', start);
  while (end != std::string::npos) {
    if (start == end) {
      ++start;
    } else {
      auto childDir = location.substr(start, end);
      base = base->getChild(childDir);
      start = end + 1;
    }
    end = location.find('/', start);
  }
  if (start != location.size()) { // no trailing slash. eg: cd Downloads
    base = base->getChild(location.substr(start));
  }
  return base;
}


auto parseInput(const auto &input) {
  const auto lines = splitLines(input);
  auto current = lines.begin();
  Directory root = Directory{nullptr, ""};
  Directory *currentDir = &root;
  while (current != lines.end()) {
    auto command = *current++;
    if (command.starts_with("$ cd")) {
        currentDir = processCd(command.substr(5), root, currentDir);
    } else if (command =="$ ls") {
        processLs(current, lines.end(), currentDir);
    } else {
        throw std::invalid_argument("unexpected command");
    }
  }
  return root;
}

auto part1(const auto &input) {
  auto root = parseInput(input);
  unsigned int result = 0;
  root.traverse([&](const Directory &dir) {
    if (dir.getDirSize() < 100'000) {
      result += static_cast<unsigned int>(dir.getDirSize());
    }
  });
  return result;
}

auto part2(const auto &input) {
  auto root = parseInput(input);
  const auto minimumToDelete = root.getDirSize() - 40'000'000;
  const auto *directoryToDelete = &root;
  root.traverse([&](const Directory &dir) {
    if (dir.getDirSize() >= minimumToDelete && dir.getDirSize() < directoryToDelete->getDirSize()) {
      directoryToDelete = &dir;
    }
  });
  return directoryToDelete->getDirSize();
}

void run() {
  const auto *taskInput = readTaskInput(7);
  std::cout << part1(taskInput) << '\n';
  std::cout << part2(taskInput) << '\n';
}
};// namespace level7

#endif// AOC_2022_CPP_LEVEL7_HPP
