cmake_minimum_required(VERSION 3.1.3)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

project(expatpp_hello_world VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)
#
# expatpp Configuration variables
#
set(EXPATPP_BUILD_EXAMPLES Off)
set(EXPATPP_BUILD_TOOLS Off)
set(EXPATPP_BUILD_TESTS Off)
set(EXPATPP_BUILD_DOCS Off)

# add expatpp project to the build
FetchContent_Declare(expatpp
  		 GIT_REPOSITORY https://github.com/gittiver/libexpatpp.git
		 GIT_TAG main
		 )

if(NOT expatpp_POPULATED)
  FetchContent_Populate(expatpp)
  add_subdirectory(${expatpp_SOURCE_DIR} ${expatpp_BINARY_DIR})
endif(NOT expatpp_POPULATED)

# add executable
add_executable(hello_world hello_world.cpp)

# link against expatpp library (produced by expatpp)
target_link_libraries(hello_world PUBLIC expatpp)
