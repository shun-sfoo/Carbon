file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(04_server_lib STATIC ${srcs})
target_include_directories(04_server_lib PUBLIC include)

add_executable(04_server server.cpp)
target_link_libraries(04_server  04_server_lib)

add_executable(04_client client.cpp)
target_link_libraries(04_client  04_server_lib)
