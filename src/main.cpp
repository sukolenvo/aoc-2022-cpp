#include <iostream>
#include <functional>
#include <span>
#include <string>

#include "level1.hpp"

int main(int argc, const char **argv)
{
  try {
    const std::array levels{
      std::function(&level1),
    };
    size_t level = 1;
    if (argc == 2) {
      auto args = std::span(argv, size_t(argc));
      level = size_t(std::stoi(args[1]));
    }
    if (level < 1 || level > levels.size()) {
      std::cout << "Level should be 1.." << levels.size() << '\n';
      return EXIT_FAILURE;
    }
    levels.at(level - 1)();

    return EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Unhandled exception in main :";
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
}
