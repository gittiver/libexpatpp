Include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.10.0)

FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
  FetchContent_Populate(catch2)

  # Set any custom variables, etc. here, then
  # populate the content as part of this build

  add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
endif()


enable_testing()
## these are unittests that can be run on any platform
add_executable(test_parse
  test_parsefile.cpp
  test_parse.cpp
)
target_link_libraries(test_parse Catch2::Catch2WithMain expatpp)
add_test(test_parse test_parse)

add_executable(test_parser_callbacks
  test_parser_callbacks.cpp
)
target_link_libraries(test_parser_callbacks Catch2::Catch2WithMain expatpp)
add_test(test_parser_callbacks test_parser_callbacks)

#test for issue #6
add_executable(test_asam_generation_problem
  test_asam_generation_problem.cpp
)
target_link_libraries(test_asam_generation_problem Catch2::Catch2WithMain expatpp)
add_test(test_asam_generation_problem test_asam_generation_problem)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --extra-verbose)
