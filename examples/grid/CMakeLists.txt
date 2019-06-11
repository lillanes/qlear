cmake_minimum_required(VERSION 3.5)
project(grid
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++17")

find_package(qlear REQUIRED)

set(sources
  "${CMAKE_CURRENT_SOURCE_DIR}/grid.cc")
add_executable(grid ${sources})

target_link_libraries(grid PUBLIC qlear::qlear)
