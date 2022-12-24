# Generic test that uses conan libs
add_library(solutions OBJECT level1.hpp)
set_target_properties(solutions PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(solutions PRIVATE project_options project_warnings)
add_executable(aoc main.cpp)
target_link_libraries(
  aoc
  PUBLIC project_options project_warnings
  PRIVATE solutions)

target_include_directories(aoc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
