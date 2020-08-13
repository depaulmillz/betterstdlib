cmake_minimum_required(VERSION 3.11)

project(BetterStdlibMultithreading VERSION "0.0.0" LANGUAGES CXX)

set(CXX_STANDARD 11)

add_library(multithreading INTERFACE)
target_include_directories(multithreading INTERFACE include)

enable_testing()
add_subdirectory(test)