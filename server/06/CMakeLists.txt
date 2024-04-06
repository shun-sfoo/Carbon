file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(06_server_lib STATIC ${srcs})
target_include_directories(06_server_lib PUBLIC include)

add_executable(06_serv serv.cpp)
target_link_libraries(06_serv 06_server_lib)
