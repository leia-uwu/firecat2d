# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

cmake_minimum_required(VERSION 3.24)

project(firecat2d C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_TESTING)
    include(CTest)
    enable_testing()
endif()

include(FindPkgConfig)
include(FetchContent)
# cmake not printing anything for what could take more than a minute is cringe
# idk why its quiet by default...
set(FETCHCONTENT_QUIET OFF)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(findSDL)
findSDL()

include(findSkia)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/include")
add_subdirectory(src)

option(BUILD_EXAMPLES "Build example games" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
