cmake_minimum_required(VERSION 3.23)
project("AllocatorCpp")


set(CMAKE_CXX_STANDARD 23)
include(${CMAKE_SOURCE_DIR}/lib/Dependencies.cmake)

add_library(
    AllocatorCpp
        ./src/allocator/linear_allocator.cpp
)
target_include_directories(AllocatorCpp
    PUBLIC
        ./src/
)


enable_testing()
add_executable(AllocatorCppTest
    ./tests/linear_allocator_test.cpp
)
target_link_libraries(
    AllocatorCppTest
    PRIVATE
        AllocatorCpp
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(AllocatorCppTest)
