# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2026 firecat2d developers

add_executable(spritebenchmark main.cpp)

target_link_libraries(
    spritebenchmark
    fc::client
)

install(TARGETS spritebenchmark DESTINATION examples)

if (DEFINED EMSCRIPTEN)
    set_target_properties(
        spritebenchmark
        PROPERTIES LINK_FLAGS ${EXAMPLES_LINK_FLAGS}
    )
endif()
