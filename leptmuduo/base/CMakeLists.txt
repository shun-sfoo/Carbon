file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)

add_library(base STATIC ${srcs})
target_include_directories(base PUBLIC include)
