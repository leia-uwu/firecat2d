# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(finduWebsockets)
    find_package(ZLIB QUIET)
    if (NOT ZLIB_FOUND)
        FetchContent_Declare(
            zlib
            GIT_REPOSITORY "https://github.com/madler/zlib.git"
            GIT_TAG v1.3.1
            GIT_SHALLOW ON
            GIT_SUBMODULES ""
            GIT_PROGRESS ON
        )
        FetchContent_MakeAvailable(zlib)
    endif()

    # thanks to https://stackoverflow.com/a/75848101
    FetchContent_Declare(
        uSockets
        GIT_REPOSITORY https://github.com/uNetworking/uSockets
        GIT_TAG v0.8.8
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
        GIT_PROGRESS ON
    )
    FetchContent_MakeAvailable(uSockets)
    file(GLOB_RECURSE SOURCES ${usockets_SOURCE_DIR}/src/*.c)
    add_library(uSockets ${SOURCES})

    target_include_directories(uSockets PUBLIC ${usockets_SOURCE_DIR}/src)
    target_compile_definitions(uSockets PRIVATE LIBUS_NO_SSL)

    FetchContent_Declare(
        uWebSockets
        GIT_REPOSITORY https://github.com/uNetworking/uWebSockets
        GIT_TAG v20.74.0
        GIT_SHALLOW ON
        GIT_SUBMODULES ""
        GIT_PROGRESS ON
    )
    FetchContent_MakeAvailable(uWebSockets)

    add_library(uWebSockets INTERFACE)
    target_include_directories(uWebSockets INTERFACE ${uwebsockets_SOURCE_DIR}/src/)
    target_link_libraries(uWebSockets INTERFACE uSockets ${ZLIB_LIBRARIES})

endfunction()
