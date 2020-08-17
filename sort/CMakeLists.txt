cmake_minimum_required(VERSION 3.3.2)

add_executable(check_correct_sorting correctness_check.cpp)
add_subdirectory(benchmarks)