# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

set(
    FLAPPY_SRC
    src/main.cpp
    src/game.cpp
)

add_executable(flappy_bird ${FLAPPY_SRC})

target_link_libraries(
    flappy_bird
    fc::client
)

install(TARGETS flappy_bird DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        flappy_bird
        PROPERTIES LINK_FLAGS ${EXAMPLES_LINK_FLAGS}
    )
endif()
