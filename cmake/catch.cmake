Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.1
)
FetchContent_MakeAvailable(Catch2)
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
