add_executable(aoc level1.hpp main.cpp)
target_link_libraries(
  aoc
  PUBLIC project_options project_warnings)

target_include_directories(aoc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
