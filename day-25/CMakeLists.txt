project(day-25)

add_executable(${PROJECT_NAME}-part1 part-1.cpp)
add_executable(${PROJECT_NAME}-part2 part-2.cpp)

target_link_libraries(${PROJECT_NAME}-part1 PUBLIC io)
target_link_libraries(${PROJECT_NAME}-part2 PUBLIC io)

target_include_directories(${PROJECT_NAME}-part1 PUBLIC ${io_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}-part2 PUBLIC ${io_SOURCE_DIR})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
  ${CMAKE_CURRENT_BINARY_DIR}/input.txt
  COPYONLY
)
