cmake_minimum_required(VERSION 3.14)
project(PriorityQueue)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    src
)

# Main executable with all required sources
add_executable(queue_app 
    src/main.cpp
)

# GoogleTest configuration
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Test executable with required sources
add_executable(queue_tests
    tests/queue_tests.cpp
)

add_executable(queue_benchmark
  tests/queue_benchmark.cpp
)

target_link_libraries(queue_tests
    GTest::gtest_main
    pthread
)

enable_testing()
add_test(NAME queue_tests COMMAND queue_tests)
add_test(NAME queue_benchmark COMMAND queue_benchmark)
