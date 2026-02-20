Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/Catch2
)

FetchContent_MakeAvailable(Catch2)
