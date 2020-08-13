cmake_minimum_required(VERSION 3.11)

project(BetterStdlib LANGUAGES CXX)

set(CXX_STANDARD 11)

enable_testing()

add_subdirectory(rand)
add_subdirectory(multithreading)