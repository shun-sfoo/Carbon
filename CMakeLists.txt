cmake_minimum_required(VERSION 3.14)

project(Carbon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TESTING ON)

add_subdirectory(slot)
add_subdirectory(capi)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
