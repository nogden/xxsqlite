project(++sqlite)
cmake_minimum_required(VERSION 2.8)

option(BUILD_UNIT_TESTS "Build the unit tests" ON)

find_package(GTest QUIET)
if(GTEST_FOUND)
  enable_testing()
  include_directories("${GTEST_INCLUDE_DIRS}")
else()
  message("### Google Test Framework not found, unit tests will not be built.")
  set(BUILD_UNIT_TESTS false)
endif()

add_definitions("-std=c++11")

add_subdirectory(src)
