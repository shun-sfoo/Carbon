find_package(Qt6)
if(Qt6_FOUND)
  add_subdirectory(capi)
endif()

if(X11_FOUND)
  add_subdirectory(xapps)
endif()

# cmake use glib-2
# https://cmake.org/cmake/help/latest/module/FindPkgConfig.html
# https://stackoverflow.com/questions/36868143/
# what-is-the-recommended-way-of-using-glib2-with-cmake
# https://itecnote.com/tecnote/the-recommended-way-of-using-glib2-with-cmake/
find_package(PkgConfig)
pkg_check_modules(glib IMPORTED_TARGET glib-2.0)
pkg_check_modules(gobject IMPORTED_TARGET gobject-2.0)
if(glib_FOUND AND gobject_FOUND)
  add_subdirectory(coop)
endif()

# message("the value " ${glib_FOUND})
# message("the value " ${glib_LIBRARIES})
# message("the value " ${glib_LINK_LIBRARIES})
# message("the value " ${glib_LIBRARY_DIRS})
