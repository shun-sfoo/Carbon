file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_library(reactor STATIC ${srcs})
target_include_directories(reactor PUBLIC include)
target_link_libraries(reactor PUBLIC logger)
