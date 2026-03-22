# Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.20)
project(okbitflag VERSION 0.1.0 LANGUAGES CXX)

add_library(okbitflag INTERFACE)
add_library(oklibs::okbitflag ALIAS okbitflag)

configure_file(
    cmake/config.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/okbitflag/config.hpp"
    @ONLY
)

target_include_directories(okbitflag INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(okbitflag INTERFACE cxx_std_20)

if(TARGET okutils)
    target_link_libraries(okbitflag INTERFACE okutils)
else()
    find_package(okutils REQUIRED)
    target_link_libraries(okbitflag INTERFACE oklibs::okutils)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS okbitflag EXPORT okbitflag-targets)

install(DIRECTORY include/okbitflag
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/okbitflag"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT okbitflag-targets
    FILE okbitflag-targets.cmake
    NAMESPACE oklibs::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okbitflag
)

configure_package_config_file(
    cmake/okbitflag-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/okbitflag-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okbitflag
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/okbitflag-config-version.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/okbitflag-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/okbitflag-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/okbitflag
)
