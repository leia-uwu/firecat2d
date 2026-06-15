# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(findSDL)
    find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
    if (NOT SDL3_FOUND)
        FetchContent_Declare(
            SDL
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG        release-3.4.10
            GIT_SHALLOW ON
            GIT_PROGRESS ON
        )
        set(SDL_GPU         OFF CACHE BOOL "" FORCE)
        set(SDL_RENDER      OFF CACHE BOOL "" FORCE)
        set(SDL_CAMERA      OFF CACHE BOOL "" FORCE)
        set(SDL_POWER       OFF CACHE BOOL "" FORCE)
        set(SDL_SENSOR      OFF CACHE BOOL "" FORCE)
        set(SDL_DIALOG      OFF CACHE BOOL "" FORCE)
        set(SDL_TRAY        OFF CACHE BOOL "" FORCE)
        set(SDL_JOYSTICK    OFF CACHE BOOL "" FORCE)
        set(SDL_HAPTIC      OFF CACHE BOOL "" FORCE)
        set(SDL_HIDAPI      OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(SDL)
    endif()
endfunction()
