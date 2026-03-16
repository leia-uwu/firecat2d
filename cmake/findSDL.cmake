# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(findSDL)
    find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
    if (NOT SDL3_FOUND)
        FetchContent_Declare(
            SDL
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG        release-3.4.2
            GIT_SHALLOW ON
        )
        FetchContent_MakeAvailable(SDL)
    endif()
endfunction()
