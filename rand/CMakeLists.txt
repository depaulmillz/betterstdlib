cmake_minimum_required(VERSION 3.11)

project(BetterStdlibRand VERSION "0.0.0" LANGUAGES CXX)

set(CXX_STANDARD 11)

add_library(rand SHARED src/zipf.cpp)
target_include_directories(rand PUBLIC include)

add_library(rand_static STATIC src/zipf.cpp)
target_include_directories(rand_static PUBLIC include)

enable_testing()
add_subdirectory(test)