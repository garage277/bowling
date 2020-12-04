cmake_minimum_required(VERSION 3.7)

project(bowling)

add_subdirectory(tests)

file(GLOB S1 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB H1 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
add_executable(console_bowling ${H1} ${S1})
