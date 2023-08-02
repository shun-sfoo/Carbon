set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.hpp)
add_library(neo STATIC ${srcs})
target_include_directories(neo PUBLIC include)
