set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0)

FetchContent_MakeAvailable(Catch2)

add_executable(slot_test slot.cpp)
target_link_libraries(slot_test PRIVATE slot Catch2::Catch2WithMain)

add_executable(log_test log_test.cpp)
target_link_libraries(log_test PRIVATE common)

find_package(Threads REQUIRED)

macro(my_test name)
  add_executable(${name} thread/${name}.cpp)
  target_link_libraries(${name} PRIVATE Threads::Threads common)
endmacro()

my_test(thread1)
my_test(thread2)
my_test(async1)
my_test(async2)
my_test(promise)
my_test(future)
my_test(lock1)
my_test(lock2)
my_test(lock3)
my_test(lock4)
my_test(lock5)
my_test(lock6)
my_test(lock7)
my_test(lock8)
my_test(lock9)
my_test(vec1)
my_test(vec2)
my_test(vec3)
my_test(condition1)
my_test(condition2)
my_test(condition3)
my_test(condition4)
my_test(condition5)
my_test(atomic1)
my_test(atomic2)
my_test(atomic3)
my_test(atomic4)
my_test(atomic5)

include(Catch)
catch_discover_tests(slot_test)
