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

file(GLOB SRC "src/*.cpp")

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
  set_property(SOURCE ${SRC} APPEND PROPERTY COMPILE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
endif()

# building libprop library
add_library(libprop ${SRC})

# building libprop test
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
  enable_testing()
  find_package(GTest REQUIRED)
  message(STATUS "Build Tests: ON")
  file(GLOB TEST_SRC "test/*.cpp")
  add_executable(testlibprop ${TEST_SRC})
  target_link_libraries(testlibprop libprop gtest pthread -lgcov)
  gtest_add_tests(TARGET testlibprop)

  add_custom_target( covreport
  COMMAND mkdir -p Coverage
  COMMAND bin/testlibprop
  COMMAND cd Coverage && lcov  --directory ${CMAKE_BINARY_DIR}/CMakeFiles/libprop.dir/src/ -t "result" -o cppagent_coverage.info -c -d .
  COMMAND cd Coverage && lcov --remove cppagent_coverage.info -o cppagent_coverage.info '/usr*' '/usr/include/boost*'
  COMMAND cd Coverage && genhtml cppagent_coverage.info -o html
  COMMENT "Generated Coverage Report Successfully!"
  )

else ()
  message(STATUS "Build tests: OFF")
  message(STATUS "Coverage generation: OFF")
endif()

