set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_executable(conn ${srcs})

target_include_directories(conn PRIVATE include)
target_link_libraries(conn PRIVATE PkgConfig::glib PkgConfig::gobject)
