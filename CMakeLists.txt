cmake_minimum_required(VERSION 3.14)

project(Carbon C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(BUILD_TESTING ON)

macro(deps name)
  include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${name}.cmake")
endmacro()

deps(catch)
deps(json)
deps(spdlog)

add_subdirectory(common)
add_subdirectory(slot)
add_subdirectory(desktop)
add_subdirectory(tests)
