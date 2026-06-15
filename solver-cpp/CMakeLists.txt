cmake_minimum_required(VERSION 3.16)
project(sudoku_solver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_library(sudoku_board
    src/board.cpp
    src/backtracking_solver.cpp
)

target_include_directories(sudoku_board PUBLIC include)

enable_testing()

add_executable(test_board tests/test_board.cpp)
target_link_libraries(test_board PRIVATE sudoku_board)
target_compile_definitions(test_board PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}/..")
add_test(NAME board_tests COMMAND test_board)

add_executable(test_backtracking_solver tests/test_backtracking_solver.cpp)
target_link_libraries(test_backtracking_solver PRIVATE sudoku_board)
target_compile_definitions(test_backtracking_solver PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}/..")
add_test(NAME backtracking_solver_tests COMMAND test_backtracking_solver)

add_executable(bench_backtracking bench/bench_backtracking.cpp)
target_link_libraries(bench_backtracking PRIVATE sudoku_board)
target_compile_definitions(bench_backtracking PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}/..")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()