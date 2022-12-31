#include <iostream>
#include <functional>
#include <span>
#include <string>

#include "level1.hpp"
#include "level2.hpp"

int main(int argc, const char **argv) // NOLINT(bugprone-exception-escape) check is unreliable on Windows, see: https://stackoverflow.com/questions/61014184/clang-tidys-bugprone-exception-escape-behaves-weirdly-with-msvc-stl
{
  try {
    const std::array levels{
      std::function(&level1::run),
      std::function(&level2::run),
    };
    size_t level = levels.size();
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
    std::cerr << "Unhandled exception in main :" << e.what();
    return EXIT_FAILURE;
  }
}
