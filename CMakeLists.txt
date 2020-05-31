cmake_minimum_required(VERSION 3.10)

project(libprop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wno-strict-overflow")


# finding boost regexp component
find_package(Boost REQUIRED)

# finding  googltest framework installed in the system
find_package(Threads REQUIRED)

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE )
endif()

# print build type to CMAKE console output
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
message(STATUS "Configuring build type: ${CMAKE_BUILD_TYPE}")

include_directories(include)

# setting binary and library output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# building libprop library
add_library(
  libprop
  src/formula.cpp
  src/formula_util.cpp
  src/parser.cpp
  src/trace.cpp
  )

# building libprop test
if (CMAKE_BUILD_TYPE MATCHES DEBUG) 
  enable_testing()
  find_package(GTest REQUIRED)
  message(STATUS "Build Tests: ON")
  file(GLOB TEST_SRC "test/*.cpp")
  add_executable(testlibprop ${TEST_SRC})
  target_link_libraries(testlibprop libprop gtest pthread)
  gtest_add_tests(TARGET testlibprop)
else ()
  message(STATUS "Build tests: OFF")
endif()
