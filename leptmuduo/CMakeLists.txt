cmake_minimum_required(VERSION 3.11)

project(leptmuduo CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(base)
add_subdirectory(logger)
add_subdirectory(reactor)
add_subdirectory(executable)


