cmake_minimum_required(VERSION 3.10)
project(utils)

add_subdirectory(io)
add_subdirectory(world)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
