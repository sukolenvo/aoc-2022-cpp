add_library(solutions common.hpp level1.hpp)
target_link_libraries(solutions PRIVATE project_options project_warnings )
set_target_properties(solutions PROPERTIES LINKER_LANGUAGE CXX)
add_executable(aoc main.cpp)
target_link_libraries(
  aoc
  PUBLIC project_options project_warnings solutions)

target_include_directories(aoc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
