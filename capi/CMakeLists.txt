set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED ENV{QT_DEV_DIR})
  message(FATAL_ERROR ">>> not define 'QT_DEV_DIR' in env path, please add it. <<<")
else ()
  set(CMAKE_PREFIX_PATH $ENV{QT_DEV_DIR})
endif ()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(capi capi.cpp)
target_link_libraries(capi PRIVATE Qt6::Widgets)
