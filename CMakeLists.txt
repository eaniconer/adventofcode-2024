cmake_minimum_required(VERSION 3.10)
project(adventofcode2024)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_subdirectory(utils)

add_subdirectory(day-06)
add_subdirectory(day-07)
add_subdirectory(day-08)
add_subdirectory(day-09)
add_subdirectory(day-10)
