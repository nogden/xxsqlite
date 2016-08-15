project(++sqlite)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(BUILD_UNIT_TESTS "Build the unit tests" ON)

find_package(GTest QUIET)
if(GTEST_FOUND)
  enable_testing()
  include_directories(${GTEST_INCLUDE_DIRS})
else()
  message("### Google Test Framework not found, unit tests will not be built.")
  set(BUILD_UNIT_TESTS false)
endif()

find_package(sqlite REQUIRED)
if(NOT SQLITE_FOUND)
	message(FATAL_ERROR "SQLite required for ++sqlite")
endif()

set(XXSQLITE_LIBRARIES ++sqlite CACHE INTERNAL "")
set(XXSQLITE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE INTERNAL "")

add_definitions("-std=c++11")

add_subdirectory(src)
