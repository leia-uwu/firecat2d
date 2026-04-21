# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

if (DEFINED EMSCRIPTEN)
    # output the client wasm and js bundles into a consistent directory
    # so vite can find them
    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/wasm-build/"
    )
    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_CURRENT_SOURCE_DIR}/wasm-build/"
    )
    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
        "${CMAKE_CURRENT_SOURCE_DIR}/wasm-build/"
    )
    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL
        "${CMAKE_CURRENT_SOURCE_DIR}/wasm-build/"
    )
    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO
        "${CMAKE_CURRENT_SOURCE_DIR}/wasm-build/"
    )

    set(CMAKE_EXECUTABLE_SUFFIX ".js" CACHE INTERNAL "")

    set(EXAMPLES_LINK_FLAGS
        "-sENVIRONMENT=web -sMODULARIZE=instance -sFULL_ES3"
    )
endif()

install(
    FILES
    public/assets/bird.png
    public/assets/pipe.png
    DESTINATION
    examples/assets
)

add_subdirectory(flappy_bird)
add_subdirectory(spritebenchmark)
add_subdirectory(grid_demo)
add_subdirectory(shapes)
