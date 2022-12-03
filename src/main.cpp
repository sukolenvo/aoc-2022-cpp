#include <functional>
#include <optional>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `aoc_2022_cpp`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#include "level1.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  const std::array levels{
    std::function(&level1),
  };
  CLI::App app{ fmt::format("{} version {}", aoc_2022_cpp::cmake::project_name, aoc_2022_cpp::cmake::project_version) };

  size_t level = levels.size() - 1;
  app.add_option("-l,--level", level, fmt::format("Run specific level (1..{}). Default is last level", levels.size()));
  bool show_version = false;
  app.add_flag("--version", show_version, "Show version information");

  CLI11_PARSE(app, argc, argv);

  if (show_version) {
    fmt::print("{}\n", aoc_2022_cpp::cmake::project_version);
    return EXIT_SUCCESS;
  }

  spdlog::info("Level selected: '{}'", level);

  if (level == 0) { spdlog::error("Level should be a positive number 1..{}", levels.size()); }
  if (level > levels.size()) { spdlog::error("Max level is {}", levels.size()); }
  levels.at(level - 1)();

  return EXIT_SUCCESS;
}
