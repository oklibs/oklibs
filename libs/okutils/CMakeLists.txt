# Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.20)
project(okutils VERSION 0.1.0 LANGUAGES CXX)

add_library(okutils INTERFACE)
add_library(oklibs::okutils ALIAS okutils)

configure_file(
    cmake/config.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/okutils/config.hpp"
    @ONLY
)

target_include_directories(okutils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(okutils INTERFACE cxx_std_20)

target_compile_definitions(okutils INTERFACE
    $<$<CONFIG:Debug>:OKL_BUILD_DEBUG=1>
    $<$<CONFIG:RelWithDebInfo>:OKL_BUILD_RELEASEDBG=1>
    $<$<CONFIG:Release>:OKL_BUILD_RELEASE=1>
    $<$<CONFIG:MinSizeRel>:OKL_BUILD_RELEASE=1>
)

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS okutils EXPORT okutils-targets)

install(DIRECTORY include/okutils
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/okutils"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT okutils-targets
    FILE okutils-targets.cmake
    NAMESPACE oklibs::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okutils
)

configure_package_config_file(
    cmake/okutils-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/okutils-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okutils
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/okutils-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/okutils-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/okutils-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okutils
)
