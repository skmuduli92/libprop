cmake_minimum_required(VERSION 3.10)

project(libprop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wno-strict-overflow")


# finding boost regexp component
find_package(Boost REQUIRED)

# finding  googltest framework installed in the system
enable_testing()
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)


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
  )

# building libprop test

if (BUILD_TESTS)
  message(STATUS "Build Tests: ON")

  add_executable(
    testlibprop
    test/testprop.cpp
    test/test_parser.cpp
    test/test_GPlus.cpp
    test/test_GMinus.cpp
    test/test_FPlus.cpp
    test/test_FMinus.cpp
    test/test_XPlus.cpp
    test/test_XMinus.cpp
    test/test_propvar.cpp
    test/test_until_operator.cpp
    test/test_termarray.cpp
    test/test_vartrace.cpp
    test/test_complexprops.cpp
    )

  target_link_libraries(testlibprop libprop gtest pthread)
  gtest_add_tests(TARGET testlibprop)

else ()
  message(STATUS "Build tests: OFF")
endif()

# Copying formula files
file(COPY extras/ DESTINATION ${CMAKE_BINARY_DIR}/bin)

add_executable(runlibprop src/TestLibProp.cpp)
target_link_libraries(runlibprop libprop)

