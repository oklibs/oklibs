# Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.20)
project(oktest VERSION 0.1.0 LANGUAGES CXX)

set(OKTEST_MAX_NESTED 8 CACHE STRING "Maximum nesting level for test cases and sections.")
option(OKTEST_WITH_EXCEPTIONS "Allow exception usage." ON)
option(OKTEST_LINK_MAIN "Provide a main function in a separate source file." ON)

if(OKTEST_WITH_EXCEPTIONS)
    set(OKTEST_WITH_EXCEPTIONS_VALUE 1)
else()
    set(OKTEST_WITH_EXCEPTIONS_VALUE 0)
endif()

if(OKTEST_LINK_MAIN)
    set(OKTEST_LINK_MAIN_VALUE 1)
else()
    set(OKTEST_LINK_MAIN_VALUE 0)
endif()

configure_file(
    cmake/config.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/oktest/config.hpp"
    @ONLY
)

add_library(oktest STATIC
    source/logger.cpp
    source/main.cpp
)
add_library(oklibs::oktest ALIAS oktest)

target_include_directories(oktest PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(oktest PUBLIC cxx_std_20)

target_compile_definitions(oktest PUBLIC OKTEST_STATIC)

# Dependencies
if(TARGET okutils)
    target_link_libraries(oktest PUBLIC okutils)
else()
    find_package(okutils REQUIRED)
    target_link_libraries(oktest PUBLIC oklibs::okutils)
endif()

if(TARGET okbitflag)
    target_link_libraries(oktest PUBLIC okbitflag)
else()
    find_package(okbitflag REQUIRED)
    target_link_libraries(oktest PUBLIC oklibs::okbitflag)
endif()

find_package(fmt REQUIRED)
target_link_libraries(oktest PUBLIC fmt::fmt)

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS oktest EXPORT oktest-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/oktest
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/oktest"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT oktest-targets
    FILE oktest-targets.cmake
    NAMESPACE oklibs::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/oktest
)

configure_package_config_file(
    cmake/oktest-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/oktest-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/oktest
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/oktest-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/oktest-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/oktest-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/oktest
)
