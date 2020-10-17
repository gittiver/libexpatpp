Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

enable_testing()
## these are unittests that can be run on any platform
add_executable(test_parser
  test_main.cpp
  test_parsefile.cpp
)
target_link_libraries(test_parser Catch2::Catch2 expatpp)
add_test(test_parser test_parser)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --extra-verbose)
