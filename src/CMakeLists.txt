add_library(solutions INTERFACE)
target_include_directories(solutions INTERFACE .)
add_executable(aoc main.cpp)
target_link_libraries(
  aoc
  PUBLIC project_options project_warnings solutions)

target_include_directories(aoc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
