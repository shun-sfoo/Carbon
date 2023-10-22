cmake_minimum_required(VERSION 3.14)

project(Carbon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_TESTING ON)

add_subdirectory(common)
add_subdirectory(slot)

if (Qt6_FOUND OR DEFINED ENV{QT_DEV_DIR})
  add_subdirectory(capi)
endif()

if (X11_FOUND) 
  add_subdirectory(xapps)
endif()

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
