file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(05_server_lib STATIC ${srcs})
target_include_directories(05_server_lib PUBLIC include)

add_executable(05_server server.cpp)
target_link_libraries(05_server 05_server_lib)

add_executable(05_client client.cpp)
target_link_libraries(05_client 05_server_lib)
