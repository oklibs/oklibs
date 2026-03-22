# Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

cmake_minimum_required(VERSION 3.20)
project(oklibs VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs/okutils)
add_subdirectory(libs/okbitflag)
add_subdirectory(libs/oktest)
