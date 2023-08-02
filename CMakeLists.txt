cmake_minimum_required(VERSION 3.11)

project(Carbon)

add_subdirectory(neo)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
