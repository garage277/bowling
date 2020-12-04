cmake_minimum_required(VERSION 3.7)

project(bowling-tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(.. ${GTEST_INCLUDE_DIRS})

add_executable(bowling_test test.cpp)

target_link_libraries(bowling_test ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
add_test(bowling_test "./bowling_test")

