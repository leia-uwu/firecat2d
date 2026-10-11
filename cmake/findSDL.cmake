# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(findSDL)
    find_package(SDL3 QUIET CONFIG COMPONENTS SDL3-shared)
    if(NOT SDL3_FOUND)
        FetchContent_Declare(
            SDL
            URL
                https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz
            URL_HASH SHA256=9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3
        )
        # gersemi: off
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
        # gersemi: on
        FetchContent_MakeAvailable(SDL)
    endif()
endfunction()
