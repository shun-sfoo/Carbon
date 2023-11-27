cmake_minimum_required(VERSION 3.14)

project(Carbon C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(BUILD_TESTING ON)

add_subdirectory(slot)

if(Qt6_FOUND OR DEFINED ENV{QT_DEV_DIR})
  add_subdirectory(capi)
endif()

if(X11_FOUND)
  add_subdirectory(xapps)
endif()

# cmake use glib-2
# https://cmake.org/cmake/help/latest/module/FindPkgConfig.html
# https://stackoverflow.com/questions/36868143/what-is-the-recommended-way-of-using-glib2-with-cmake
# https://itecnote.com/tecnote/the-recommended-way-of-using-glib2-with-cmake/
find_package(PkgConfig)
pkg_check_modules(glib IMPORTED_TARGET glib-2.0)
pkg_check_modules(gobject IMPORTED_TARGET gobject-2.0)
if(glib_FOUND AND gobject_FOUND)
  add_subdirectory(coop)
endif()

# message("the value " ${deps_FOUND})
# message("the value " ${deps_LIBRARIES})
# message("the value " ${deps_LINK_LIBRARIES})
# message("the value " ${deps_LIBRARY_DIRS})

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(common)
  add_subdirectory(tests)
endif()
