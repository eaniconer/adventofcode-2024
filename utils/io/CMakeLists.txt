cmake_minimum_required(VERSION 3.10)
project(io)

add_library(io STATIC
  file_reader.cpp
  file_reader.hpp
)

target_include_directories(io PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)