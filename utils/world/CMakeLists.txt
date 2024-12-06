cmake_minimum_required(VERSION 3.10)
project(world)

add_library(world STATIC
  plane/direction.hpp
  plane/object.cpp
  plane/object.hpp
  plane/vec2.hpp
)

target_include_directories(world PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)