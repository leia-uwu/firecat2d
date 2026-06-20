# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_executable(flappy_bird flappyBird.cpp)

target_link_libraries(
    flappy_bird
    PRIVATE SDL3::SDL3
    fc::client
)

install(TARGETS flappy_bird DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        flappy_bird
        PROPERTIES LINK_FLAGS ${EXAMPLES_LINK_FLAGS}
    )
endif()
