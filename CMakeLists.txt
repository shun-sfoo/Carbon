cmake_minimum_required(VERSION 3.11)

project(Carbon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TESTING ON)

add_subdirectory(slot)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
