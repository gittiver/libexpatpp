cmake_minimum_required(VERSION 3.15)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# This allows controlling documented build time switches
# when project is pulled in using the add_subdirectory function, e.g.
#
#   set(EXPATPP_BUILD_DOCS OFF)
#   add_subdirectory(${expatpp_SOURCE_DIR} ${expatpp_BINARY_DIR})
#
# Without activating behavior NEW for policy CMP0077 here,
# a user with -Wdev enabled would see warning
#
#   Policy CMP0077 is not set: option() honors normal variables.  Run "cmake
#   --help-policy CMP0077" for policy details.  Use the cmake_policy command to
#   set the policy and suppress this warning.
#
#   For compatibility with older versions of CMake, option is clearing the
#   normal variable 'EXPATPP_BUILD_DOCS'.
#
# and effectively not be able to adjust option EXPATPP_BUILD_DOCS.
#
# For more details please see:
# - https://cmake.org/cmake/help/latest/policy/CMP0077.html
#
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW)
endif()

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

project(expatpp
    VERSION
        0.1.7
    LANGUAGES
        CXX
)

set(PACKAGE_BUGREPORT "gittiver@traumkristalle.net")
set(PACKAGE_NAME "expatpp")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE_NAME}")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FetchContent)

#
# Configuration
#
option(EXPATPP_BUILD_EXAMPLES "build the examples for expatpp library" ON)
option(EXPATPP_BUILD_TOOLS "build the tools for expatpp library(xsdgen)" OFF)
option(EXPATPP_BUILD_TESTS "build the tests for expatpp library" ON)
option(EXPATPP_SHARED_LIBS "build a shared expatpp library" OFF)
option(EXPATPP_BUILD_DOCS "build api documentation" ${_EXPATPP_BUILD_DOCS_DEFAULT})
option(EXPATPP_ENABLE_INSTALL "install expatpp files in cmake install target" ON)
option(EXPATPP_WARNINGS_AS_ERRORS "Treat all compiler warnings as errors" OFF)

#
# Environment checks
#

#include(${CMAKE_CURRENT_LIST_DIR}/ConfigureChecks.cmake)

configure_file(expatpp_config.h.cmake "${CMAKE_CURRENT_BINARY_DIR}/expatpp_config.h")
add_definitions(-DHAVE_EXPATPP_CONFIG_H)
#expat_install(FILES "${CMAKE_CURRENT_BINARY_DIR}/expatpp_config.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(EXTRA_COMPILE_FLAGS)
if (EXPATPP_WARNINGS_AS_ERRORS)
    if(MSVC)
        add_definitions(/WX)
    else(MSVC)
        set(EXTRA_COMPILE_FLAGS "${EXTRA_COMPILE_FLAGS} -Werror")
    endif(MSVC)
endif(EXPATPP_WARNINGS_AS_ERRORS)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}")

if (MSVC)
    if (EXPAT_MSVC_STATIC_CRT)
        message("-- Using static CRT ${EXPAT_MSVC_STATIC_CRT}")
        foreach(flag_var
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL
                CMAKE_CXX_FLAGS_RELWITHDEBINFO
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL
                CMAKE_C_FLAGS_RELWITHDEBINFO
                )
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endforeach()
    endif()
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -wd4996)
endif(MSVC)

#handle expat dependency
if(EXPATPP_USE_SYSTEM_EXPAT)
include (${CMAKE_ROOT}/Modules/FindEXPAT.cmake)
mark_as_advanced(EXPAT_INCLUDE_DIRS EXPAT_LIBRARIES EXPAT_FOUND)
endif(EXPATPP_USE_SYSTEM_EXPAT)
  
if (NOT EXPAT_FOUND)
  FetchContent_Declare(expat
  		 GIT_REPOSITORY https://github.com/libexpat/libexpat.git
		 GIT_TAG R_2_6_2)
  if(NOT expat_POPULATED)

    set(EXPAT_SHARED_LIBS Off)
    set(EXPAT_BUILD_TOOLS Off)
    set(EXPAT_BUILD_EXAMPLES Off)
    set(EXPAT_BUILD_TESTS Off)
    set(EXPAT_SHARED_LIBS Off)
    set(EXPAT_BUILD_DOCS Off)
    set(EXPAT_WARNINGS_AS_ERRORS Off)
    set(EXPAT_BUILD_FUZZERS Off)
    set(EXPAT_WITH_LIBBSD Off)
    set(EXPAT_ENABLE_INSTALL On)
    FetchContent_Populate(expat)

    add_subdirectory(${expat_SOURCE_DIR}/expat ${expat_BINARY_DIR})
  endif()
  set(EXPAT_LIBRARIES expat)
  get_target_property(EXPAT_INCLUDE_DIRS expat INCLUDE_DIRECTORIES)

endif (NOT EXPAT_FOUND)

#
# the C++ expatpp library target
#
set(expatpp_HEADERS
    src/expatpp.hpp
    src/xmlparser.hpp
    src/delegate.hpp
    src/generator.hpp
    src/state.hpp
)

set(expatpp_SRCS
    ${expatpp_HEADERS}
    src/xmlparser.cpp
    src/delegate.cpp
    src/generator.cpp
    src/state.cpp
)

if(EXPATPP_SHARED_LIBS)
    set(_SHARED SHARED)
else(EXPATPP_SHARED_LIBS)
    set(_SHARED STATIC)
endif(EXPATPP_SHARED_LIBS)

add_library(expatpp ${_SHARED} ${expatpp_SRCS})
set_target_properties(expatpp PROPERTIES POSITION_INDEPENDENT_CODE True)
target_link_libraries(expatpp expat)

set(LIBCURRENT 0)    # sync
set(LIBREVISION 1)  # with
set(LIBAGE 0)        # configure.ac!
math(EXPR LIBCURRENT_MINUS_AGE "${LIBCURRENT} - ${LIBAGE}")

set_property(TARGET expatpp PROPERTY OUTPUT_NAME "${_EXPATPP_OUTPUT_NAME}")
if(NOT WIN32)
    set_property(TARGET expatpp PROPERTY VERSION ${LIBCURRENT_MINUS_AGE}.${LIBAGE}.${LIBREVISION})
    set_property(TARGET expatpp PROPERTY SOVERSION ${LIBCURRENT_MINUS_AGE})
    set_property(TARGET expatpp PROPERTY NO_SONAME ${NO_SONAME})
endif(NOT WIN32)

target_include_directories(expatpp
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS expatpp EXPORT expatpp
                      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		      )

if(EXPATPP_BUILD_DOCS)
  add_subdirectory(docs/doxygen)
endif()

#
# build tools
#
if(EXPATPP_BUILD_TOOLS)
  add_executable(xsdgen src/xsdgen/xsdgen.cpp)
  target_link_libraries(xsdgen expatpp)
endif(EXPATPP_BUILD_TOOLS)

#
# C++ code examples
#
if(EXPATPP_BUILD_EXAMPLES)
  add_executable(doxyxml examples/doxyxml.cpp examples/doxyxml.hpp)
  target_link_libraries(doxyxml expatpp)
  set_property(TARGET doxyxml PROPERTY RUNTIME_OUTPUT_DIRECTORY examples)
endif(EXPATPP_BUILD_EXAMPLES)

#
# C/C++ test runners
#
if(EXPATPP_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif(EXPATPP_BUILD_TESTS)

export(
    TARGETS
        expatpp
    FILE
        cmake/expatpp-targets.cmake  # not going to be installed
)

#
# Summary
#
string(TOUPPER "${CMAKE_BUILD_TYPE}" _EXPAT_BUILD_TYPE_UPPER)

#
# packaging   
#
include(CPack)
include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR TGZ ZIP)
if(APPLE)
  if(${CMAKE_VERSION} VERSION_LESS "3.13.0") 
    message("Please consider to switch to CMake 3.13.0 for OSX DragAndDrop Installer")
  else(${CMAKE_VERSION} VERSION_LESS "3.13.0") 
    set(CPACK_GENERATOR DragNDrop ${CPACK_GENERATOR})
  endif(${CMAKE_VERSION} VERSION_LESS "3.13.0")  
endif(APPLE)

set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-src-${PROJECT_VERSION})
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_PACKAGE_CONTACT https://github.com/gittiver/libexpatpp/issues) 
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "expatpp Development Team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR}/**;/build/**;/.git/")

if(WIN32 AND NOT UNIX)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}\\\\${PROJECT_VERSION}")
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlashes.
  # set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
else(WIN32 AND NOT UNIX)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}/${PROJECT_VERSION}")
  set(CPACK_STRIP_FILES "bin/${PROJECT_NAME}-gen")
  set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
#set(CPACK_PACKAGE_EXECUTABLES ???)

set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-src-${PROJECT_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
        | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

message(STATUS "===========================================================================")
message(STATUS "")
message(STATUS "Configuration")
message(STATUS "  Prefix ..................... ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build type ................. ${CMAKE_BUILD_TYPE}")
message(STATUS "  Shared libraries ........... ${EXPATPP_SHARED_LIBS}")
if(WIN32)
    message(STATUS "  Binary postfix ............. ${CMAKE_${_EXPATPP_BUILD_TYPE_UPPER}_POSTFIX}")
endif()
message(STATUS "")
message(STATUS "  Build documentation ........ ${EXPATPP_BUILD_DOCS}")
message(STATUS "  Build examples ............. ${EXPATPP_BUILD_EXAMPLES}")
message(STATUS "  Build tools(xsdgen)......... ${EXPATPP_BUILD_TOOLS}")
message(STATUS "  Build tests ................ ${EXPATPP_BUILD_TESTS}")
message(STATUS "")
message(STATUS "  Features")
message(STATUS "")

if(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    message(STATUS "Continue with")
    message(STATUS "  make")
    if(EXPAT_BUILD_TESTS)
        message(STATUS "  make test")
    endif()
    if(EXPAT_ENABLE_INSTALL)
        message(STATUS "  sudo make install")
    endif()
    message(STATUS "")
endif()
message(STATUS "===========================================================================")
