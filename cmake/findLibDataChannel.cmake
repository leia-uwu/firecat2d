# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

function(findLibDataChannel)
    if (DEFINED EMSCRIPTEN)
        FetchContent_Declare(
            LibDataChannel-wasm
            GIT_REPOSITORY https://github.com/paullouisageneau/datachannel-wasm
            GIT_TAG        v0.4.0
            GIT_SHALLOW ON
            GIT_PROGRESS ON
        )
        FetchContent_MakeAvailable(LibDataChannel-wasm)
        add_library(LibDataChannel::LibDataChannel ALIAS datachannel-wasm)
    else()
        find_package(LibDataChannel)
        if (NOT LibDataChannel_FOUND)
            FetchContent_Declare(
                LibDataChannel
                GIT_REPOSITORY https://github.com/paullouisageneau/libdatachannel
                GIT_TAG        v0.24.5
                GIT_SHALLOW ON
                GIT_PROGRESS ON
            )
            FetchContent_MakeAvailable(LibDataChannel)
        endif()
    endif()

endfunction()
