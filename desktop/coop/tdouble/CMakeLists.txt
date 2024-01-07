set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c include/*.h)

add_executable(tdouble ${srcs})

target_include_directories(tdouble PRIVATE include)
target_link_libraries(tdouble PRIVATE PkgConfig::glib PkgConfig::gobject)
