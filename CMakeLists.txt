cmake_minimum_required(VERSION 3.2)
project(nesebar)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "-std=c++14")

add_executable(nesebar
  src/main.cpp
  src/core6502.cpp
  src/nes.cpp
  src/nescart.cpp)
