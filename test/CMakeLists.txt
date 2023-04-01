cmake_minimum_required(VERSION 3.15...3.23)

project(CmakeConfigPackageTests LANGUAGES CXX)

# ---- Test as standalone project the exported config package ----

if(PROJECT_IS_TOP_LEVEL OR TEST_INSTALLED_VERSION)
  enable_testing()

  find_package(aoc_2022_cpp CONFIG REQUIRED) # for intro, project_options, ...

  if(NOT TARGET aoc_2022_cpp::project_options)
    message(FATAL_ERROR "Required config package not found!")
    return() # be strictly paranoid for Template Janitor github action! CK
  endif()
endif()

# ---- Dependencies ----

find_package(Catch2 CONFIG REQUIRED)

include(Catch)

add_library(catch_main OBJECT catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE aoc_2022_cpp::project_options)

add_executable(tests tests.cpp level1_tests.cpp level2_tests.cpp level3_tests.cpp level4_tests.cpp level5_tests.cpp
  level6_tests.cpp level7_tests.cpp level8_tests.cpp level9_tests.cpp level10_tests.cpp level11_tests.cpp
  level12_tests.cpp level13_tests.cpp level14_tests.cpp level15_tests.cpp level16_tests.cpp level17_tests.cpp)
target_link_libraries(tests PRIVATE project_warnings project_options catch_main solutions)

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  tests
  TEST_PREFIX
  "unittests."
  REPORTER
  xml
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)