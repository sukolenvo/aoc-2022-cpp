#include <iostream>
#include <functional>
#include <span>
#include <string>

#include "level1.hpp"
#include "level2.hpp"
#include "level3.hpp"
#include "level4.hpp"
#include "level5.hpp"
#include "level6.hpp"
#include "level7.hpp"
#include "level8.hpp"
#include "level9.hpp"
#include "level10.hpp"
#include "level11.hpp"
#include "level12.hpp"
#include "level13.hpp"
#include "level14.hpp"
#include "level15.hpp"
#include "level16.hpp"
#include "level17.hpp"
#include "level18.hpp"

int main(int argc, const char **argv) // NOLINT(bugprone-exception-escape) check is unreliable on Windows, see: https://stackoverflow.com/questions/61014184/clang-tidys-bugprone-exception-escape-behaves-weirdly-with-msvc-stl
{
  try {
    const std::array levels{
      std::function(&level1::run),
      std::function(&level2::run),
      std::function(&level3::run),
      std::function(&level4::run),
      std::function(&level5::run),
      std::function(&level6::run),
      std::function(&level7::run),
      std::function(&level8::run),
      std::function(&level9::run),
      std::function(&level10::run),
      std::function(&level11::run),
      std::function(&level12::run),
      std::function(&level13::run),
      std::function(&level14::run),
      std::function(&level15::run),
      std::function(&level16::run),
      std::function(&level17::run),
      std::function(&level18::run),
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
