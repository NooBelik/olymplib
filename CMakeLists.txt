cmake_minimum_required(VERSION 2.8)
                                               
option(test "Build all tests." OFF)

set(PROJECT_NAME MyProject)
project(${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -O2")

add_library(act act.cpp)
add_executable(main main.cpp)

target_link_libraries(main act)

if (APPLE)
  add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
  add_definitions(-D__GLIBCXX__)
endif (APPLE)
                                                               
add_subdirectory(gtest)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

##############
# Unit Tests
##############
add_executable(act_ut act_ut.cpp)

target_link_libraries(act_ut gtest gtest_main)
target_link_libraries(act_ut act)

add_test(NAME act COMMAND act_ut)
