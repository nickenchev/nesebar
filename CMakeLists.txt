cmake_minimum_required(VERSION 3.2)
project(nesebar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(nesebar
  src/main.cpp
  src/core6502.cpp
  src/nes.cpp
  src/nescart.cpp)

target_compile_options(nesebar PUBLIC -Wall -Wextra -Werror)
