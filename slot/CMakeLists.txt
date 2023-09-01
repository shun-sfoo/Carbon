set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# header only use INTERFACE and no need file()
add_library(slot INTERFACE)
target_include_directories(slot INTERFACE include)
