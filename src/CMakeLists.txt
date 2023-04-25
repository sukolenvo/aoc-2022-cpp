add_library(solutions INTERFACE)
target_include_directories(solutions INTERFACE .)
add_executable(aoc main.cpp)
target_link_libraries(
  aoc
  PUBLIC project_options project_warnings solutions)

add_custom_command(TARGET aoc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/input ${CMAKE_CURRENT_BINARY_DIR}/input)

target_include_directories(aoc PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
